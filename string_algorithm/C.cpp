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

            const uint32_t alpha_size = 'z' - 'a' + 3;

            struct basic_tr_function {
                  uint32_t operator() (char c) {
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
                uint32_t S = __private::alpha_size>
      class array_trie;

      template <typename T, class F, uint32_t S>
      std::ostream& operator<< (std::ostream&, const array_trie<T,F,S>&);

      template <typename T, class F, uint32_t S>
      class array_trie {
            public:
                  array_trie (F tr_func);
                  array_trie ();
                 ~array_trie ();

                  // class iterator;

                  void /*iterator*/ insert(const std::string& s, const T& value);
                  T& operator[] (const std::string& s);

                  void aho ();
                  uint32_t debug_get_link (uint32_t idx);
                  uint32_t debug_get_idx (const std::string& s);

                  void find (const std::string&, auto);

            private:
                  struct node {
                        uint32_t to[S];
                        uint32_t link;
                        uint32_t term_link; // link to closest terminal link
                        uint32_t value;

                        node () : value (uint32_t(-1)) {
                              for (uint32_t i = 0; i < S; ++i)
                                    to[i] = -1;
                        };
                  };

                  std::vector<node> nodes;
                  std::vector<std::pair<uint32_t,T>> terminal;
                  std::vector<std::pair<uint32_t, uint32_t>> terminal_ref; // contains a linked list of all references to the same terminal vertex
                  std::vector<uint32_t> terminal_ref_end;

                  F tr_func;

            friend std::ostream& operator<< <T,F,S> (std::ostream&, const array_trie&);
      };

      template <typename T, class F, uint32_t S>
      array_trie<T,F,S>::array_trie (F tr_func)
                                    : nodes (1),
                                      terminal (),
                                      tr_func (tr_func) {}

      template <typename T, class F, uint32_t S>
      array_trie<T,F,S>::array_trie ()
                                    : nodes (1),
                                      terminal (),
                                      tr_func () {}

      template <typename T, class F, uint32_t S>
      array_trie<T,F,S>::~array_trie () {}

      template <typename T, class F, uint32_t S>
      void array_trie<T,F,S>::insert (const std::string& s, const T& value) {
            uint32_t idx = 0;

            for (auto c : s) {
                  uint32_t z = tr_func(c);

                  if (nodes[idx].to[z] == uint32_t(-1)) {
                        nodes[idx].to[z] = nodes.size();
                        nodes.emplace_back();
                  }

                  idx = nodes[idx].to[z];
            }

            node& term = nodes[idx];

            if (term.value == uint32_t(-1)) {
                  term.value = terminal.size();

                  terminal.emplace_back(terminal_ref.size(), value);
                  terminal_ref_end.emplace_back(terminal_ref.size());
                  terminal_ref.emplace_back(term.value, uint32_t(-1));
            } else {
                  auto& t = terminal[term.value];
                  t.second = value;
                  terminal_ref[terminal_ref_end[term.value]].second = terminal_ref.size();
                  terminal_ref_end[term.value] = terminal_ref.size();
                  terminal_ref.emplace_back(term.value, uint32_t(-1));
            }
      }

      template <typename T, class F, uint32_t S>
      T& array_trie<T,F,S>::operator[] (const std::string& s) {
            uint32_t idx = 0;

            for (auto c : s) {
                  uint32_t z = tr_func(c);
                  assert (nodes[idx].to[z] != uint32_t(-1));

                  idx = nodes[idx].to[z];
            }
            
            return terminal[nodes[idx].value].second;
      }

      template <typename T, class F, uint32_t S> // trie structure is demolished by this operation
      void array_trie<T,F,S>::aho () {         // may be I would write a restoring function, but
            std::queue<uint32_t> pending;        // not now...
            uint32_t idx = 0;

            for (uint32_t i = 0; i < S; ++i) {
                  node& current = nodes[0];
                  uint32_t& next = current.to[i];

                  if (next == uint32_t(-1))
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

                  if (current.value == uint32_t(-1))
                        current.term_link = nodes[current.link].term_link;
                  else
                        current.term_link = idx;

                  for (uint32_t i = 0; i < S; ++i) {
                        uint32_t next = current.to[i];

                        if (next != uint32_t(-1)) {
                              pending.push(next);
                              nodes[next].link = nodes[current.link].to[i];
                              
                              continue;
                        }

                        current.to[i] = nodes[current.link].to[i];
                  }
            }
      }

      /*template <typename T, class F, uint32_t S>
      void array_trie<T,F,S>::find (const std::string& s, std::vector<int>& ans) {
            uint32_t idx = 0;

            for (auto c : s) {
                  idx = nodes[idx].to[tr_func(c)];
                  uint32_t t_idx = nodes[idx].term_link;

                  while (t_idx != 0) { // TODO: organise a normal fix
                        ans[nodes[t_idx].value] += 1;
                        t_idx = nodes[nodes[t_idx].link].term_link;
                  }
            }
      }*/

      template <typename T, class F, uint32_t S>
      void array_trie<T,F,S>::find (const std::string& s, auto func) {
            uint32_t idx = 0;

            for (uint32_t i = 0; i < s.size(); ++i) {
                  char c = s[i];

                  idx = nodes[idx].to[tr_func(c)];
                  uint32_t t_idx = nodes[idx].term_link;

                  while (t_idx != 0) {
                        auto tr_idx = terminal[nodes[t_idx].value].first;
                        while (tr_idx != uint32_t(-1)) {
                              func(tr_idx, i);
                              tr_idx = terminal_ref[tr_idx].second;
                        }
                        
                        t_idx = nodes[nodes[t_idx].link].term_link;
                  }
            }
      }
//                 ___________
//                |___DEBUG___|
#ifdef DEBUG
      template <typename T, class F, uint32_t S>
      uint32_t array_trie<T,F,S>::debug_get_link (uint32_t idx) {
            return nodes[idx].link;
      }

      template <typename T, class F, uint32_t S>
      uint32_t array_trie<T,F,S>::debug_get_idx (const std::string& s) {
            uint32_t idx = 0;

            for (auto c : s) {
                  uint32_t z = tr_func(c);
                  idx = nodes[idx].to[z];
            }

            return idx;
      }
#endif
}

#endif
#include <map>
#include <cstdlib>
#include <ctime>

int main () {
      std::string s;
      std::cin >> s;
 
      uint32_t N;
      std::cin >> N;
 
      std::vector<int> length;
      std::vector<std::vector<int>> ans;
 
      dst::array_trie<int> T;
      for (uint32_t i = 0; i < N; ++i) {
            std::string u;
            std::cin >> u;
            T.insert(u,i);
 
            length.push_back(u.size());
            ans.push_back(std::vector<int>());
      }
 
      T.aho();
      T.find(s, [&length, &ans](uint32_t n, uint32_t idx) -> void {
                        ans[n].push_back(idx-length[n]+2);
                        return;
                  });
 
      for (auto& v : ans) {
            std:: cout << v.size();
 
            for (auto x : v)
                  std::cout << ' ' << x;
            std::cout << '\n';
      }
}

