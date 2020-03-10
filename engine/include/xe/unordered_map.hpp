//
// Created by FLXR on 9/3/2019.
//

#ifndef XE_UNORDERED_MAP_HPP
#define XE_UNORDERED_MAP_HPP

#include <unordered_map>

namespace xe {

  template<typename K, typename V>
  using unordered_map = std::unordered_map<K, V, std::hash<K>, std::equal_to<K>>;

}

#endif //XE_UNORDERED_MAP_HPP
