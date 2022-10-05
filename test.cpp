#include "s_alg.h"
#include <map>
#include <cstdlib>
#include <ctime>

#define N 1000000
#define L 10

int main () {
      
      srand(time(NULL));
      
      std::map<std::string, int> M;

      dst::__basic_array_trie<int> T;
      for (int i = 0; i < N; ++i) {
            size_t length = (rand() % L) + 1;
            std::string s;

            for (size_t j = 0; j < length; ++j) {
                  s += ('a' + rand()%26);
            }

            std::cout << "insert " << s << ' ' << i << std::endl;

            M[s] = i;
            T.insert (s, i);
      }


      for (int i = 0; i < N; ++i) {
            size_t length = (rand() % L) + 1;
            std::string s;

            for (size_t j = 0; j < length; ++j) {
                  s += ('a' + rand()%26);
            }

            if (M.find(s) != M.end()) {
                  std::cout << "check " << s << std::endl;
                  assert (M[s] == T[s]);
            }
      }

      std::cout << "ALL TESTS PASSED!!!\n";

}
