#ifndef IOT_CLUSTERING_HPP
#define IOT_CLUSTERING_HPP

#include <vector>

#include "iot/sample.hpp"

namespace iot {

// Deterministic 2-D k-means (Lloyd's algorithm) over a machine's IoT history,
// clustering the (timestamp, value) points into `k` groups. Both axes are
// min-max normalized to [0, 1] independently before clustering, so the two very
// different units (time vs reading) are comparable; distance is squared
// Euclidean in that normalized space.
//
// The procedure is fully deterministic (no randomness):
//   * Normalize: per axis, x' = (x - min) / (max - min), or 0 if max == min.
//   * Initialize: order the samples by (timestamp, then value, then input
//     index); the initial centroid of cluster j (0-based) is the normalized
//     point at ordered position (j * n) / k (integer division).
//   * Assign: each point goes to the nearest centroid by squared Euclidean
//     distance; ties go to the lowest cluster index.
//   * Update: each centroid becomes the mean of the points currently assigned
//     to it (from one assignment snapshot); an empty cluster keeps its centroid.
//   * Stop: when an assignment repeats the previous iteration's assignment, or
//     after `max_iterations` assignment steps.
//
// Returns the cluster index in [0, k) for every sample, in input order.
// Throws std::invalid_argument if k <= 0, max_iterations <= 0, or k exceeds the
// number of samples.
std::vector<int> kmeans_cluster(const std::vector<Sample>& samples, int k,
                                int max_iterations = 100);

// A 3-feature IoT reading: a timestamp plus two sensor channels. `timestamp` is
// always present; either `value` or `value2` may be MISSING, encoded as NaN.
struct Reading3D {
  long long timestamp;
  double value;   // NaN if missing
  double value2;  // NaN if missing
};

// Deterministic 3-D k-means (Lloyd's algorithm) over Reading3D points
// (timestamp, value, value2), with mean-imputation of missing features.
//
// The procedure is fully deterministic (no randomness):
//   * Impute (BEFORE normalizing): for each axis, replace every missing (NaN)
//     entry with the mean of that axis's PRESENT values. If an axis has no
//     present values at all, its missing entries become 0. (timestamp is never
//     missing.)
//   * Normalize: per axis, x' = (x - min) / (max - min) over the imputed values,
//     or 0 if max == min. Distance is squared Euclidean in the 3-D normalized
//     space.
//   * Initialize: order the readings by (timestamp, then imputed value, then
//     imputed value2, then input index); centroid j (0-based) starts at the
//     normalized point at ordered position (j * n) / k (integer division).
//   * Assign: nearest centroid by squared Euclidean distance; ties go to the
//     lowest cluster index.
//   * Update: each centroid becomes the mean of its assigned points (one
//     snapshot); an empty cluster keeps its centroid.
//   * Stop: when an assignment repeats the previous one, or after
//     `max_iterations` assignment steps.
//
// Returns the cluster index in [0, k) for every reading, in input order.
// Throws std::invalid_argument if k <= 0, max_iterations <= 0, or k exceeds the
// number of readings.
std::vector<int> kmeans_cluster_3d(const std::vector<Reading3D>& readings, int k,
                                   int max_iterations = 100);

}  // namespace iot

#endif  // IOT_CLUSTERING_HPP
