# Usage

### Build

```
mkdir -p build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

### Run

```
./build/ccar_ola --years 1 --sims 20000 --seed 42 --outdir out
```

### Flags
`--years`
: number of planning years to simulate event timelines for (default 1)

`--sims`
: number of Monte Carlo simulations for annual LDA percentiles (default 20000)

`--seed`
: RNG seed (default 12345)

`--outdir`
: output directory (default out)

## Outputs
events.csv
: event-level simulated losses (time bucketed into quarters)

capital_plan.csv
: aggregated losses by risk type and quarter + annual total

lda_summary.csv
: annual loss distribution percentiles by risk type (P50, P90, P95, P99, P99.9)