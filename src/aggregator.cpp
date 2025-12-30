#include "aggregator.hpp"
#include <algorithm>
#include <stdexcept>

static double percentile(std::vector<double> v, double p) {
  if (v.empty()) throw std::invalid_argument("percentile: empty vector");
  if (p < 0.0 || p > 1.0) throw std::invalid_argument("percentile p must be in [0,1]");
  std::sort(v.begin(), v.end());
  const double idx = p * (static_cast<double>(v.size() - 1));
  const auto i = static_cast<size_t>(idx);
  const double frac = idx - static_cast<double>(i);
  if (i + 1 >= v.size()) return v.back();
  return v[i] * (1.0 - frac) + v[i + 1] * frac;
}

std::vector<CapitalPlanRow> Aggregator::to_capital_plan(const std::vector<Event>& events, int years) {
  if (years <= 0) throw std::invalid_argument("years must be > 0");

  // key: (year, risk_string, bucket)
  struct Key {
    int y;
    std::string r;
    std::string b;
    bool operator<(const Key& o) const {
      if (y != o.y) return y < o.y;
      if (r != o.r) return r < o.r;
      return b < o.b;
    }
  };

  std::map<Key, double> sums;

  for (const auto& e : events) {
    const std::string r = to_string(e.risk);
    const std::string q = "Q" + std::to_string(e.quarter);
    sums[{e.year_index, r, q}] += e.loss;
    sums[{e.year_index, r, "Annual"}] += e.loss;
  }

  // Ensure rows exist even if no events occurred (CCAR tables often expect explicit zeros)
  const std::vector<std::string> buckets = {"Q1","Q2","Q3","Q4","Annual"};

  // Discover risks present in events
  std::vector<std::string> risks;
  {
    std::map<std::string, bool> seen;
    for (const auto& e : events) seen[to_string(e.risk)] = true;
    for (const auto& kv : seen) risks.push_back(kv.first);
  }

  std::vector<CapitalPlanRow> rows;
  for (int y = 0; y < years; ++y) {
    for (const auto& r : risks) {
      for (const auto& b : buckets) {
        const Key k{y, r, b};
        const double val = (sums.find(k) == sums.end()) ? 0.0 : sums[k];
        rows.push_back(CapitalPlanRow{y, r, b, val});
      }
    }
  }

  // Add totals across risks (optional but useful)
  for (int y = 0; y < years; ++y) {
    for (const auto& b : buckets) {
      double total = 0.0;
      for (const auto& r : risks) {
        const Key k{y, r, b};
        total += (sums.find(k) == sums.end()) ? 0.0 : sums[k];
      }
      rows.push_back(CapitalPlanRow{y, "TotalOperationalRisk", b, total});
    }
  }

  return rows;
}

std::map<std::string, std::map<std::string, double>>
Aggregator::annual_percentiles(const std::vector<std::string>& risk_names,
                               const std::vector<std::vector<double>>& annual_losses) {
  if (risk_names.size() != annual_losses.size())
    throw std::invalid_argument("risk_names and annual_losses size mismatch");

  std::map<std::string, std::map<std::string, double>> out;
  const std::vector<std::pair<std::string,double>> pcts = {
    {"P50", 0.50},
    {"P90", 0.90},
    {"P95", 0.95},
    {"P99", 0.99},
    {"P99.9", 0.999},
  };

  for (size_t i = 0; i < risk_names.size(); ++i) {
    std::map<std::string, double> row;
    for (const auto& pp : pcts) {
      row[pp.first] = percentile(annual_losses[i], pp.second);
    }
    out[risk_names[i]] = row;
  }
  return out;
}
