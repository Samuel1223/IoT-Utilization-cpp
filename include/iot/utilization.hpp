#ifndef IOT_UTILIZATION_HPP
#define IOT_UTILIZATION_HPP

#include <vector>

#include "iot/sample.hpp"

namespace iot {

// Selects which rule decides whether the machine was working during the segment
// between two consecutive samples.
enum class Mode { Position, Speed };

// Computes a machine's utilization rate from its time-ordered history of
// readings. The history is a list of (timestamp, value) Samples; the analyzer
// walks consecutive pairs and decides, per the configured Mode, whether the
// machine was working during each segment.
class UtilizationAnalyzer {
 public:
  explicit UtilizationAnalyzer(Mode mode);

  Mode mode() const;

  // The merged, sorted, disjoint set of working intervals across the history.
  std::vector<Interval> working_intervals(const std::vector<Sample>& samples) const;

  // Total working duration (sum of the working intervals' durations).
  long long working_seconds(const std::vector<Sample>& samples) const;

  // The observation window: last timestamp minus first timestamp (0 if < 2).
  long long window_seconds(const std::vector<Sample>& samples) const;

  // working_seconds / window_seconds, in [0, 1]; 0 when the window is empty.
  double utilization_rate(const std::vector<Sample>& samples) const;

 private:
  Mode mode_;
};

}  // namespace iot

#endif  // IOT_UTILIZATION_HPP
