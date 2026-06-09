#ifndef IOT_SAMPLE_HPP
#define IOT_SAMPLE_HPP

namespace iot {

// A single reading from a machine's history log. `timestamp` is an integer time
// unit (e.g. epoch seconds); `value` is the reading itself -- a position value
// in Position mode, or a speed value in Speed mode.
struct Sample {
  long long timestamp;
  double value;
};

// A half-open time interval [start, end). Its duration is `end - start`; an
// interval with start >= end is empty (zero duration).
struct Interval {
  long long start;
  long long end;
};

inline bool operator==(const Interval& a, const Interval& b) {
  return a.start == b.start && a.end == b.end;
}
inline bool operator!=(const Interval& a, const Interval& b) { return !(a == b); }

}  // namespace iot

#endif  // IOT_SAMPLE_HPP
