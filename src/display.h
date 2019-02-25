#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "errors.h"
#include "parameters.h"

/* PIPE Declaration */
int disp_pipe;	// display data pipe file descripter

/* display pthread declaration from recd_pipe  */
pthread_t disp_pthread;

/* Operated Function */
static void displayDATA(const data_t *data);
static void *dispDATA_routine(void *arg);
static void createDISP_pthread(void);
static void cancleDISP_pthread(void);

#endif /*_DISPLAY_H_*/
