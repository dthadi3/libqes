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

dnatrie_t *
dnatrie_create ()
{
    dnatrie_t *trie = km_calloc(1, sizeof(*trie));
    trie->alloced_nodes = 1<<10;
    trie->nodes = km_calloc(trie->alloced_nodes, size_t(*trie->nodes));
    trie->next_node = trie->nodes;

}
void
dnatrie_insert(dnatrie_t *trie, const char *seq)
{
    dnatrie_node_t *iter = trie->nodes;
    while (iter != NULL) {
        iter = 
    }

}
size_t dnatrie_lookup(dnatrie_t *trie, const char *seq);
void dnatrie_remove(dnatrie_t *trie, const char *seq);
extern size_t char_to_nt_offset(const char nt);

