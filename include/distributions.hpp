#pragma once
#include "rng.hpp"
#include <random>
#include <stdexcept>

struct PoissonFreq {
  double lambda; // > 0
  int sample(Rng& rng) const;
};

struct LognormalSev {
  // log(X) ~ Normal(mu, sigma)
  double mu;
  double sigma; // > 0
  double sample(Rng& rng) const;
};

// Optional: Pareto severity (useful for heavy-tail demo)
struct ParetoSev {
  // X = xm / U^(1/alpha), U~Uniform(0,1)
  double xm;     // scale > 0
  double alpha;  // shape > 0
  double sample(Rng& rng) const;
};
