#include "distributions.hpp"
#include <cmath>

int PoissonFreq::sample(Rng& rng) const {
  if (lambda <= 0.0) throw std::invalid_argument("Poisson lambda must be > 0");
  std::poisson_distribution<int> dist(lambda);
  return dist(rng.engine());
}

double LognormalSev::sample(Rng& rng) const {
  if (sigma <= 0.0) throw std::invalid_argument("Lognormal sigma must be > 0");
  std::normal_distribution<double> dist(mu, sigma);
  const double z = dist(rng.engine());
  // exp for lognormal
  return std::exp(z);
}

double ParetoSev::sample(Rng& rng) const {
  if (xm <= 0.0 || alpha <= 0.0) throw std::invalid_argument("Pareto params must be > 0");
  std::uniform_real_distribution<double> u(0.0, 1.0);
  double U = u(rng.engine());
  // avoid exactly 0
  if (U <= 0.0) U = std::numeric_limits<double>::min();
  return xm / std::pow(U, 1.0 / alpha);
}
