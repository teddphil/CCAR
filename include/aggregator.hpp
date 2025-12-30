#pragma once
#include "types.hpp"
#include <vector>
#include <map>
#include <string>

class Aggregator {
public:
  // Aggregate event-level losses into capital plan rows (Q1..Q4 + Annual) per year and risk type
  static std::vector<CapitalPlanRow> to_capital_plan(const std::vector<Event>& events, int years);

  // Compute empirical percentiles from simulated annual losses (per risk type)
  // returns rows keyed by risk_type string: map[risk] -> map[pct_label] -> value
  static std::map<std::string, std::map<std::string, double>>
  annual_percentiles(const std::vector<std::string>& risk_names,
                     const std::vector<std::vector<double>>& annual_losses);
};
