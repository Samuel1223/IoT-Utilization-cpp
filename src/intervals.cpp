#include "iot/intervals.hpp"

#include <algorithm>

namespace iot {

std::vector<Interval> merge_intervals(std::vector<Interval> intervals) {
  std::vector<Interval> nonempty;
  nonempty.reserve(intervals.size());
  for (const auto& iv : intervals) {
    if (iv.end > iv.start) nonempty.push_back(iv);
  }
  std::sort(nonempty.begin(), nonempty.end(),
            [](const Interval& a, const Interval& b) {
              if (a.start != b.start) return a.start < b.start;
              return a.end < b.end;
            });

  std::vector<Interval> out;
  for (const auto& iv : nonempty) {
    if (!out.empty() && iv.start <= out.back().end) {
      out.back().end = std::max(out.back().end, iv.end);
    } else {
      out.push_back(iv);
    }
  }
  return out;
}

long long total_seconds(const std::vector<Interval>& intervals) {
  long long total = 0;
  for (const auto& iv : intervals) {
    if (iv.end > iv.start) total += iv.end - iv.start;
  }
  return total;
}

Interval clamp_interval(const Interval& iv, long long window_start,
                        long long window_end) {
  const long long s = std::max(iv.start, window_start);
  const long long e = std::min(iv.end, window_end);
  if (e <= s) return Interval{s, s};
  return Interval{s, e};
}

}  // namespace iot
