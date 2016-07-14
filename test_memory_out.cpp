#include <iostream>
#include <string>

#define LRU_DEBUG 1
#include "lru.hpp"

int main () {

  lru<> lb1(1000000000000);
  std::cout << "filling with big datas until exeption or oom. DISABLE SWAP BEFORE RUN. \n";
  std::string valb;
  valb = std::string(1000000000, 'b');
  for(int i = 0; i <= 1000; ++i) {
    std::string keyb(std::to_string(i));
    auto r = lb1.insert(std::to_string(i), valb);
    std::cout << keyb << " r=" << r << " \n";
    if (!r)
      break;
  }

}
