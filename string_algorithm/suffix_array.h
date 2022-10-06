#include <iostream>
#include <vector>
#include <cassert>
#include <unordered_map>
#include <map>


#ifndef __DST_ALGORITHM
#define __DST_ALGORITHM

namespace dst {
      template <typename T, size_t S>
      void stable_sort (std::vector<T>& vec, auto func) {

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
            std::vector<std::pair<size_t, size_t>> sort(N);

            for (size_t i = 0; i < N; ++i)
                  sort[i] = {i,0};

            dst::stable_sort<std::pair<size_t,size_t>, S>(sort,
                        [&str](const auto& p){return str[p.first];});

            for (size_t i = 0; i < log; ++i) {
                  for (size_t i = 0; i < N; ++i)
                        sort[i].second = (sort[i].first + (1ul << i)) % N;

                  dst::stable_sort<std::pair<size_t,size_t>, S>(sort,
                              [&str] (const auto& x) 
                              {return str[x.second];});

                  dst::stable_sort<std::pair<size_t,size_t>, S>(sort,
                              [&str, &sort] (const auto& x)
                              {return str[x.first];});
            }
            
            array.resize(N);
            for (size_t i = 0; i < N; ++i)
                  array[i] = sort[i].first;
      };
}

#endif
