# CCAR Operational Loss Aggregator (C++)

Simulates operational loss events (Cyber, Fraud, Algorithmic Trading) and aggregates them into a CCAR-like capital plan format using an Loss Distribution Approach (LDA).

## LDA model
For each risk type r:
- Frequency N_r ~ Poisson(lambda_r)
- Severities X_{r,i} ~ Lognormal(mu_r, sigma_r) (default)
- Annual loss L_r = sum_{i=1..N_r} X_{r,i}

Monte Carlo simulations generate the loss distribution and compute percentiles for CCAR-style capital planning.

## Outputs
- events.csv: event-level simulated losses (time bucketed into quarters)
- capital_plan.csv: aggregated losses by risk type and quarter + annual total
- lda_summary.csv: annual loss distribution percentiles by risk type (P50, P90, P95, P99, P99.9)

## Build
```
mkdir -p build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

## Run
```
./build/ccar_ola --years 1 --sims 20000 --seed 42 --outdir out
```

## Flags
* `--years`   number of planning years to simulate event timelines for (default 1)
* `--sims`    number of Monte Carlo simulations for annual LDA percentiles (default 20000)
* `--seed`    RNG seed (default 12345)
* `--outdir`  output directory (default out)
