# CCAR Operational Loss Aggregator (C++)

### Overview
The project simulates operational loss events (Cyber, Fraud, Algorithmic Trading) and aggregates them into a CCAR-like capital plan format using an Loss Distribution Approach (LDA). It uses Monte Carlo simulations to generate the loss distribution and compute percentiles for CCAR-style capital planning.

### Models
For each risk type $r$, there are

Frequency 
: $N_r \sim \text{Poisson}(\lambda_r)$

Severities
: $X_{r,i} \sim \text{Lognormal}(\mu_r, \sigma_r)$

Annual loss
: $L_r = \sum_{i=1..N_r} X_{r,i}$
