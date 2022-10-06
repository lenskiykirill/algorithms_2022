#ifndef __DST_STRING_ALGORITHM
#define __DST_STRING_ALGORITHM

#define DEBUG

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cassert>

namespace dst {
      namespace __private { // namespace for default constants, functions and classes

            const size_t alpha_size = 'z' - 'a' + 3;

            struct basic_tr_function {
                  size_t operator() (char c) {
                        if (c == '#')
                              return 0;
                        if (c == '$')
                              return alpha_size - 1;

                        assert ((c >= 'a') && (c <= 'z'));
                        return c - 'a' + 1;
                  };
            };
      };

      template <typename T, class F = __private::basic_tr_function,
                size_t S = __private::alpha_size>
      class array_trie;

      template <typename T, class F, size_t S>
      std::ostream& operator<< (std::ostream&, const array_trie<T,F,S>&);

      template <typename T, class F, size_t S>
      class array_trie {
            public:
                  array_trie (F tr_func);
                  array_trie ();
                 ~array_trie ();

                  // class iterator;

                  void /*iterator*/ insert(const std::string& s, const T& value);
                  T& operator[] (const std::string& s);

                  void aho ();
                  size_t debug_get_link (size_t idx);
                  size_t debug_get_idx (const std::string& s);

                  void find (const std::string&, auto);

            private:
                  struct node {
                        size_t to[S];
                        size_t link;
                        size_t term_link; // link to closest terminal link
                        size_t value;

                        node () : value (size_t(-1)) {
                              for (size_t i = 0; i < S; ++i)
                                    to[i] = -1;
                        };
                  };

                  std::vector<node> nodes;
                  std::vector<T> terminal;

                  F tr_func;

            friend std::ostream& operator<< <T,F,S> (std::ostream&, const array_trie&);
      };

      template <typename T, class F, size_t S>
      array_trie<T,F,S>::array_trie (F tr_func)
                                    : nodes (1),
                                      terminal (),
                                      tr_func (tr_func) {}

      template <typename T, class F, size_t S>
      array_trie<T,F,S>::array_trie ()
                                    : nodes (1),
                                      terminal (),
                                      tr_func () {}

      template <typename T, class F, size_t S>
      array_trie<T,F,S>::~array_trie () {}

      template <typename T, class F, size_t S>
      void array_trie<T,F,S>::insert (const std::string& s, const T& value) {
            size_t idx = 0;

            for (auto c : s) {
                  size_t z = tr_func(c);

                  if (nodes[idx].to[z] == size_t(-1)) {
                        nodes[idx].to[z] = nodes.size();
                        nodes.emplace_back();
                  }

                  idx = nodes[idx].to[z];
            }

            node& term = nodes[idx];

            if (term.value == size_t(-1)) {
                  term.value = terminal.size();
                  terminal.emplace_back(value);
            } else {
                  terminal[term.value] = value;
            }
      }

      template <typename T, class F, size_t S>
      T& array_trie<T,F,S>::operator[] (const std::string& s) {
            size_t idx = 0;

            for (auto c : s) {
                  size_t z = tr_func(c);
                  assert (nodes[idx].to[z] != size_t(-1));

                  idx = nodes[idx].to[z];
            }
            
            return terminal[nodes[idx].value];
      }

      template <typename T, class F, size_t S> // trie structure is demolished by this operation
      void array_trie<T,F,S>::aho () {         // may be I would write a restoring function, but
            std::queue<size_t> pending;        // not now...
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
            nodes[0].term_link = 0;

            while (!pending.empty()) {
                  idx = pending.front();
                  pending.pop();

                  node& current = nodes[idx];

                  if (current.value == size_t(-1))
                        current.term_link = nodes[current.link].term_link;
                  else
                        current.term_link = idx;

                  for (size_t i = 0; i < S; ++i) {
                        size_t next = current.to[i];

                        if (next != size_t(-1)) {
                              pending.push(next);
                              nodes[next].link = nodes[current.link].to[i];
                              
                              continue;
                        }

                        current.to[i] = nodes[current.link].to[i];
                  }
            }
      }

      /*template <typename T, class F, size_t S>
      void array_trie<T,F,S>::find (const std::string& s, std::vector<int>& ans) {
            size_t idx = 0;

            for (auto c : s) {
                  idx = nodes[idx].to[tr_func(c)];
                  size_t t_idx = nodes[idx].term_link;

                  while (t_idx != 0) { // TODO: organise a normal fix
                        ans[nodes[t_idx].value] += 1;
                        t_idx = nodes[nodes[t_idx].link].term_link;
                  }
            }
      }*/

      template <typename T, class F, size_t S>
      void array_trie<T,F,S>::find (const std::string& s, auto func) {
            size_t idx = 0;

            for (size_t i = 0; i < s.size(); ++i) {
                  char c = s[i];

                  idx = nodes[idx].to[tr_func(c)];
                  size_t t_idx = nodes[idx].term_link;

                  while (t_idx != 0) {
                        func(nodes[t_idx].value, i);
                        t_idx = nodes[nodes[t_idx].link].term_link;
                  }
            }
      }
//                 ___________
//                |___DEBUG___|
#ifdef DEBUG
      template <typename T, class F, size_t S>
      size_t array_trie<T,F,S>::debug_get_link (size_t idx) {
            return nodes[idx].link;
      }

      template <typename T, class F, size_t S>
      size_t array_trie<T,F,S>::debug_get_idx (const std::string& s) {
            size_t idx = 0;

            for (auto c : s) {
                  size_t z = tr_func(c);
                  idx = nodes[idx].to[z];
            }

            return idx;
      }
#endif
}

#endif
