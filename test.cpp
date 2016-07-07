/*

g++ -std=c++11 -lstdc++ test.cpp

*/

#include "lru.hpp"

int main () {

  lru<> l1(10000);

  lru<int> l_int(20000);

  lru<double> l_double(30000);

}
