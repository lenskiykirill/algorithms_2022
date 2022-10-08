#ifndef __DST_SUFFIX_TREE
#define __DST_SUFFIX_TREE

#include <iostream>
#include <string>
#include <vector>

namespace dst {
      namespace dft {
            const uint8_t alpha = 'z'-'a'+3;
            const uint8_t Alpha = ('Z'-'A') + ('z'-'a') + 4;

            template <uint8_t S>
            struct tr_func;

            template<>
            struct tr_func<alpha> {
                  uint8_t operator() (char c) {
                        if (c == '^')
                              return 0;
                        if (c == '$')
                              return alpha-1;

                        assert ((c >= 'a') && (c <= 'z'));
                        return 1 + c - 'a';
                  };
            };
      }
}

namespace dst {
      template <uint8_t S = dft::alpha, class F = dft::tr_func<S>>
      class s_tree {
            private:
                  struct position {
                        size_t node;
                        uint8_t edge;
                        size_t length;
                  };

            public:
                  s_tree ();
                  s_tree (F tr_func);

                  append (const std::string& s);
                  
            private:
                  size_t get_child (size_t node, uint8_t edge); // get the child of node; -1 on fail
                  
                  void add_child (size_t node, size_t edge,
                                  size_t s_begin, size_t length); // set the child of node

                  size_t sp_get_link (size_t node);
                  void sp_set_link (size_t node, size_t link);
                  size_t get_pos_edge (size_t node, uint8_t edge);
                  size_t get_length (size_t node, uint8_t edge);
                  
                  position get_link (position pos);

                  struct node {
                        size_t edge[S];
                        size_t pos_edge[S];
                        size_t length[S]; // if b is directly after a, a.length = 1!

                        node () {for (size_t i = 0; i < S; ++i) edge[i] = size_t(-1); };
                        //iterate (auto func); ?
                  };

                  std::vector<node> nodes;
                  std::vector<uint8_t> string;

                  F tr_func;
      };

      template <uint8_t S, class F>
      s_tree<S,F>::s_tree ()
                          : nodes(1),
                            tr_func () {}

      template <uint8_t S, class F>
      s_tree<S,F>::s_tree (const F& tr_func)
                          : nodes(1),
                            tr_func (tr_func) {}

      template <uint8_t S, class F>
      size_t s_tree<S,F>::get_child (size_t node, uint8_t edge) {
            return nodes[node].edge[edge];
      }

      template <uint8_t S, class F>
      size_t s_tree<S,F>::sp_get_link (size_t node) {
            return nodes[node].link;
      }

      template <uint8_t S, class F>
      void s_tree<S,F>::sp_set_link (size_t node, size_t link) {
            nodes[node].link = link;
      }

      template <uint8_t S, class F>
      size_t s_tree<S,F>::get_pos_edge (size_t node, uint8_t edge) {
            return nodes[node].pos_edge[edge];
      }

      template <uint8_t S, class F>
      size_t s_tree<S,F>::get_length (size_t node, uint8_t edge) {
            return nodes[node].length[edge];
      }

      template <uint8_t S, class F>
      void s_tree<S,F>::add_child (size_t node, size_t edge,
                                   size_t s_begin, size_t length) {
            uint8_t c = string[s_begin];

            nodes[node].edge[c] = edge;
            nodes[node].pos_edge[c] = s_begin;
            nodes[node].length[c] = length;
      }

      template <uint8_t S, class F>
      s_tree<S,F>::position s_tree<S,F>::get_link (position pos) {
            size_t link_idx = sp_get_link(pos.node);
            uint8_t edge = pos.edge;

            size_t text_pos = get_pos_edge(pos.node, pos.edge);
            size_t length = pos.length;

            while (length > get_length(link_idx, edge)) {
                  size_t shift = get_lenght(link_idx, edge);

                  length -= (shift);
                  text_pos += shift;

                  link_idx = get_child(link_idx, edge);
                  edge = string[text_pos];
            }
      }
}

#endif
