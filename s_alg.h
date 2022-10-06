#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>

#ifndef __DST_S_ALGORITHMS
#define __DST_S_ALGORITHMS

namespace dst {

      // this is the common translation function. It features 26 alphabetic lowercase letters
      // and special separation sign -- #.
      struct __basic_tr_function {
            size_t operator()(char c) {
                  if (c == '#')
                        return 0;

                  assert ((c >= 'a') && (c <= 'z'));
                  return c - 'a';
            };
      };
      
      // __basic_array_trie structure is a general template for more elaborate trie structures
      // such as Aho-Corasick trie and suffix trie
      // in general. __basic_array_trie supports two arrays -- go and at. It also supports
      // inserting, erasing and even iterating. Wander-tool, unique of its kind. It may as well support
      // compression [for suffix trie, for example].
      

      template <typename T, class F = __basic_tr_function, size_t S = 27>
      class __basic_array_trie;

      template <typename T, class F, size_t S>
      std::ostream& operator<< (std::ostream&, const __basic_array_trie<T,F,S>&);

      template <typename T, class F, size_t S>
      class __basic_array_trie {
            public:
                  __basic_array_trie (F tr_func);                                 // Constructors and destructors
                  __basic_array_trie ();                                          // May be further developed
                  __basic_array_trie (const __basic_array_trie& other) = delete;  // But that scenario is unlikely
                  __basic_array_trie (__basic_array_trie&& other) = delete;       // 
                 ~__basic_array_trie () = default;                                // 

                  void insert (const std::string& s, const T& value);             // I used to love coding
               // void insert (const std::string& s, T&& value) = delete;         // these types of things
               // void insert (const char* s, const T& value) = delete;           // but I do no more.
               // void insert (const char* s, T&& value) = delete;                // 

                  T& operator[] (const std::string& s);
               // const T& operator[] (const std::string& s) const = delete;
               // T& operator[] (const char* s) = delete;
               // const T& operator[] (const char* s) const = delete;
                  
                  void normalize (); // FIXES link AND goto FOR EVERY NODE
            
            protected:

                  struct __node {
                        size_t children[S]; // numbers of children in 
                        size_t go[S];
                        size_t link; 
                        size_t value = -1;  // a pointer to terminal_nodes

                        __node () {
                              for (size_t i = 0; i < S; ++i)
                                    children[i] = -1;
                        };
                  };

                  std::vector<T> terminal_nodes; // array which stores values of terminal nodes
                  std::vector<__node> nodes; // array of nodes. nodes 0 is always epsilon

                  F tr_func; // basic translation function. Used to translate chars into numbers

            friend std::ostream& operator<< <T,F,S> (std::ostream&, const __basic_array_trie&);
      };

      template <typename T, class F, size_t S>
      __basic_array_trie<T,F,S>::__basic_array_trie (F tr_func) 
                                                    : terminal_nodes(),
                                                      nodes (1),
                                                      tr_func (tr_func) {}

      template <typename T, class F, size_t S>
      __basic_array_trie<T,F,S>::__basic_array_trie ()
                                                    : terminal_nodes(),
                                                      nodes (1),
                                                      tr_func () {}

      template <typename T, class F, size_t S>
      std::ostream& operator<< (std::ostream& os, const __basic_array_trie<T,F,S>& tr) {
            os << '\n' << " ____________________\n"\
                          "| __basic_array_trie |\n"\
                          "|____DEBUG_OUTPUT____|\n"\
                          "\n"\
                          "1) nodes: " << tr.nodes.size() << "\n"\
                          "2) terminal_nodes: " << tr.terminal_nodes.size() << "\n";
            os << '\n';
            return os;
      }


      // THIS SECTION IS COVERED BY TESTS 5.10.2022; VERYFIED -- OK.

      template<typename T, class F, size_t S>
      void __basic_array_trie<T,F,S>::insert (const std::string& s, const T& value) { // TODO: switch to using iterators
            size_t current_idx = 0;
            
            for (auto c : s) {
                  size_t z = tr_func(c);

                  if (nodes[current_idx].children[z] == -1) {
                        nodes[current_idx].children[z] = nodes.size();
                        nodes.emplace_back();
                  }

                  current_idx = nodes[current_idx].children[z];
            }

            __node& current_node = nodes[current_idx];

            if (current_node.value == -1) {
                  current_node.value = terminal_nodes.size();
                  terminal_nodes.emplace_back(value);
            } else {
                  terminal_nodes[current_node.value] = value;
            }
      }

      // THIS SECTION IS COVERED BY TESTS 5.10.2022; VERYFIED -- OK.

      template<typename T, class F, size_t S>
      T& __basic_array_trie<T,F,S>::operator[] (const std::string& s) { // TODO: implement find function
            size_t current_idx = 0;

            for (auto c : s) {
                  size_t z = tr_func(c);
                  assert (nodes[current_idx].children[z] != -1);

                  current_idx = nodes[current_idx].children[z];
            }

            return terminal_nodes[nodes[current_idx].value];
      }

      template<typename T, class F, size_t S>
      void __basic_array_trie<T,F,S>::normalize { // fixes link and goto for every trie node
            // THIS FUNCTION IMPLEMENTS TRIE NORMALISATION ALGORITHM
            // USEFUL FOR Aho-Corasick ALGORITHM AND MANY OTHERS AS WELL
            // GENERALLY IT SIMPLY STARTS BFS FROM ROOT AND DOES THE FOLLOWING
            // 1) link[v] = maximal nontrivial suffix in the tree.
            // 2) go[v][c] = maximal suffix of v+c, might be trivial as well
            // 
            // If u is a parent of v (v = u+c), link[v] = go[u][c].
            // go[u][]
      }
}

#endif
