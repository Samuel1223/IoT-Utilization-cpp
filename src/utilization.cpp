#include "iot/utilization.hpp"

#include <cstddef>
#include <stdexcept>
#include <utility>

#include "iot/intervals.hpp"

namespace iot {

namespace {

// Timestamps must be non-decreasing; equal timestamps are allowed. A strictly
// decreasing step is a malformed history.
void check_ordered(const std::vector<Sample>& samples) {
  for (std::size_t i = 1; i < samples.size(); ++i) {
    if (samples[i].timestamp < samples[i - 1].timestamp) {
      throw std::invalid_argument(
          "samples must be in non-decreasing timestamp order");
    }
  }
}

}  // namespace

UtilizationAnalyzer::UtilizationAnalyzer(Mode mode,
                                         WorkingPredicate speed_predicate,
                                         long long min_working_seconds)
    : mode_(mode),
      speed_predicate_(std::move(speed_predicate)),
      min_working_seconds_(min_working_seconds) {}

Mode UtilizationAnalyzer::mode() const { return mode_; }

std::vector<Interval> UtilizationAnalyzer::working_intervals(
    const std::vector<Sample>& samples) const {
  if (min_working_seconds_ < 0) {
    throw std::invalid_argument("min_working_seconds must be non-negative");
  }
  check_ordered(samples);
  std::vector<Interval> raw;
  for (std::size_t i = 1; i < samples.size(); ++i) {
    const Sample& a = samples[i - 1];
    const Sample& b = samples[i];
    // Position moved across the segment, or (in Speed mode) the configured
    // predicate judged the segment as working.
    const bool working =
        mode_ == Mode::Position ? a.value != b.value : speed_predicate_(a, b);
    // Debounce per raw segment, before merging: a segment shorter than the
    // threshold is dropped, even if it would coalesce with a neighbor.
    if (working && b.timestamp - a.timestamp >= min_working_seconds_) {
      raw.push_back(Interval{a.timestamp, b.timestamp});
    }
  }
  return merge_intervals(std::move(raw));
}

long long UtilizationAnalyzer::working_seconds(
    const std::vector<Sample>& samples) const {
  return total_seconds(working_intervals(samples));
}

long long UtilizationAnalyzer::window_seconds(
    const std::vector<Sample>& samples) const {
  check_ordered(samples);
  if (samples.size() < 2) return 0;
  return samples.back().timestamp - samples.front().timestamp;
}

double UtilizationAnalyzer::utilization_rate(
    const std::vector<Sample>& samples) const {
  const long long window = window_seconds(samples);
  if (window <= 0) return 0.0;
  const long long working = working_seconds(samples);
  return static_cast<double>(working) / static_cast<double>(window);
}

double UtilizationAnalyzer::utilization_rate(const std::vector<Sample>& samples,
                                             long long window_start,
                                             long long window_end) const {
  if (window_end <= window_start) return 0.0;
  long long working = 0;
  for (const auto& w : working_intervals(samples)) {
    const Interval c = clamp_interval(w, window_start, window_end);
    working += c.end - c.start;
  }
  return static_cast<double>(working) /
         static_cast<double>(window_end - window_start);
}

}  // namespace iot
