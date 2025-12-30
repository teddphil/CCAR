#pragma once
#include <random>

class Rng {
public:
  explicit Rng(std::uint64_t seed) : eng_(static_cast<std::mt19937_64::result_type>(seed)) {}

  std::mt19937_64& engine() { return eng_; }

private:
  std::mt19937_64 eng_;
};
