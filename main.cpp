#include "s_alg.h"

int main() {
      dst::__basic_array_trie<int> T;
      T.insert (std::string("hello"), 1);
      T.insert (std::string("hell"), 1);
      std::cout << T["hell"];
}
