#pragma once
#include <cstdint>
#include <string>
#include <vector>

enum class RiskType : std::uint8_t {
  Cyber = 0,
  Fraud = 1,
  AlgoTrading = 2
};

inline std::string to_string(RiskType r) {
  switch (r) {
    case RiskType::Cyber: return "Cyber";
    case RiskType::Fraud: return "Fraud";
    case RiskType::AlgoTrading: return "AlgorithmicTrading";
  }
  return "Unknown";
}

struct Event {
  RiskType risk;
  int year_index;     // 0..years-1
  int quarter;        // 1..4
  double loss;        // positive
};

struct CapitalPlanRow {
  int year_index;          // 0..years-1
  std::string risk_type;   // "Cyber", ...
  std::string bucket;      // "Q1".."Q4" or "Annual"
  double loss_sum;         // aggregate
};
