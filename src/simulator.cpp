#include "simulator.hpp"
#include "rng.hpp"
#include <random>
#include <stdexcept>

Simulator::Simulator(int years, std::vector<RiskModel> models)
  : years_(years), models_(std::move(models)) {
  if (years_ <= 0) throw std::invalid_argument("years must be > 0");
  if (models_.empty()) throw std::invalid_argument("models must not be empty");
}

std::vector<Event> Simulator::simulate_events(std::uint64_t seed) const {
  Rng rng(seed);

  // quarter assignment: Uniform over 1..4 for each event
  std::uniform_int_distribution<int> qdist(1, 4);

  std::vector<Event> events;
  events.reserve(1000);

  for (int y = 0; y < years_; ++y) {
    for (const auto& m : models_) {
      const int n = m.freq.sample(rng);
      for (int i = 0; i < n; ++i) {
        Event e;
        e.risk = m.risk;
        e.year_index = y;
        e.quarter = qdist(rng.engine());
        e.loss = m.sev.sample(rng);
        events.push_back(e);
      }
    }
  }
  return events;
}

std::vector<std::vector<double>> Simulator::simulate_annual_losses(int sims, std::uint64_t seed) const {
  if (sims <= 0) throw std::invalid_argument("sims must be > 0");
  Rng rng(seed);

  std::vector<std::vector<double>> out(models_.size(), std::vector<double>(static_cast<size_t>(sims), 0.0));

  for (int s = 0; s < sims; ++s) {
    for (size_t r = 0; r < models_.size(); ++r) {
      const auto& m = models_[r];
      const int n = m.freq.sample(rng);
      double sum = 0.0;
      for (int i = 0; i < n; ++i) sum += m.sev.sample(rng);
      out[r][static_cast<size_t>(s)] = sum;
    }
  }
  return out;
}
