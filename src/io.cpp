#include "io.hpp"
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <stdexcept>

namespace fs = std::filesystem;

void io::ensure_dir(const std::string& outdir) {
  std::error_code ec;
  fs::create_directories(outdir, ec);
  if (ec) throw std::runtime_error("Failed to create output dir: " + outdir);
}

void io::write_events_csv(const std::string& path, const std::vector<Event>& events) {
  std::ofstream f(path);
  if (!f) throw std::runtime_error("Cannot open " + path);

  f << "year_index,quarter,risk_type,loss\n";
  f << std::fixed << std::setprecision(2);
  for (const auto& e : events) {
    f << e.year_index << "," << e.quarter << "," << to_string(e.risk) << "," << e.loss << "\n";
  }
}

void io::write_capital_plan_csv(const std::string& path, const std::vector<CapitalPlanRow>& rows) {
  std::ofstream f(path);
  if (!f) throw std::runtime_error("Cannot open " + path);

  f << "year_index,risk_type,bucket,loss_sum\n";
  f << std::fixed << std::setprecision(2);
  for (const auto& r : rows) {
    f << r.year_index << "," << r.risk_type << "," << r.bucket << "," << r.loss_sum << "\n";
  }
}

void io::write_lda_summary_csv(const std::string& path,
                              const std::map<std::string, std::map<std::string, double>>& summary) {
  std::ofstream f(path);
  if (!f) throw std::runtime_error("Cannot open " + path);

  f << "risk_type,P50,P90,P95,P99,P99.9\n";
  f << std::fixed << std::setprecision(2);

  for (const auto& kv : summary) {
    const auto& risk = kv.first;
    const auto& m = kv.second;
    f << risk << ","
      << m.at("P50") << ","
      << m.at("P90") << ","
      << m.at("P95") << ","
      << m.at("P99") << ","
      << m.at("P99.9") << "\n";
  }
}
