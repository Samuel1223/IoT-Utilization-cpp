#include "iot/clustering.hpp"

#include <stdexcept>

namespace iot {

std::vector<int> kmeans_cluster_3d(const std::vector<Reading3D>&, int, DistanceFn,
                                   CentroidFn, int) {
  throw std::logic_error("not implemented");
}

std::vector<ClusterTrend> calculate_cluster_trends(const std::vector<Reading3D>&,
                                                   const std::vector<int>&, int) {
  throw std::logic_error("not implemented");
}

}  // namespace iot
