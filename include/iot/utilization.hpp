#ifndef IOT_UTILIZATION_HPP
#define IOT_UTILIZATION_HPP

#include <functional>
#include <vector>

#include "iot/sample.hpp"

namespace iot {

// Selects which rule decides whether the machine was working during the segment
// between two consecutive samples.
enum class Mode { Position, Speed };

// Decides, in Speed mode, whether the machine was working during the segment
// bounded by two consecutive samples `a` (earlier) and `b` (later).
using WorkingPredicate = std::function<bool(const Sample& a, const Sample& b)>;

// The default Speed-mode rule: the machine is working across the segment only
// when both endpoint speed readings are nonzero.
inline bool both_ends_nonzero(const Sample& a, const Sample& b) {
  return a.value != 0.0 && b.value != 0.0;
}

// Computes a machine's utilization rate from its time-ordered history of
// readings. The history is a list of (timestamp, value) Samples; the analyzer
// walks consecutive pairs and decides, per the configured Mode, whether the
// machine was working during each segment.
class UtilizationAnalyzer {
 public:
  // In Speed mode, `speed_predicate` decides whether each segment counts as
  // working; it defaults to `both_ends_nonzero`. Position mode ignores it.
  explicit UtilizationAnalyzer(
      Mode mode, WorkingPredicate speed_predicate = both_ends_nonzero);

  Mode mode() const;

  // The merged, sorted, disjoint set of working intervals across the history.
  std::vector<Interval> working_intervals(const std::vector<Sample>& samples) const;

  // Total working duration (sum of the working intervals' durations).
  long long working_seconds(const std::vector<Sample>& samples) const;

  // The observation window: last timestamp minus first timestamp (0 if < 2).
  long long window_seconds(const std::vector<Sample>& samples) const;

  // working_seconds / window_seconds, in [0, 1]; 0 when the window is empty.
  double utilization_rate(const std::vector<Sample>& samples) const;

  // Utilization over an explicit observation window [window_start, window_end):
  // the working time that falls inside the window, divided by the window length
  // (window_end - window_start). Working intervals are clamped to the window
  // before summing. Returns 0.0 when the window is empty (window_end <=
  // window_start).
  double utilization_rate(const std::vector<Sample>& samples,
                          long long window_start, long long window_end) const;

 private:
  Mode mode_;
  WorkingPredicate speed_predicate_;
};

}  // namespace iot

#endif  // IOT_UTILIZATION_HPP
