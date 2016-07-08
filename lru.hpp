#include <cstddef>
#include <string>
#include <list>
#include <map>

template <typename Type>
size_t calc_size(const Type & key) {
  return sizeof(key);
}

template <>
size_t calc_size<std::string>(const std::string & key) {
  return sizeof(key) + key.capacity();
}

template <class Key_type = std::string, class Value_type = std::string>
class lru {
public:

  lru(size_t limit_);

  bool insert(Key_type & key, Value_type &value);

  bool get (Key_type & key, Value_type ** to);

  bool remove (Key_type & key);

private:
  typedef size_t Key_store_type;
  typedef std::list<Key_store_type>::iterator list_iter;
  typedef typename std::map<Key_store_type, std::pair<Value_type, list_iter> >::iterator storage_iter;

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
    //return std::hash<Key_type>(key);
    std::hash<Key_type> fn;
    return fn(key);
    
    //return std::hash<Key_type>(const_cast<Key_type&>(key));
    //return std::hash<>(key);
  };

  inline Key_store_type calc_elem_size(Key_store_type hash) {
    auto it = storage.find(hash);
    if (it == storage.end())
      return 0;
    return calc_stored_size(it);
  };

};




template <typename Key_type, typename Value_type>
lru<Key_type, Value_type>::lru(size_t limit_) :
  map_node_size(sizeof(void*) * 3),
  list_node_size(sizeof(void*) * 3),
  limit(limit_) {
  size += sizeof(*this);
}

template <typename Key_type, typename Value_type>
bool lru<Key_type, Value_type>::insert(Key_type & key, Value_type &value) {

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
    for (auto sit = sorted_keys.rbegin(); sit != sorted_keys.rend(); sit = sorted_keys.rbegin())  {
      // cur check
      if (hash == *sit) {
        // dont! TODO
      }
      auto storage_it = storage.find(*sit);
      size -= calc_stored_size( storage_it );
      storage.erase( storage_it );
      sorted_keys.erase(sit.base());
      if ((size - current_size) + want_size < limit) {

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
  return true;
}



template <typename Key_type, typename Value_type>
bool lru<Key_type, Value_type>::get (Key_type & key, Value_type ** to) {
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

template <typename Key_type, typename Value_type>
bool lru<Key_type, Value_type>::remove (Key_type & key) {
  return storage.erase(calc_hash(key));
}

