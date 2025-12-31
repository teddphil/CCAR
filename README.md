# CCAR Operational Loss Aggregator (C++)

### Overview
Simulates operational loss events (Cyber, Fraud, Algorithmic Trading) and aggregates them into a CCAR-like capital plan format using an Loss Distribution Approach (LDA). Monte Carlo simulations generate the loss distribution and compute percentiles for CCAR-style capital planning.

### Model
For each risk type $r$, there are

Frequency 
$$N_r \~ \text{Poisson}(\lambda_r)$$

Severities
$$X_{r,i} \~ \text{Lognormal}(\mu_r, \sigma_r)$$

Annual loss
$$L_r = sum_{i=1..N_r} X_{r,i}$$
