/*
 * ============================================================================
 *
 *       Filename:  testdata.c
 *
 *    Description:  Data for tests
 *
 *        Version:  1.0
 *        Created:  31/05/14 20:03:59
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "testdata.h"

const size_t n_loremipsum_lines = 11;
const size_t loremipsum_fsize = 80+76+80+75+80+79+77+75+69+1+20;
const size_t loremipsum_line_lens[] = {
    80, 76, 80, 75, 80, 79, 77, 75, 69, 1, 20
};
const char *loremipsum_lines[] = {
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec ornare tortor et\n",
    "rhoncus iaculis. Sed suscipit, arcu nec elementum vestibulum, tortor tortor\n",
    "dictum dui, eu sodales magna orci eu libero. Cras commodo, ligula tempor auctor\n",
    "vulputate, eros urna gravida eros, eget congue leo quam quis mi. Curabitur\n",
    "luctus augue nibh, eget vehicula augue commodo eget. Donec condimentum molestie\n",
    "adipiscing. In non purus lacus. Nam nec mollis mauris. Donec rhoncus, diam sit\n",
    "amet rhoncus viverra, lectus risus tincidunt ipsum, in dignissim justo purus\n",
    "eget enim. Fusce congue nulla egestas est auctor faucibus. Integer feugiat\n",
    "molestie leo, a interdum neque pretium nec. Etiam sit amet nibh leo.\n",
    "\n",
    "End of lorem ipsum.\n",
};
const char *first_fastq_read[] = {
    "HWI-ST960:105:D10GVACXX:2:1101:1151:2158",
    "1:N:0: bcd:RPI9 seq:CACGATCAGATC",
    "CACGATCAGATCAANGACATTGAATCTATATGT",
    "JJJJJJJIJHIJCC#4ADFFHHHGHJJJJIJJJ",
};
const size_t first_fastq_len = 33;
