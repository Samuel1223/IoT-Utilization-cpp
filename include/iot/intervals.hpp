#ifndef IOT_INTERVALS_HPP
#define IOT_INTERVALS_HPP

#include <vector>

#include "iot/sample.hpp"

namespace iot {

// Coalesce a collection of half-open [start, end) intervals into a sorted set of
// disjoint, non-adjacent intervals. Empty inputs (start >= end) are dropped.
// Overlapping and touching/adjacent intervals (a.end == b.start) are merged.
std::vector<Interval> merge_intervals(std::vector<Interval> intervals);

// Total covered duration: the sum of (end - start) over the given intervals.
// Empty intervals (start >= end) contribute 0.
long long total_seconds(const std::vector<Interval>& intervals);

// Clamp the interval to the window [window_start, window_end). Returns the
// overlapping sub-interval, or a zero-length interval when there is no overlap.
Interval clamp_interval(const Interval& iv, long long window_start,
                        long long window_end);

}  // namespace iot

#endif  // IOT_INTERVALS_HPP
