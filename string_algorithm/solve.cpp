#include <iostream>
#include <vector>
#include <cassert>

namespace dst {
      void weight_sort (size_t S, const std::vector<std::pair<size_t,size_t>>& W,
                        std::vector<size_t>& w) {

            size_t N = W.size();

            std::vector<size_t> t(N);
            std::vector<size_t> cnt(S+1);

            for (size_t i = 0; i < N; ++i) {
                  size_t z = W[i].second;
                  cnt[z+1]++;
            }

            for (size_t i = 1; i < S; ++i)
                  cnt[i] += cnt[i-1];

            for (size_t i = 0; i < N; ++i) {
                  size_t z = W[i].second;
                  t[cnt[z]++] = i;
            }

            std::vector<size_t> t1 (N);
            for (auto& e : cnt)
                  e = 0;

            for (size_t i = 0; i < N; ++i) {
                  size_t z = W[t[i]].first;
                  cnt[z+1]++;
            }

            for (size_t i = 1; i < S; ++i)
                  cnt[i] += cnt[i-1];

            for (size_t i = 0; i < N; ++i) {
                  size_t z = W[t[i]].first;
                  t1[cnt[z]++] = t[i];
            }

            w.resize(N);
            w[t1[0]] = 0;

            for (size_t i = 1; i < N; ++i) {
                  if (W[t1[i]].first > W[t1[i-1]].first)
                        w[t1[i]] = w[t1[i-1]]+1;
                  else {
                        if (W[t1[i]].second > W[t1[i-1]].second)
                              w[t1[i]] = w[t1[i-1]]+1;
                        else
                              w[t1[i]] = w[t1[i-1]];
                  }
            }
      }
}

int main () {
      std::vector<std::pair<size_t,size_t>> v = {{3, 2}, {1, 5}, {3,1}};
      std::vector<size_t> w;

      dst::weight_sort(6, v, w);

      for (auto e : w)
            std::cout << e << ' ';
      std::cout << std::endl;
}
