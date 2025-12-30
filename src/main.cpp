#include "simulator.hpp"
#include "aggregator.hpp"
#include "io.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

struct Args {
  int years = 1;
  int sims = 20000;
  std::uint64_t seed = 12345;
  std::string outdir = "out";
};

static Args parse_args(int argc, char** argv) {
  Args a;
  for (int i = 1; i < argc; ++i) {
    std::string k = argv[i];
    auto need = [&](const std::string& name) -> std::string {
      if (i + 1 >= argc) throw std::runtime_error("Missing value for " + name);
      return argv[++i];
    };

    if (k == "--years") a.years = std::stoi(need(k));
    else if (k == "--sims") a.sims = std::stoi(need(k));
    else if (k == "--seed") a.seed = static_cast<std::uint64_t>(std::stoull(need(k)));
    else if (k == "--outdir") a.outdir = need(k);
    else if (k == "--help" || k == "-h") {
      std::cout <<
        "ccar_ola --years N --sims M --seed S --outdir DIR\n"
        "Defaults: --years 1 --sims 20000 --seed 12345 --outdir out\n";
      std::exit(0);
    } else {
      throw std::runtime_error("Unknown arg: " + k);
    }
  }
  return a;
}

int main(int argc, char** argv) {
  try {
    const Args args = parse_args(argc, argv);

    // Calibratable “simulated but plausible” parameters.
    // Units: losses in dollars (or any currency); adjust mu/sigma/lambda to match your story.
    // Reminder: Lognormal mean depends on mu and sigma; these are log-space params.
    std::vector<RiskModel> models = {
      { RiskType::Cyber,       PoissonFreq{ 18.0 }, LognormalSev{ 11.2, 1.0 } }, // frequent, medium-heavy
      { RiskType::Fraud,       PoissonFreq{ 10.0 }, LognormalSev{ 11.6, 1.1 } }, // less freq, heavier
      { RiskType::AlgoTrading, PoissonFreq{  4.0 }, LognormalSev{ 12.2, 1.3 } }, // rare, very heavy
    };

    Simulator sim(args.years, models);

    io::ensure_dir(args.outdir);

    // 1) Event-level simulation (timeline) for "Capital Plan" quarter aggregation
    auto events = sim.simulate_events(args.seed);
    io::write_events_csv(args.outdir + "/events.csv", events);

    auto cap_rows = Aggregator::to_capital_plan(events, args.years);
    io::write_capital_plan_csv(args.outdir + "/capital_plan.csv", cap_rows);

    // 2) LDA annual loss distribution percentiles (Monte Carlo)
    auto annual_losses = sim.simulate_annual_losses(args.sims, args.seed + 999);

    std::vector<std::string> risk_names;
    for (const auto& m : sim.models()) risk_names.push_back(to_string(m.risk));

    auto summary = Aggregator::annual_percentiles(risk_names, annual_losses);
    io::write_lda_summary_csv(args.outdir + "/lda_summary.csv", summary);

    std::cout << "Wrote:\n"
              << "  " << args.outdir << "/events.csv\n"
              << "  " << args.outdir << "/capital_plan.csv\n"
              << "  " << args.outdir << "/lda_summary.csv\n";

    return 0;
  } catch (const std::exception& ex) {
    std::cerr << "ERROR: " << ex.what() << "\n";
    return 1;
  }
}
