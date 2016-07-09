#include <iostream>
#include <string>

#define LRU_DEBUG 1
#include "lru.hpp"

struct kt { std::string kv1, kv2; };
struct vt { std::string vv1, vv2; };

namespace std {
template<> struct hash<kt> {
  typedef kt argument_type;
  typedef std::size_t result_type;
  result_type operator()(argument_type const& s) const {
    result_type const h1 ( std::hash<std::string>()(s.kv1) );
    result_type const h2 ( std::hash<std::string>()(s.kv2) );
    return h1 ^ (h2 << 1);
  }
};
}

namespace lru_calc {

template <>
size_t real_sizeof<vt>(const vt & key) {
  return sizeof(key) + key.vv1.capacity() + key.vv2.capacity();
}
}


int main () {
  lru<kt, vt> lc(100000);
  lc.insert(kt(), {std::string(1000, 'a'), std::string(10000, 'b')});
}
