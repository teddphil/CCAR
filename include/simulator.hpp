#pragma once
#include "types.hpp"
#include "distributions.hpp"
#include <vector>

struct RiskModel {
  RiskType risk;
  PoissonFreq freq;
  LognormalSev sev; // keep default lognormal for simplicity
};

class Simulator {
public:
  Simulator(int years, std::vector<RiskModel> models);

  // Event timeline simulation: generate event-level data bucketed into quarters
  std::vector<Event> simulate_events(std::uint64_t seed) const;

  // LDA annual loss distribution (Monte Carlo): returns matrix [risk_index][sim]
  std::vector<std::vector<double>> simulate_annual_losses(int sims, std::uint64_t seed) const;

  const std::vector<RiskModel>& models() const { return models_; }
  int years() const { return years_; }

private:
  int years_;
  std::vector<RiskModel> models_;
};
