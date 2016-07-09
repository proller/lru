/*

astyle --style=attach -s2 --keep-one-line-blocks --keep-one-line-statements --pad-oper lru.hpp test.cpp
g++ -std=c++11 -lstdc++ test.cpp
g++ -std=c++11 -lstdc++ -DLRU_DEBUG=1 test.cpp
*/

//#define LRU_DEBUG 1

#include <iostream>
//#include <thread>

#include "lru.hpp"

int main () {


//std::this_thread::sleep_for(std::chrono::seconds(100));
// mem here: 13312  1572

#if !LRU_DEBUG
  // real mem from top: 997504 987208
  lru<> lb1(1000000000);
  std::cout << "little datas\n";
  for(int i = 0; i <= 100000000; ++i) {
   std::string keyb(std::to_string(i));
   std::string valb(1, 'b');
   lb1.insert(keyb, valb);
  }
#endif



  lru<int, int> l_int(20000);
  int i1 = 1, i2 = 2;
  l_int.insert(i1, i2);
  auto gp = l_int.get(i1);
  l_int.remove(i1);

  // TODO:
  //l_int.insert(1, 2);
  //l_int.insert(3, 4);



  for(int i = 10; i <= 20000; ++i) {
    int i3 = i * 3;
    l_int.insert(i, i3);
    int ig = 11;
    auto gp =l_int.get(ig);
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

#if !LRU_DEBUG

  // real mem from top: 990044 979604
  lru<> lb(1000000000);
  std::cout << "big datas\n";
  for(int i = 0; i <= 100000; ++i) {
   std::string keyb(std::to_string(i));
   std::string valb(1000000, 'b');
   lb.insert(keyb, valb);
  }
  std::cout << "little datas\n";
  for(int i = 0; i <= 100000000; ++i) {
   std::string keyb(std::to_string(i));
   std::string valb(1, 'b');
   lb.insert(keyb, valb);
  }
#endif


}
