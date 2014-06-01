/*
 * ============================================================================
 *
 *       Filename:  helpers.c
 *
 *    Description:  Helpers for tests
 *
 *        Version:  1.0
 *        Created:  29/05/14 16:22:23
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "helpers.h"

/* This is how we name out output files, n_writables is the num of writable
   files we've made. */
static int n_writables = 0;
/* This holds the prefix. It is set (via extern) in test.c's main func. */
char * data_prefix = NULL;


/*===  FUNCTION  ============================================================*
Name:           find_data_file
Paramters:      filepath: the file, under ./data, to find
Description:    Finds the full path to ``filepath``
Returns:        A ``char *`` on the heap that contains a valid path to the
                file ``filepath``, or NULL on error;
 *===========================================================================*/

char *
find_data_file(const char * filepath)
{
    size_t buflen = 1<<12;
    size_t len = 0;
    char buf[buflen];

    /* Bail out if we can't get the data_prefix. */
    if (data_prefix == NULL) {
        return NULL;
    }
    len = snprintf(buf, buflen, "%s/data/%s", data_prefix, filepath);
    /* File name is too long, return NULL */
    if (len >= buflen) {
        return NULL;
    }
    /* Null terminate AFTER making sure len < buflen */
    buf[len] = '\0';
    /* Check we can access the file. If so, strdup & return */
    if (access(buf, F_OK) == 0) {
        return strndup(buf, buflen);
    }
    /* If we get to here, something's gone wrong. */
    return NULL;
}


/*===  FUNCTION  ============================================================*
Name:           get_writable_file
Paramters:      None
Description:    Gets a temporary file which can be written to, under the data
                directory.
Returns:        A ``char *`` on the heap that contains a valid path to a file
                writable file, or NULL on error;
 *===========================================================================*/

char *
get_writable_file()
{
    size_t buflen = 1<<12;
    size_t len = 0;
    char buf[buflen];

    /* Bail out if we can't get the data_prefix. */
    if (data_prefix == NULL) {
        return NULL;
    }
    len = snprintf(buf, buflen, "%s/data/%05d", data_prefix, n_writables++);
    /* File name is too long, return NULL */
    if (len >= buflen) {
        return NULL;
    }
    /* Null terminate AFTER making sure len < buflen */
    buf[len] = '\0';
    /* Check we can access the file. If so, strdup & return */
    return strndup(buf, buflen);
}


/*===  FUNCTION  ============================================================*
Name:           clean_writable_file
Paramters:      char *: filename
Description:    Unlink ``file`` and free the memory holding the path.
Returns:        void
 *===========================================================================*/

void
clean_writable_file(char *filepath)
{
    if (filepath != NULL) {
        remove(filepath);
        free(filepath);
    }
}


/*===  FUNCTION  ============================================================*
Name:           crc32_file
Paramters:      char *: filename
Description:    Calculate (using zlib) crc32 checksum.
Returns:        const char *: The crc32 sum, or NULL on error.
 *===========================================================================*/

char *
crc32_file(const char *filepath)
{
    FILE *fp = NULL;
    size_t buflen = 1<<10;
    size_t len = 0;
    unsigned char buffer[buflen];
    unsigned long crc = crc32(0L, Z_NULL, 0);
    char crcbuf[9];

    /* Open file */
    fp = fopen(filepath, "rb");
    if (fp == NULL) {
        return NULL;
    }
    while (!feof(fp)) {
        len = fread(buffer, 1, buflen, fp);
        crc = crc32(crc, buffer, len);
    }
    fclose(fp);
    len = snprintf(crcbuf, 9, "%08lx", crc);
    crcbuf[len] = '\0';
    return strdup(crcbuf);
}
