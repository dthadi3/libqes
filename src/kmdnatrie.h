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

typedef union __dnatrie_node_data {
    size_t count;
    void *ptr;
} dnatrie_node_data_t;

typedef struct __dnatrie_node {
    struct __dnatrie_node *children[4];
    dnatrie_node_data_t data;
} dnatrie_node_t;


typedef struct __dnatrie {
    dnatrie_node_t *nodes;
    dnatrie_node_t *next_node;
    size_t nodes_alloced;
} dnatrie_t;

dnatrie_t *dnatrie_create(void);
int dnatrie_insert(dnatrie_t *trie, const char *seq, size_t len);
dnatrie_node_t *dnatrie_lookup(dnatrie_t *trie, const char *seq, size_t len, size_t *prefix_len);
void dnatrie_remove(dnatrie_t *trie, const char *seq);
extern size_t char_to_nt_offset(const char nt);

#endif /* KMDNATRIE_H */
