
template <typename Key_type, typename Value_type>
class lru {
public:

  lru(size_t limit_);

  bool insert(const Key_type & key, const Value_type &value);

  bool get (const Key_type & key, Value_type ** to) const;

  bool remove (const Key_type & key);

private:
  size_t calc_size(const Key_type & key, const Value_type &value);

  typename size_t Key_store_type;
  typename std::list<Key_store_type>::iterator list_iter;
  std::map<Key_store_type, std::pair<Value_type, list_iter> > storage;
  std::list<Key_store_type> sorted_keys;

  const size_t limit;
  const size_t map_node_size;
  const size_t list_node_size;
  size_t size = 0;
};

template <typename Type>
size_t calc_size(const Type & key) {
  return sizeof(key);
}

template <>
size_t calc_size<std::string>(const std::string & key) {
  return sizeof(key) + key.capacity();
}



template <typename Key_type, typename Value_type>
lru::lru(size_t limit_) :
  map_node_size(sizeof(void*) * 3),
  list_node_size(sizeof(void*) * 3),
  limit(limit_) {
  size += sizeof(*this);
}

template <typename Key_type, typename Value_type>
bool lru::insert(const Key_type & key, const Value_type &value) {

Key_store_type hash = std::: hash(key);
  auto it = storage.find(hash);
  size_t current_size = 0;
  if (it != storage.end()) {
    current_size = sizeof(it->first) + sizeof(it->second.first) + calc_size(it->second.second) + map_node_size + list_node_size + sizeof(Key_store_type);
  }

  size_t want_size = calc_size(key) + calc_size(value) + map_node_size + ... ;

  if (current_size < want_size && (size - current_size) + want_size > limit) {
    //clean
    for (auto sit = sorted_keys.rbegin(); sit != sorted_keys.rend(); sit = sorted_keys.rbegin())  {
      // cur check
      if (hash == *sit) {
        // dont!
      }
      size -= calc_elem_size( sorted_keys.rbegin() );
      storage.erase( *sit );
      sorted_keys.erase(sit);
      if ((size - current_size) + want_size < limit) {

      }
    }
  }

  if (current_size) {
    it = storage.find(hash);
    sorted_keys.erase(it->second.second);
    sorted_keys.emplace_front(hash);
    storage[hash] = std::make_pair<Value_type, list_iter>(value, sorted_keys.begin());
    size -= current_size;
  } else {
    sorted_keys.emplace_front(hash);
    auto pr = storage.emplace(hash, std::make_pair<Value_type, list_iter>(value, sorted_keys.begin()));
    it = pr.second; //?
  }
  //size += calc_size(it->first) + calc_size(it->second) + map_node_size;
  auto new_size = sizeof(it->first) + sizeof(it->second.first) + calc_size(it->second.second) + map_node_size + list_node_size + sizeof(Key_store_type);
  size += new_size;
}



template <typename Key_type, typename Value_type>
bool lru::get (const Key_type & key, Value_type ** to) const {
  auto it = storage.find(key);
  if (it != storage.end()) {
    if (!to)
      return false;
    *to = &it->second;
    return true;
  }
  return false;
}
}

template <typename Key_type, typename Value_type>
bool lru::remove (const Key_type & key) {
  return storage.erase(key);
}

