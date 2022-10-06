#include <iostream>
#include <vector>
#include <cassert>
#include <unordered_map>
#include <map>


#ifndef __DST_ALGORITHM
#define __DST_ALGORITHM

namespace dst {
      template <typename T>
      void stable_sort (size_t S, std::vector<T>& vec, auto func) {

            size_t N = vec.size();

            std::vector<T> A = std::move(vec);
            std::vector<size_t> cnt(S+1, 0);

            for (const auto& e : A)
                  cnt[func(e)+1] += 1;

            for (size_t i = 1; i < S; ++i)
                  cnt[i] += cnt[i-1];
            
            vec.resize(N);

            for (size_t i = 0; i < N; ++i) {
                  auto z = func(A[i]);
                  vec[cnt[z]++] = std::move(A[i]);
            }
      }
}

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

#endif


#ifndef __DST_SUFFIX_ARRAY
#define __DST_SUFFIX_ARRAY

namespace dst {
      // 
      //

      namespace __private {
            const size_t alpha_size = 'z' - 'a' + 3;

            template <size_t S>
            struct basic_tr_function;

            template<>
            struct basic_tr_function<alpha_size> {
                  size_t operator() (char c) {
                        if (c == '#')
                              return 0;
                        if (c == '$')
                              return alpha_size - 1;

                        assert ((c >= 'a') && (c <= 'z'));
                        return c - 'a' + 1;
                  };
            };
      }

      template <size_t S = __private::alpha_size,
                class F = __private::basic_tr_function<S>>
      void suffix_array (const std::string& s, std::vector<size_t>& array, F tr_func = F()) {

            std::vector<size_t> str (s.size()+1);
            for (size_t i = 0; i < s.size(); ++i)
                  str[i] = tr_func(s[i]);
            str[s.size()] = 0;

            size_t log = 0;
            for (; 1ul<<log < s.size(); ++log) {}

            size_t N = str.size();

            std::vector<size_t> weight (N);
            std::vector<std::pair<size_t,size_t>> W (N);

            for (size_t i = 0; i < N; ++i) {
                  W[i].first = str[i];
                  W[i].second = 0;
            }

            weight_sort(S, W, weight);

            for (size_t i = 0; i < log; ++i) {
                  for (size_t j = 0; j < N; ++j) {
                        W[j].first = weight[j];
                        W[j].second = weight[(j + (1ul << i)) % N];
                  }

                  weight_sort(N, W, weight);
            }
            
            array.resize(N);
            for (size_t i = 0; i < N; ++i)
                  array[weight[i]] = i;
      };
}

#endif

int main () {

      std::string s;
      std::cin >> s;
      std::vector<size_t> v;

      dst::suffix_array(s,v);

      for (size_t e = 1; e < v.size(); ++e)
            std::cout << v[e]+1 << ' ';
      std::cout << std::endl;
}
