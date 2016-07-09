/*

lru cache with defined maximum memory usage

usage:
lru<key_type=std::string, value_type=std::string> cache(10000) // pass size limit in bytes
bool cache.insert(key, value) // return true if successful
value_type * stored = cache.get(key) : pointer to stored value, nullptr if not found
bool cache.remove(key) // return true if successful

for your custom types need specify:
std::hash<key_type>()()
namespace lru_calc { template <> size_t real_sizeof<value_type>(const value_type &) {...} } // real size calculator

*/

#include <string> //only for default type
#include <list>
#include <map>
#include <functional>

#if LRU_DEBUG
#include <iostream>
#define LRU_PRINT(a) std::cout << a;
#else
#define LRU_PRINT(a) ;
#endif

namespace lru_calc {
template <typename T> size_t real_sizeof(const T & key) { return sizeof(key); }
template <> size_t real_sizeof<std::string>(const std::string & key) {  return sizeof(key) + key.capacity(); }
}

template <class key_type = std::string, class value_type = std::string, class hasher = std::hash<key_type> >
class lru {
  using key_store_type = decltype(hasher()(key_type()));
  using sorted_type = std::list<key_store_type>;
  using sorted_iter = typename sorted_type::iterator;
  using storage_type = std::map<key_store_type, std::pair<value_type, sorted_iter> >;
  using storage_iter = typename storage_type::iterator;

  storage_type storage;
  sorted_type sorted_keys;

  size_t stored_size = 0;
  const size_t limit;
  const size_t storage_node_size;
  const size_t sorted_node_size;
  const size_t max_size;

  inline size_t calc_stored_size(const storage_iter & it) const {
    return 0
           + sizeof(it->first)  // map key (hash)
           + lru_calc::real_sizeof<>(it->second.first)  // map value
           + sizeof(it->second.second) // map iterator to list
           + storage_node_size // map internal
           + sizeof(key_store_type) // list
           + sorted_node_size // list internal
           ;
  };
  inline size_t calc_possible_size(const value_type &value) const {
    return 0
           + sizeof(key_store_type)
           + lru_calc::real_sizeof<>(value)
           + sizeof(sorted_iter)
           + storage_node_size
           + sizeof(key_store_type)
           + sorted_node_size;
  };

  inline key_store_type calc_hash(const key_type &key) const {
    return hasher()(key);
  };

  inline key_store_type calc_elem_size(key_store_type hash) {
    auto it = storage.find(hash);
    if (it == storage.end())
      return 0;
    return calc_stored_size(it);
  };

public:
  lru(size_t limit_) :
    storage_node_size(sizeof(void*) * 4), //bit larger than need (~3.5)
    sorted_node_size(sizeof(void*) * 3),
    limit(limit_),
    stored_size(sizeof(*this)),
    max_size(limit - stored_size) {
    LRU_PRINT("initial stored_size=" << stored_size << "\n");
  }

  bool insert(const key_type & key, const value_type &value) {
    size_t want_size = calc_possible_size(value);
    if (want_size > max_size) {
      LRU_PRINT("NOT inserted max_size=" << max_size << " want_size=" << want_size << " stored_size=" << stored_size << " storage=" << storage.size() << " sorted_keys=" << sorted_keys.size() << "\n");
      return false;
    }
    auto hash = calc_hash(key);
    auto it = storage.find(hash);
    size_t current_size = 0;
    if (it != storage.end()) {
      current_size = calc_stored_size(it);
    }

    //need clean cache?
    size_t erased = 0;
    if (current_size < want_size && (stored_size - current_size) + want_size > limit) {
      //clean
      for (auto sit = sorted_keys.rbegin(); sit != sorted_keys.rend(); ++sit)  {
        if (hash == *sit) {
          continue;
        }
        auto storage_it = storage.find(*sit);
        stored_size -= calc_stored_size( storage_it );
        storage.erase( storage_it );
        sorted_keys.erase(storage_it->second.second);
        LRU_PRINT("cleaning hash=" << *sit << " clean=" << calc_stored_size( storage_it ) << " want_size=" << want_size << " stored_size=" << stored_size << " storage=" << storage.size() << " sorted_keys=" << sorted_keys.size() << "\n");
        ++erased;
        if ((stored_size - current_size) + want_size < limit) {
          break;
        }
      }

      if ((stored_size - current_size) + want_size > limit) {
        // should never happen
        return false;
      }

    }

    try {
      // replace if exists
      if (current_size) {
        // get new valid iterator
        if (erased)
          it = storage.find(hash);
        sorted_keys.erase(it->second.second);
        sorted_keys.emplace_front(hash);
        it->second = std::make_pair(value, sorted_keys.begin());
        stored_size -= current_size;
      } else {
        sorted_keys.emplace_front(hash);
        auto empr = storage.emplace(hash, std::make_pair(value, sorted_keys.begin()));
        it = empr.first;
      }
    } catch (...) {
      LRU_PRINT("memory out? on inserting want_size=" << want_size << "\n");
      return false;
    }
    auto new_size = calc_stored_size(it);
    stored_size += new_size;

    LRU_PRINT("inserted new_size=" << new_size << " want_size=" << want_size
              /*<< " key.size="<<key.size() << " key.capacity="<<key.capacity() << " value.size="<< value.size() << " value.capacity="<< value.capacity() */
              << " stored_size=" << stored_size << " storage=" << storage.size() << " sorted_keys=" << sorted_keys.size() << " hash=" << hash << "\n");

    return true;
  }

  value_type * get(const key_type & key) {
    auto hash = calc_hash(key);
    auto it = storage.find(hash);
    if (it != storage.end()) {
      if (it->second.second != sorted_keys.begin()) {
        sorted_keys.erase(it->second.second);
        sorted_keys.emplace_front(hash);
        it->second.second = sorted_keys.begin();
        LRU_PRINT("get ok sorted replace hash=" << hash << "\n");
      } else {
        LRU_PRINT("get ok sorted alreadyfirst hash=" << hash << "\n");
      }

      return &it->second.first;
    }
    LRU_PRINT("get not found hash=" << hash << "\n");
    return nullptr;
  }

  bool remove(const key_type & key) {
    auto hash = calc_hash(key);
    auto it = storage.find(hash);
    if (it != storage.end()) {
      stored_size -= calc_stored_size( it );
      sorted_keys.erase(it->second.second);
      storage.erase(hash);
      LRU_PRINT("remove ok " << " stored_size=" << stored_size << " storage=" << storage.size() << " sorted_keys=" << sorted_keys.size()  << " hash=" << hash << "\n");
      return true;
    }
    LRU_PRINT("remove not found " << " stored_size=" << stored_size << " storage=" << storage.size() << " sorted_keys=" << sorted_keys.size() << " hash=" << hash << "\n");
    return false;
  }

};
