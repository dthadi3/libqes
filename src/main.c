/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  test code
 *
 *        Version:  1.0
 *        Created:  01/08/13 18:58:44
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include <stdio.h>
#include "kmseq.h"

int main(int argc, char *argv[]) {

    kmfile *file = create_kmfile(argv[1]);
    printf("kmfile at %x\n", file);

    destroy_kmfile(file);
    file = NULL;

    return 0;
}
