/*

astyle --style=attach -s2 --keep-one-line-blocks --keep-one-line-statements --pad-oper lru.hpp test.cpp
g++ -std=c++14 -lstdc++ test.cpp

*/

#define LRU_DEBUG 1
#include "lru.hpp"

int main () {

  lru<> l1(10000);


  lru<int, int> l_int(20000);
  int i1 = 1, i2 = 2;
  l_int.insert(i1, i2);
  int * to = nullptr;
  l_int.get(i1, &to);
  l_int.remove(i1);

  for(int i = 10; i <= 20000; ++i) {
    int i3 = i * 3;
    l_int.insert(i, i3);
  }

  lru<double> l_double(30000);

  std::string key1("a");
  std::string val1("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
  l1.insert(key1, val1);


  key1="aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  val1="b";
  l1.insert(key1, val1);

}
