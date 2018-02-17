
// Generic LRU cache implementation

#include <iostream>
#include <unordered_map>
#include <list>
#include <assert.h>

using namespace std;

// Key and Value types to be c++ literals 
template<typename K, typename V>
class LRUCache {
  private:
    int LRUSize_;
    unordered_map<K, typename list<pair<K,V>>::iterator> LRUHash_;
    list<pair<K,V>> LRUList_;
    void RemoveLRUEntry();
    int numElems_;

  public:
    LRUCache(int size):LRUSize_(size),numElems_(0) {
    }
    void CacheInsert(const K &key, const V &value);
    bool CacheLookup(const K &key, V &value);
};

template<typename K, typename V>
void 
LRUCache<K,V>::RemoveLRUEntry() {
  auto lastIterator = LRUList_.end();
  lastIterator--;
  K key = lastIterator->first;
  LRUHash_.erase(lastIterator->first);
  LRUList_.erase(lastIterator);
  cout << "\nKey:   " << key << "Removed";
  numElems_--;
}

template<typename K, typename V>
void 
LRUCache<K,V>::CacheInsert(const K &key, const V &value)
{

  cout << "\nInserting Key: " << key << "  Value: " << value;

  /* Find the element if present in hashtable */
	auto it = LRUHash_.find(key);
	if (it != LRUHash_.end()) {
    auto listIterator = LRUHash_[key];
    K hashKey = (*listIterator).first;
    assert(hashKey == key);
    get<1>(*listIterator) = value;
    LRUList_.splice(LRUList_.begin(), LRUList_, listIterator);
    return;
  }

  // Check if LRUFull
  if (numElems_ >= LRUSize_) {
    RemoveLRUEntry();
  }

  // If element is absent
  LRUList_.push_front({key,value});
  auto iter = LRUList_.begin();
  LRUHash_[key] = iter;
  numElems_++;
  return;
}


template<typename K, typename V>
bool
LRUCache<K,V>::CacheLookup(const K &key, V &val)
{
  if (LRUHash_.find(key) == LRUHash_.end()) {
    return false;
  }

  auto listIterator = LRUHash_[key];
  val = (*listIterator).second;
  
  if (listIterator == LRUList_.begin())
    return true;

  LRUList_.splice(LRUList_.begin(), LRUList_, listIterator);
  return true;
}

void Lookup(auto &cache, int &key, string &value) {
  if (cache.CacheLookup(key, value))
  {
    cout << "\nLookup Success Key:  " << key << "   Value:   " << value;
  } else {
    cout << "\nKey:   " << key << "Not Found";
  }
}


const int cacheSize = 5;
int main()
{
	LRUCache<int, string> cache(cacheSize);
  
  cache.CacheInsert(1, "india");
  cache.CacheInsert(2, "us");
  cache.CacheInsert(3, "america");
  cache.CacheInsert(4, "africa");
  cache.CacheInsert(1, "china");
  cache.CacheInsert(5, "pak");
  cache.CacheInsert(2, "srilanka");

  string value;
  int key;

  key = 3; 
  Lookup(cache, key, value);
  
  cache.CacheInsert(6, "ratlam");
  cache.CacheInsert(7, "hyderabad");

  key = 4; 
  Lookup(cache, key, value);
  cache.CacheInsert(8, "bangalore");
  
  key = 4; 
  Lookup(cache, key, value);

  key = 2;
  Lookup(cache, key, value);
  
  key = 6;
  Lookup(cache, key, value);

  cache.CacheInsert(6, "bhopal");
  
  key = 2;
  Lookup(cache, key, value);
  
  cout << endl;

}
