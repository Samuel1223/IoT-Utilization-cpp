#include "iot/clustering.hpp"

#include <stdexcept>

namespace iot {

std::vector<int> kmeans_cluster_3d(const std::vector<Reading3D>& /*readings*/,
                                   int /*k*/, int /*max_iterations*/) {
  throw std::logic_error("not implemented");
}

}  // namespace iot
