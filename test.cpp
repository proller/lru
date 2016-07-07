/*

astyle --style=attach -s2 --keep-one-line-blocks --keep-one-line-statements --pad-oper lru.hpp test.cpp
g++ -std=c++14 -lstdc++ test.cpp

*/

#include "lru.hpp"

int main () {

  lru<> l1(10000);

  l1.insert("a", "b");

  lru<int> l_int(20000);

  lru<double> l_double(30000);

}
