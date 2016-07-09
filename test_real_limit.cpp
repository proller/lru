#include <iostream>
#include <string>
#include <thread>

#include "lru.hpp"

int main () {

  std::cout << "sleep 10: now look at clean process mem usage\n";
  std::this_thread::sleep_for(std::chrono::seconds(10));
  // mem here: 13312  1572

  lru<> lb1(1000000000);
  std::cout << "filling with little datas. you can run ` watch 'ps auxw | grep test2' ` \n";
  for(int i = 0; i <= 100000000; ++i) {
    std::string keyb(std::to_string(i));
    std::string valb(1, 'b');
    lb1.insert(keyb, valb);
  }
  // mem here: 997504 987208

}
