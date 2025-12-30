#pragma once
#include "types.hpp"
#include <string>
#include <vector>
#include <map>

namespace io {
  void ensure_dir(const std::string& outdir);

  void write_events_csv(const std::string& path, const std::vector<Event>& events);

  void write_capital_plan_csv(const std::string& path, const std::vector<CapitalPlanRow>& rows);

  void write_lda_summary_csv(const std::string& path,
                             const std::map<std::string, std::map<std::string, double>>& summary);
}
