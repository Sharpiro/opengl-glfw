#pragma once

#include <vector>

template <typename T>
std::vector<T> concat_vector(std::vector<T> *v1, std::vector<T> *v2) {
  auto new_vec = std::vector<T>(*v1);
  new_vec.insert(new_vec.end(), v2->begin(), v2->end());
  return new_vec;
}
