#include <iostream>
#include <vector>
#include <cassert>
#include <bits/stdc++.h>

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

namespace dst {
      namespace __private {
            const size_t alpha = 'z' - 'a' + 3;

            template <size_t>
            struct tr_func;

            template<>
            struct tr_func<alpha> {
                  size_t operator() (char c) {
                        if (c == '#')
                              return 0;
                        if (c == '$')
                              return alpha-1;
                        assert ((c >= 'a') && (c <= 'z'));
                        return c - 'a' + 1;
                  }
            };
      }

      template <size_t S = __private::alpha, class F = __private::tr_func<S>>
      void suffix_array (const std::string& s, std::vector<size_t>& array, F tr_func = F()) {
            
            std::vector<size_t> str (s.size() + 1);
            for (size_t i = 0; i < s.size(); ++i)
                  str[i] = tr_func(s[i]);
            str[s.size()] = 0;

            size_t N = str.size();
            size_t log = 0;
            for (; (1ul<<log) < N; ++log) {}

            std::vector<size_t> weight (N);
            std::vector<std::pair<size_t,size_t>> W(N);

            for (size_t i = 0; i < N; ++i) {
                  W[i].first = str[i];
                  W[i].second = 0;
            }

            weight_sort(S, W, weight);

            for (size_t i = 0; i < log; ++i) {
                  for (size_t j = 0; j < N; ++j) {
                        W[j].first = weight[j];
                        W[j].second = weight[(j + (1ull << i)) % N];
                  }

                  weight_sort(N, W, weight);
            }

            array.resize(N-1);
            for (size_t i = 0; i < N-1; ++i)
                  array[weight[i]-1] = i;
      }

      template <size_t S = __private::alpha, class F = __private::tr_func<S>>
      void LCP (const std::string& s, const std::vector<size_t>& p,
                std::vector<size_t>& L, F tr_func = F()) {
            size_t N = p.size();
            std::vector<size_t> pos (N);

            for (size_t i = 0; i < N; ++i) {
                  pos[p[i]] = i;
            } // pos[k] = number of k-th suffix in array

            L.resize(N-1);
            size_t k = 0;

            for (size_t i = 0; i < N; ++i) {
                  k = std::max(k, 1) - 1;

                  if (pos[i] == N-1)
                        continue;

                  size_t j = p[pos[i]+1];
                  while ((std::max(i,j) + k < N) && (s[i+k] == s[j+k])) ++k;
                  L[pos[i]] = k;
            }
      }
}

namespace dst {
      template <class F>
      class sparse_table {
            public:
                  sparse_table (const std::vector<size_t>& vals, F cmp);
                  size_t get (size_t begin, size_t end);
            private:
                  size_t N;
                  std::vector<std::vector<size_t>> table;
                  F cmp;
      };
      
      template <class F>
      sparse_table<F>::sparse_table (const std::vector<size_t>& vals, F cmp) : cmp(cmp) {
            N = vals.size();
            size_t log = 0;
            for (; (1ul << log) < N; ++log) {}

            table.resize(log);
            table[0].resize(N);

            for (size_t j = 0; j < N; ++j)
                  table[0][j] = vals[j];

            for (size_t l = 1; l < log; ++l) {
                  size_t p = (1 << l) - 1;
                  table[i].resize(N-p);

                  for (size_t j = 0; j < N-p; ++j) {
                        table[l][j] = this->cmp(table[l-1][j], table[l-1][j+p]);
                  }
            }
      }

      template <class F>
      size_t sparse_table<F>::get (size_t begin, size_t end) {
            size_t l = end - begin;
            size_t log = 0;

            for (; (1ul << log) < l; ++log) {}
            --log;

            assert ((1ul<<(log+1)) <= l);
            size_t p = (1ull << log);

            return cmp(table[log][begin], table[log][end-p]);
      }
}

int main () {
      std::string s;
      
      std::ios_base::sync_with_stdio(false);
      std::cin.tie(NULL);

      std::cin >> s;

      std::vector<size_t> v;
      dst::suffix_array(s,v);

      std::vector<size_t> lcp;
      dst::LCP (s,v,lcp);

      auto min = [](size_t a, size_t b)->size_t {return std::min(a,b)};
      auto max = [](size_t a, size_t b)->size_t {return std::max(a,b)};

      // now it is de time to launch. And for Italian politician it is absolutely crucial to eat
      // de lunche. And now it iz de time of LUNCHE!!!
      
      dst::sparse_table<decltype(min)> min_tab (v, min);
      dst::sparse_table<decltype(max)> man_tab (v, max);

      std::vector<size_t> stack (1, 0);
      size_t L = lcp.size();

      std::vector<size_t> left_boundary (L);
      std::vector<size_t> right_boundary (L);

      for (size_t i = 0; i < L; ++i) {
            for (;stack.back() > lcp[i]; stack.pop_back()) {}
      }
}

