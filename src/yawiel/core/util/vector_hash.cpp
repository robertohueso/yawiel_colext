#ifndef YAWIEL_CORE_UTIL_VECTOR_HASH_CPP
#define YAWIEL_CORE_UTIL_VECTOR_HASH_CPP

#include <vector>
#include <boost/container_hash/hash.hpp>

namespace std {
  template <> struct hash<vector<size_t>>
  {
    //! Hash function for an n-gram vector.
    size_t operator()(const vector<size_t>& x) const
    {
      return boost::hash_range(x.cbegin(), x.cend());
    }
  };
}

#endif
