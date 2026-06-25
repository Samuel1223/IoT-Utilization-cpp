#include "iot/clustering.hpp"

#include <stdexcept>

namespace iot {

std::vector<int> kmeans_cluster(const std::vector<Sample>& /*samples*/,
                                int /*k*/, int /*max_iterations*/) {
  throw std::logic_error("not implemented");
}

}  // namespace iot
