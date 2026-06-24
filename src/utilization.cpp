#include "iot/utilization.hpp"

#include <stdexcept>
#include <utility>

#include "iot/intervals.hpp"

namespace iot {

namespace {
[[noreturn]] void not_implemented() {
  throw std::logic_error("not implemented");
}
}  // namespace

UtilizationAnalyzer::UtilizationAnalyzer(Mode mode,
                                         WorkingPredicate speed_predicate)
    : mode_(mode), speed_predicate_(std::move(speed_predicate)) {}

Mode UtilizationAnalyzer::mode() const { return mode_; }

std::vector<Interval> UtilizationAnalyzer::working_intervals(
    const std::vector<Sample>&) const {
  not_implemented();
}

long long UtilizationAnalyzer::working_seconds(
    const std::vector<Sample>&) const {
  not_implemented();
}

long long UtilizationAnalyzer::window_seconds(
    const std::vector<Sample>&) const {
  not_implemented();
}

double UtilizationAnalyzer::utilization_rate(
    const std::vector<Sample>&) const {
  not_implemented();
}

double UtilizationAnalyzer::utilization_rate(const std::vector<Sample>&,
                                             long long, long long) const {
  not_implemented();
}

}  // namespace iot
