/*

astyle --style=attach -s2 --keep-one-line-blocks --keep-one-line-statements --pad-oper lru.hpp test.cpp
g++ -std=c++11 -lstdc++ test.cpp
g++ -std=c++11 -lstdc++ -DLRU_DEBUG=1 test.cpp
*/

#define LRU_DEBUG 1

#include <iostream>
#include <string>
//#include <thread>

#include "lru.hpp"

int main () {

  lru<int, int> l_int(2000);
  int i1 = 1, i2 = 2;
  l_int.insert(1, 2);
  auto gp = l_int.get(1);
  l_int.remove(1);
  l_int.insert(1, 2);
  l_int.insert(3, 4);



  for(int i = 10; i <= 5000; ++i) {
    int i3 = i * 3;
    l_int.insert(i, i * 3);
    int ig = 11;
    auto gp = l_int.get(ig);
    if (gp)
      std::cout << "get " << ig << " = " << *gp << "\n";
    ig = 15;
    gp = l_int.get(ig);
    if (gp)
      std::cout << "get " << ig << " = " << *gp << "\n";
  }

  // 20 must be cleared because not used
  int ign = 20;
  gp = l_int.get(ign);
  if (gp) {
    std::cout << "WRONG! get " << ign << " = " << *gp << "\n";
  } else {
    std::cout << "OK get " << ign << " = " << gp << "\n";
  }

  lru<double> l_double(30000);

  lru<> l1(10000);
  std::string key1("a");
  std::string val1(1000, 'b');
  l1.insert(key1, val1);


  key1 = std::string(100, 'a');
  val1 = "b"; // capacity still ~1000
  l1.insert(key1, val1);
  val1 = "bbbbbbbbbbb";
  val1.shrink_to_fit();
  l1.insert(key1, val1);
  std::string * tos = nullptr;

  auto gps = l1.get(key1);
  if (gps) {
    std::cout << "OK get replaced " << key1 << " = " << *gps << "\n";
  }
  val1 = std::string(500, 'c');

  l1.insert(key1, val1);
  l1.remove(key1);
  l1.remove(key1);
  l1.remove(key1);

  // bigger than storage, dont even try store
  val1 = std::string(500000, 'c');
  l1.insert(key1, val1);

  //exactly as free space
  lru<> l2(10000);
  val1 = std::string(10000 - sizeof(l2) - 104 - 8, 'c');
  l2.insert(key1, val1);
  val1 = std::string(1, 'c');
  val1.shrink_to_fit();
  l2.insert(key1, val1);

}
