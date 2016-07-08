#include <cstddef>
#include <string>
#include <list>
#include <map>

#if defined(LRU_DEBUG)
#include <iostream>
#define LRU_PRINT(a) std::cout << a;
#else
#define LRU_PRINT(a) ;
#endif

template <typename Type>
size_t calc_size(const Type & key) {
  return sizeof(key);
}

template <>
size_t calc_size<std::string>(const std::string & key) {
  return sizeof(key) + key.capacity();
}

template <class Key_type = std::string, class Value_type = std::string, class hash_func = std::hash<Key_type>>
class lru {
public:

  lru(size_t limit_) :
    map_node_size(sizeof(void*) * 3),
    list_node_size(sizeof(void*) * 3),
    limit(limit_) {
    size += sizeof(*this);

    LRU_PRINT("initial size=" << size << "\n");
  }


  bool insert(Key_type & key, Value_type &value) {

    auto hash = calc_hash(key);
    auto it = storage.find(hash);
    size_t current_size = 0;
    if (it != storage.end()) {
      current_size = calc_stored_size(it);
    }

    size_t want_size = calc_size_pair(key, value);

    //need clean cache?
    if (current_size < want_size && (size - current_size) + want_size > limit) {
      //clean
      for (auto sit = sorted_keys.rbegin(); sit != sorted_keys.rend(); ++sit)  {
        if (hash == *sit) {
          continue;
        }
        auto storage_it = storage.find(*sit);
        size -= calc_stored_size( storage_it );
        storage.erase( storage_it );
        sorted_keys.erase(storage_it->second.second);
        LRU_PRINT("cleaning " << calc_stored_size( storage_it ) << " want_size=" << want_size << " size=" << size << " storage=" << storage.size() << " sorted_keys=" << sorted_keys.size() << "\n");

        if ((size - current_size) + want_size < limit) {
          break;
        }
      }
    }

    if (current_size) {
      it = storage.find(hash);
      sorted_keys.erase(it->second.second);
      sorted_keys.emplace_front(hash);
      //storage[hash] = std::make_pair(value, sorted_keys.begin());
      //del!!
      storage.erase( it );
      auto pr = storage.emplace(hash, std::make_pair(value, sorted_keys.begin()));
      size -= current_size;
    } else {
      sorted_keys.emplace_front(hash);
      auto pr = storage.emplace(hash, std::make_pair(value, sorted_keys.begin()));
      it = pr.first; //?
    }
    //size += calc_size(it->first) + calc_size(it->second) + map_node_size;
    auto new_size = calc_stored_size(it);
    size += new_size;

    LRU_PRINT("inserted new_size=" << new_size << " want_size=" << want_size << " size=" << size << " storage=" << storage.size() << " sorted_keys=" << sorted_keys.size() << "\n");

    return true;
  }


  bool get (Key_type & key, Value_type ** to) {
    auto hash = calc_hash(key);
    auto it = storage.find(hash);
    if (it != storage.end()) {
      if (!to)
        return false;

      sorted_keys.erase(it->second.second);
      sorted_keys.emplace_front(hash);

      *to = &it->second.first;
      return true;
    }
    return false;
  }


  bool remove (Key_type & key) {

    auto hash = calc_hash(key);
    auto it = storage.find(hash);
    if (it != storage.end()) {
      sorted_keys.erase(it->second.second);
      storage.erase(hash);
      size -= calc_stored_size( it );
      LRU_PRINT("erasing " << " size=" << size << " storage=" << storage.size() << " sorted_keys=" << sorted_keys.size() << "\n");
      return true;
    }
    return false;
  }

private:
  using Key_store_type = size_t;
  using list_iter = std::list<Key_store_type>::iterator;
  using storage_iter = typename std::map<Key_store_type, std::pair<Value_type, list_iter> >::iterator;

  std::map<Key_store_type, std::pair<Value_type, list_iter> > storage;
  std::list<Key_store_type> sorted_keys;

  const size_t limit;
  const size_t map_node_size;
  const size_t list_node_size;
  size_t size = 0;

  inline size_t calc_size_pair(const Key_type & key, const Value_type &value) const {
    return sizeof(Key_store_type) + calc_size<>(value) + map_node_size + list_node_size;
  };

  inline size_t calc_stored_size(const storage_iter & it) const {
    return sizeof(it->first) + sizeof(it->second.first) + calc_size<>(it->second.second) + map_node_size + list_node_size + sizeof(Key_store_type);
  };

  //inline Key_store_type calc_hash(const Key_type &key) const {
  inline Key_store_type calc_hash(Key_type &key) const {
    hash_func fn;
    return fn(key);
  };

  inline Key_store_type calc_elem_size(Key_store_type hash) {
    auto it = storage.find(hash);
    if (it == storage.end())
      return 0;
    return calc_stored_size(it);
  };

};
