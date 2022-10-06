#include "string_algorithm.h"

#include <map>
#include <cstdlib>
#include <ctime>

void dumb_checker (const std::string& s, const std::vector<std::string>& v) {
      
      std::vector<std::vector<int>> res(v.size());

      int i = 0;
      for (const auto& s1 : v) {
            size_t idx = s.find(s1);
            while (idx != std::string::npos) {
                  idx = s.find(s1,idx+1);
                  res[i].push_back(idx+1);
            }
            ++i;
      }
}

int main () {
      std::string s;
      std::cin >> s;

      size_t N;
      std::cin >> N;

      std::vector<string> S(N);

      std::vector<int> length;
      std::vector<std::vector<int>> ans;

      dst::array_trie<int> T;
      for (size_t i = 0; i < N; ++i) {
            std::cin >> S[i];
            T.insert(S[i],i);

            length.push_back(S[i].size());
            ans.push_back(std::vector<int>());
      }

      T.aho();
      T.find(s, [&length, &ans](size_t n, size_t idx) -> void {
                        ans[n].push_back(idx-length[n]+2);
                        return;
                  });

      auto cmp = dumb_checker (s, S);
      assert (cmp.size() == ans.size());

      for (auto i = 0; i < ans.size(); ++i) {
            assert (cmp[i].size() == ans[i].size());

            for (auto j = 0; j < ans[i].size(); ++j)
                  assert (cmp[i][j] == ans[i][j]);
      }
}
