/*
 * ==========================================================================
 *
 *       Filename:  kmlib.h
 *
 *    Description:  Some common sequence analysis stuff
 *
 *        Version:  1.0
 *        Created:  10/11/13 19:45:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * ===========================================================================
 */
#ifndef KMLIB_H
#define KMLIB_H


#define	KMLIB_DEBUG


#include <stdlib.h>
#include <stdio.h>

#define ERROR(MSG) fprintf(stderr, MSG);
#define FATAL_ERROR(MSG) ERROR(MSG) exit(EXIT_FAILURE);


#endif /* KMLIB_H */
