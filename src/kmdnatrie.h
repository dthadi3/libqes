/*
 * ============================================================================
 *
 *       Filename:  kmdnatrie.h
 *
 *    Description:  A DNA trie data structure
 *
 *        Version:  1.0
 *        Created:  13/05/14 14:34:16
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#ifndef KMDNATRIE_H
#define KMDNATRIE_H


#include <kmutil.h>
#include <kmstr.h>

typedef struct __dnatrie_node_t {
    struct __dnatrie_node_t *children[4];
    union {
        size_t count;
        void *data
    } data;
} dnatrie_node_t;


tyepdef struct __dnatrie_t {
    dnatrie_node_t *nodes;
    dnatrie_node_t *next_node;
    size_t nodes_alloced;
}

dnatrie_t *dnatrie_create();
void dnatrie_insert(dnatrie_t *trie, const char *seq);
size_t dnatrie_lookup(dnatrie_t *trie, const char *seq);
void dnatrie_remove(dnatrie_t *trie, const char *seq);
extern size_t char_to_nt_offset(const char nt);

#endif /* KMDNATRIE_H */
