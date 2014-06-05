/*
 * ============================================================================
 *
 *       Filename:  kmdnatrie.c
 *
 *    Description:  A DNA trie data structure
 *
,*        Version:  1.0
 *        Created:  13/05/14 14:34:16
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "kmdnatrie.h"

unsigned char char_2na_table[256] = {
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 0, 4, 1,  4, 4, 4, 2,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  3, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 0, 4, 1,  4, 4, 4, 2,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  3, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4
};

#define NT_IDX(nt, res, onfail) STMT_BEGIN      \
    res = char_2na_table[nt];                   \
    if (res > 3) {                              \
        onfail;                                 \
    } STMT_END

dnatrie_t *
dnatrie_create (void)
{
    dnatrie_t *trie = km_calloc(1, sizeof(*trie));
    trie->nodes_alloced = 1<<10;
    trie->nodes = km_calloc(trie->nodes_alloced, sizeof(*trie->nodes));
    trie->next_node = trie->nodes;

}

int
dnatrie_insert(dnatrie_t *trie, const char *seq, size_t len,
        dnatrie_node_data_t data)
{
    dnatrie_node_t *last_nt_in_trie;
    dnatrie_node_t *iter;
    dnatrie_node_t *next;
    size_t iii = 0;
    size_t prefix_in_tree = 0;

    iter = dnatrie_lookup(trie, seq, len, &prefix_in_tree);
    while (prefix_in_tree < len) {
        size_t child_num = 0;
        /* Always save this, but only increment the next node if we need it */
        next = trie->next_node;
        /* Reserve us a new node for the next nt */
        trie->next_node += 1;
        /* Check for resizing, do so if necessary */
        if (trie->nodes - iter <= 1) {
            trie->nodes_alloced = kmroundupz(trie->nodes_alloced);
            trie->nodes = km_realloc(trie->nodes,
                    trie->nodes_alloced * sizeof(trie->nodes));
            if (trie->nodes == NULL) {
                return 0;
            }
        }
        iter->children[
    }
}

dnatrie_node_t *
dnatrie_lookup(dnatrie_t *trie, const char *seq, size_t len,
               size_t *prefix_len)
{
    dnatrie_node_t *iter = trie->nodes;
    dnatrie_node_t *parent = iter;
    size_t iii = 0;
    unsigned char child
    if (len == 0) {
        len = strlen(seq);
    }
    while (iter != NULL && iii < len) {
        size_t res = 0;
        parent = iter;
        NT_IDX(seq[iii++], res, *prefix_len = iii; return NULL;)
        iter = iter->children[res];
    }



void dnatrie_remove(dnatrie_t *trie, const char *seq);
extern size_t char_to_nt_offset(const char nt);

