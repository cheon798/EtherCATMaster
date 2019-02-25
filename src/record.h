#ifndef _RECORD_H_
#define _RECORD_H_

#include "errors.h"
#include "parameters.h"

/* PIPE Declaration */
int recd_pipe;	// record data pipe file descriptor

/* record pthread declaration from recd_pipe */
pthread_t recd_pthread;

/* recording file descriptor */
FILE* recd_file = NULL;

/* Operated Function */
static void recordDATA(FILE *file, const data_t *data);
static void *recdDATA_routine(void *arg);
static void createRECD_pthread(void);
static void cancleRECD_pthread(void);

#endif /*_RECORD_H_*/
