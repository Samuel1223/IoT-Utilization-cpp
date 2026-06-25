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

}  // namespace iot

#endif  // IOT_CLUSTERING_HPP
