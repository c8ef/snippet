#include <iostream>
#include <list>
#include <unordered_map>

using namespace std;

class LRU {
 private:
  list<pair<int, int>> cache_;
  unordered_map<int, list<pair<int, int>>::iterator> map_;
  int capacity_;

 public:
  LRU(int capacity) : capacity_(capacity) {}

  int get(int key) {
    auto iter = map_.find(key);
    if (iter == map_.end())
      return -1;
    else {
      int value = iter->second->second;
      cache_.push_front(*(iter->second));
      cache_.erase(iter->second);
      map_[key] = cache_.begin();
      return value;
    }
  }

  void put(int key, int value) {
    auto iter = map_.find(key);
    if (iter == map_.end()) {
      if (cache_.size() == capacity_) {
        auto lst = cache_.back();
        map_.erase(lst.first);
        cache_.pop_back();
      }
      cache_.push_front({key, value});
      map_[key] = cache_.begin();
    } else {
      cache_.erase(iter->second);
      cache_.push_front({key, value});
      map_[key] = cache_.begin();
    }
  }
};

int main() {
  LRU cache{2};
  cache.put(1, 1);
  cache.put(2, 2);
  cout << cache.get(1) << " ";
  cache.put(3, 3);
  cout << cache.get(2) << " ";
  cache.put(4, 4);
  cout << cache.get(1) << " ";
  cout << cache.get(3) << " ";
  cout << cache.get(4) << endl;
  return 0;
}