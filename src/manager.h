#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "errors.h"
#include "parameters.h"

/* PIPE Declaration */
int cmd_pipe;	// command pipe file descriptor

/* Process ID Declaration */
pid_t disp_pid;
pid_t recd_pid;

/* FLAG Initialization */
enum EXAM_FLAG M_FLAG_STAT_WAIT		= EXAM_FLAG_OFF;
enum EXAM_FLAG M_FLAG_STAT_LOAD_EMM	= EXAM_FLAG_OFF;
enum EXAM_FLAG M_FLAG_STAT_NODE		= EXAM_FLAG_OFF;
enum EXAM_FLAG M_FLAG_STAT_DIST_CLK	= EXAM_FLAG_OFF;
enum EXAM_FLAG M_FLAG_STAT_RDNC		= EXAM_FLAG_OFF;
enum EXAM_FLAG M_FLAG_STAT_ELMO		= EXAM_FLAG_OFF;
enum EXAM_FLAG M_FLAG_DISP_STAT		= EXAM_FLAG_OFF;
enum EXAM_FLAG M_FLAG_RECD_STAT		= EXAM_FLAG_OFF;
enum EXAM_FLAG M_FLAG_SERV_STAT		= EXAM_FLAG_OFF;
enum EXAM_FLAG M_FLAG_POS_MODE		= EXAM_FLAG_OFF;
enum EXAM_FLAG M_FLAG_VEL_MODE		= EXAM_FLAG_OFF;
enum EXAM_FLAG M_FLAG_CUR_MODE		= EXAM_FLAG_OFF;
enum EXAM_FLAG M_FLAG_MAIN_TASK		= EXAM_FLAG_OFF;
enum EXAM_FLAG M_FLAG_CTRL_TASK		= EXAM_FLAG_OFF;
enum EXAM_FLAG M_FLAG_EXIT			= EXAM_FLAG_OFF;

/* Operated Function */
static void showMENU(void);
static int inputCMD(cmd_t *cmd);
static int sendCMD(cmd_t *cmd);

static pid_t getPID(const char *pname);
static int sendSGN(const pid_t pid, const int signum);

#endif /*_MANAGER_H_*/
