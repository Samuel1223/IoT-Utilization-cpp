#include "iot/clustering.hpp"

#include <algorithm>
#include <cstddef>
#include <stdexcept>

namespace iot {

namespace {

struct Point {
  double t;
  double v;
};

double squared_distance(const Point& a, const Point& b) {
  const double dt = a.t - b.t;
  const double dv = a.v - b.v;
  return dt * dt + dv * dv;
}

}  // namespace

std::vector<int> kmeans_cluster(const std::vector<Sample>& samples, int k,
                                int max_iterations) {
  if (k <= 0) {
    throw std::invalid_argument("k must be positive");
  }
  if (max_iterations <= 0) {
    throw std::invalid_argument("max_iterations must be positive");
  }
  const std::size_t n = samples.size();
  if (static_cast<std::size_t>(k) > n) {
    throw std::invalid_argument("k must not exceed the number of samples");
  }

  // Per-axis min-max bounds.
  long long t_min = samples[0].timestamp;
  long long t_max = samples[0].timestamp;
  double v_min = samples[0].value;
  double v_max = samples[0].value;
  for (const auto& s : samples) {
    t_min = std::min(t_min, s.timestamp);
    t_max = std::max(t_max, s.timestamp);
    v_min = std::min(v_min, s.value);
    v_max = std::max(v_max, s.value);
  }

  std::vector<Point> points(n);
  for (std::size_t i = 0; i < n; ++i) {
    points[i].t =
        t_max > t_min ? static_cast<double>(samples[i].timestamp - t_min) /
                            static_cast<double>(t_max - t_min)
                      : 0.0;
    points[i].v =
        v_max > v_min ? (samples[i].value - v_min) / (v_max - v_min) : 0.0;
  }

  // Deterministic initialization: order by (timestamp, value, index), then pick
  // evenly spaced ordered positions.
  std::vector<std::size_t> order(n);
  for (std::size_t i = 0; i < n; ++i) order[i] = i;
  std::sort(order.begin(), order.end(), [&](std::size_t a, std::size_t b) {
    if (samples[a].timestamp != samples[b].timestamp) {
      return samples[a].timestamp < samples[b].timestamp;
    }
    if (samples[a].value != samples[b].value) {
      return samples[a].value < samples[b].value;
    }
    return a < b;
  });

  std::vector<Point> centroids(static_cast<std::size_t>(k));
  for (int j = 0; j < k; ++j) {
    const std::size_t pos = (static_cast<std::size_t>(j) * n) /
                            static_cast<std::size_t>(k);
    centroids[static_cast<std::size_t>(j)] = points[order[pos]];
  }

  std::vector<int> assignments(n, -1);
  std::vector<int> previous;
  for (int iter = 0; iter < max_iterations; ++iter) {
    for (std::size_t i = 0; i < n; ++i) {
      int best = 0;
      double best_d = squared_distance(points[i], centroids[0]);
      for (int j = 1; j < k; ++j) {
        const double d =
            squared_distance(points[i], centroids[static_cast<std::size_t>(j)]);
        if (d < best_d) {  // strict '<' keeps the lowest index on ties
          best_d = d;
          best = j;
        }
      }
      assignments[i] = best;
    }
    if (assignments == previous) break;
    previous = assignments;

    std::vector<double> sum_t(static_cast<std::size_t>(k), 0.0);
    std::vector<double> sum_v(static_cast<std::size_t>(k), 0.0);
    std::vector<int> count(static_cast<std::size_t>(k), 0);
    for (std::size_t i = 0; i < n; ++i) {
      const std::size_t c = static_cast<std::size_t>(assignments[i]);
      sum_t[c] += points[i].t;
      sum_v[c] += points[i].v;
      count[c]++;
    }
    for (int j = 0; j < k; ++j) {
      const std::size_t c = static_cast<std::size_t>(j);
      if (count[c] > 0) {
        centroids[c].t = sum_t[c] / count[c];
        centroids[c].v = sum_v[c] / count[c];
      }
    }
  }

  return assignments;
}

}  // namespace iot
