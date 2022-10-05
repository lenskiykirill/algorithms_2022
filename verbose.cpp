#include <iostream>
#include "s_alg.h"

int main() {
      std::string s;
      int x;
      dst::__basic_array_trie<int> T;

      while (true) {
            std::cin >> s;
            if (s == "quit")
                  break;

            if (s == "insert") {
                  std::cin >> s >> x;
                  T.insert (s, x);
                  continue;
            }

            if (s == "get") {
                  std::cin >> s;
                  std::cout << T[s];
                  continue;
            }
      }
}
