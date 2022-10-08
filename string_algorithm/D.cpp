#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cassert>

namespace dst {

      namespace __private {
            const size_t alpha = 'z' - 'a' + 1;

            template <size_t S>
            struct tr_func;

            template<>
            struct tr_func<alpha> {
                  size_t operator() (char c) {
                        return c-'a';
                  }
            };
      };

      template <size_t S = __private::alpha, class F = __private::tr_func<S>>
      class trie {
            public:
                  trie ();
                 ~trie () = default;
                  
                  void insert (const std::string& s);
                  void aho ();

                  size_t size();
                  void fill_matrix (std::vector<std::vector<size_t>>& m);
                  
            private:
                  struct node {
                        size_t to[S];
                        size_t link;
                        bool is_terminal = false; // maybe I don't need this, but who cares
                        size_t term_value = 0;

                        node () {
                              for (size_t i = 0; i < S; ++i)
                                    to[i] = size_t(-1);
                        }
                  };

                  std::vector<node> nodes;
                  F tr_func;
      };

      template <size_t S, class F>
      trie<S,F>::trie() : nodes (1), tr_func() {}

      template <size_t S, class F>
      void trie<S,F>::insert (const std::string& s) {
            size_t idx = 0;

            for (auto c : s) {
                  size_t z = tr_func(c);

                  if (nodes[idx].to[z] == size_t(-1)) {
                        nodes[idx].to[z] = nodes.size();
                        nodes.emplace_back();
                  }

                  idx = nodes[idx].to[z];
            }

            nodes[idx].is_terminal = true;
      }

      template <size_t S, class F>
      void trie<S,F>::aho () {
            std::queue<size_t> pending;
            size_t idx = 0;

            for (size_t i = 0; i < S; ++i) {
                  node& current = nodes[0];
                  size_t& next = current.to[i];

                  if (next == size_t(-1))
                        current.to[i] = 0;
                  else {
                        nodes[next].link = 0;
                        pending.push(next);
                  }
            }

            nodes[0].link = 0;
            nodes[0].term_value = 0;

            while (!pending.empty()) {
                  idx = pending.front();
                  pending.pop();

                  node& current = nodes[idx];
                  current.term_value = nodes[current.link].term_value;

                  if (current.is_terminal)
                        ++current.term_value;

                  for (size_t i = 0; i < S; ++i) {
                        size_t& next = current.to[i];

                        if (next != size_t(-1)) {
                              pending.push(next);
                              nodes[next].link = nodes[current.link].to[i];
                              continue;
                        }

                        current.to[i] = nodes[current.link].to[i];
                  }
            }
      }

      template <size_t S, class F>
      size_t trie<S,F>::size() {return nodes.size();}

      template <size_t S, class F>
      void trie<S,F>::fill_matrix (std::vector<std::vector<size_t>>& m) {
            size_t N = nodes.size();

            assert (m.size() == N);
            for (const auto& v : m)
                  assert (v.size() == N);

            for (size_t i = 0; i < N; ++i) {

                  for (size_t j = 0; j < N; ++j) {
                        m[i][j] = 0;
                  }

                  for (size_t c = 0; c < S; ++c) {
                        uint32_t idx = nodes[i].to[c];
                        if (nodes[idx].term_value == 0)
                              m[i][idx] += 1;
                  }
            }
      }
}

/*  ___
 * |   |
 *  abcde
 *   |__|
 *
 *
 * Автомат на k состояниях. Читает очередной символ. term_link[v] -- число гадости,
 * которая могла родиться на этом шаге. Нам же нужно, чтобы гадости не родилось вовсе.
 * Пусть dp[pos][j] -- число способов попасть в pos на j-м шаге, не прочитав гадких слов.
 * Тогда сколько у нас есть способов попасть в $k$ вершину? Ну, давайте пробежимся по всем
 * to[k] и ссылкам из нее. За квадрат построим матрицу. Если term_value > 0, ставим 0. Если из k
 * туда не попасть, ставим 0. Иначе -- единичку. Ну и херачим умножение матриц (быстрое). Короче,
 * туда-сюда обратно -- опа, и задача решилась. Теперь осталось тупо переписать код C с модификациями
 * и дело в шляпе. (Шлепе, большом русском коте)
 */

void multiply (const std::vector<std::vector<size_t>>& A,
               const std::vector<std::vector<size_t>>& B,
               std::vector<std::vector<size_t>>& C) {
      size_t N = A.size();

      for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j) {
                  size_t c = 0;
                  for (size_t k = 0; k < N; ++k) {
                        c += A[i][k]*B[k][j];
                        c %= 10000;
                  }

                  C[i][j] = c;
            }
}

int main () {
      size_t N, M;
      std::cin >> M >> N;

      dst::trie T;
      std::string s;

      for (size_t i = 0; i < N; ++i) {
            std::cin >> s;
            T.insert(s);
      }

      T.aho();
      N = T.size();

      std::vector<std::vector<size_t>> m (N, std::vector<size_t>(N));
      T.fill_matrix (m);

      decltype(m) m1 = m, m2 = m, tmp = m;

      size_t alpha = 26;
      size_t all = 26;

      for (size_t i = 0; i < M-1; ++i) {
            multiply (m1, m, tmp);
            all = (all*alpha) % 10000;
            m1.swap(tmp);
      }

      size_t res = 0;

      for (size_t i = 0; i < N; ++i)
            res += m1[0][i];

      res %= 10000;
      res = (all + 10000 - res);
      res %= 10000;

      std::cout << res << std::endl;
}
