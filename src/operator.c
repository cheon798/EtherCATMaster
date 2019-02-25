#include <native/task.h>
#include <native/timer.h>
#include <native/pipe.h>

#include <rtdk.h>

#include "operator.h"

int 
main(int argc, char *argv[])
{
	int rtn;


	/* Perform auto-init of rt_printf busffers */
	rt_print_auto_init(1);

	rt_printf("\n ================================================= CTRL-C to exit! \n");

	rt_printf("\n Real-Time EtherCAT Master is about to start... \n");

	signal(SIGTERM, catchSGNL);
	signal(SIGINT, catchSGNL);

	rt_printf("\n -- Prevent memory paging... \n");

	rtn = mlockall(MCL_CURRENT | MCL_FUTURE);
	if (rtn == -1) {
		errExit("mlockall %s", main);
	}


	/* RT PIPE Creation */
	rt_printf("\n -- Create the Real-Time PIPE. \n");

	/* Create the CMD_PIPE */
	rtn = rt_pipe_create(&CMD_PIPE,				// pipe descriptor address
						 "CMDPIPE",				// message pipe symbolic name 
						 0,						// device associated with pipe:0-->/dev/rtp0 
						 sizeof(cmd_t)*50);		// pipe buffer pool size 
	
	if (rtn != 0) {
		rt_errExitEN(rtn, "rt_pipe_create() <CMD_PIPE> main");
	}
	else {
		rt_printf("\n ---- Success creating CMD_PIPE. \n");
	}


	/* Create the DISP_PIPE */
	rtn = rt_pipe_create(&DISP_PIPE,			// pipe descriptor address
						 "DISP_PIPE",			// message pipe symbolic name
						 1,						// device associated with pipe:1-->/dev/rtp1
						 sizeof(data_t)*50);	// pipe buffer pool size 
	
	if (rtn != 0) {
		rt_errExitEN(rtn, "rt_pipe_create() <DISP_PIPE> main");
	}
	else {
		rt_printf("\n ---- Success creating DISP_PIPE. \n");
	}

	/* Create the RECD_PIPE */
	rtn = rt_pipe_create(&RECD_PIPE,			// pipe descriptor address
						 "RECD_PIPE",			// message pipe symbolic name
						 2,						// device associated with pipe:2-->/dev/rtp2 
						 sizeof(data_t)*50);	// pipe buffer pool size 
	
	if (rtn != 0) {
		rt_errExitEN(rtn, "rt_pipe_create() <RECD_PIPE> main");
	}
	else {
		rt_printf("\n ---- Success creating RECD_PIPE. \n");
	}


	/* Main RT TASK Creation */
	rt_printf("\n -- Create the Real-Time TASK. \n");

	/* Create the MAIN_TASK */
	rtn = rt_task_create(&MAIN_TASK,		// task descriptor address
						 "MAINTASK",		// task name 
						 TASK_STKSZ,		// stack size : 0 --> reasonable pre-defined size 
						 TASK_PRIO_MAIN,	// task priority : 99
						 TASK_MODE);		// task creation mode : T_FPU|T_CPU(0) 
	
	if (rtn != 0) {
		rt_errExitEN(rtn, "rt_task_create() <MAIN_TASK> main");
	} 
	else {
		rt_printf("\n ---- Success creating MAIN_TASK. \n");
	
		/* Start the MAIN_TASK */
		rtn = rt_task_start(&MAIN_TASK,			// task descriptor address
							&mainTASK_routine,	// task's body routine : task entry point
							NULL);				// user-defined opaque cookie
		
		if (rtn !=0) {
			rt_errExitEN(rtn, "rt_task_start() <MAIN_TASK> main");
		}
	}


	pause();

	return 0;
}


void 
cleanALL(void)
{
	int rtn;
	
	/* Device state file close */
	rtn = fclose(dev_file_disp);
		
	if (rtn == -1) {
		errExit("fclose dev_file_disp %s", cleanALL);
	}
	else {
		rt_printf("\n >>>> Success closing dev_file_disp.");
	}
	
	rtn = fclose(dev_file_recd);
		
	if (rtn == -1) {
		errExit("fclose dev_file_recd %s", cleanALL);
	}
	else {
		rt_printf("\n >>>> Success closing dev_file_recd.");
	}


	/* RT PIPE Deletion */
	rtn = rt_pipe_delete(&CMD_PIPE);
	
	if (rtn != 0) {
		rt_errExitEN(rtn, "rt_pipe_delete() <CMD_PIPE> cleanALL");
	}
	else {
		rt_printf("\n >>>> Success deleting CMD_PIPE. \n");
	}

	if (RECD_STAT_FLAG == EXAM_FLAG_ON) {

		rtn = rt_pipe_delete(&RECD_PIPE);
	
		if (rtn != 0) {
			rt_errExitEN(rtn, "rt_pipe_delete() <RECD_PIPE> cleanALL");
		}
		else {
			rt_printf("\n >>>> Success deleting RECD_PIPE. \n");
			RECD_STAT_FLAG = EXAM_FLAG_OFF;
		}
	}
	else {
		rt_printf("\n >>>> RECD_PIPE is already deleted or not create. \n");
	}
	
	if (DISP_STAT_FLAG == EXAM_FLAG_ON) {

		rtn = rt_pipe_delete(&DISP_PIPE);
	
		if (rtn != 0) {
			rt_errExitEN(rtn, "rt_pipe_delete() <DISP_PIPE> cleanALL");
		}
		else {
			rt_printf("\n >>>> Success deleting DISP_PIPE. \n");
			DISP_STAT_FLAG = EXAM_FLAG_OFF;
		}
	}
	else {
		rt_printf("\n >>>> DISP_PIPE is already deleted or not create. \n");
	}


	/* RT TASK Deletion */
	if (CTRL_TASK_FLAG == EXAM_FLAG_ON) {

		rtn = rt_task_delete(&CTRL_TASK);
		
		if (rtn != 0) {
			rt_errExitEN(rtn, "rt_task_delete() <CTRL_TASK> cleanALL");
		}
		else {
			rt_printf("\n >>>> Success deleting CTRL_TASK. \n");
			CTRL_TASK_FLAG = EXAM_FLAG_OFF;
		}
	}
	else {
		rt_printf("\n >>>> CTRL_TASK is already deleted or not create. \n");
	}

	if (MAIN_TASK_FLAG == EXAM_FLAG_ON) {
		
		rtn = rt_task_delete(&MAIN_TASK);
		
		if (rtn != 0) {
			rt_errExitEN(rtn, "rt_task_delete() <MAIN_TASK> cleanALL");
		}
		else {
			rt_printf("\n >>>> Success deleting MAIN_TASK. \n");
			MAIN_TASK_FLAG = EXAM_FLAG_OFF;
		}
	}
	else {
		rt_printf("\n >>>> MAIN_TASK is already deleted or not create. \n");
	}
}


void
catchSGNL(int signal)
{
	cleanALL();

	rt_printf("\n ====================================================== Good Bye~! \n");
}


void
mainTASK_routine(void *cookie)
{
	int i, j;
	int rtn;
	unsigned long ovr;

	/* Command & data structure */
	cmd_t readCMD;
	data_t viewDATA, writeDATA;

	/* Initialize the command & data structure */
	readCMD.command = EXAM_CMD_WAIT;

	for (i=0 ; i<S_NUM ; i++) {
		for (j=0 ; j<S_DOF ; j++) {
			readCMD.bar[i].pst_t[j][BUF] 	= ABS[BUF];
			readCMD.bar[i].q_t[j] 			= 0.0;
			readCMD.bar[i].qdot_t[j] 		= 0.0;
			readCMD.bar[i].q2dot_t[j] 		= 0.0;
			readCMD.bar[i].curr_t[j] 		= 0.0;
			readCMD.bar[i].torq_t[j] 		= 0.0;
		}
	}

	for (i=0 ; i<S_NUM ; i++) {
		for (j=0 ; j<S_DOF ; j++) {
			viewDATA.stick[i].q_d[j] 		= 0.0;
			viewDATA.stick[i].q_a[j] 		= 0.0;
			viewDATA.stick[i].qdot_d[j] 	= 0.0;
			viewDATA.stick[i].qdot_a[j] 	= 0.0;
			viewDATA.stick[i].q2dot_d[j] 	= 0.0;
			viewDATA.stick[i].q2dot_a[j] 	= 0.0;
			viewDATA.stick[i].curr_d[j] 	= 0.0;
			viewDATA.stick[i].curr_a[j] 	= 0.0;
			viewDATA.stick[i].torq_d[j] 	= 0.0;
			viewDATA.stick[i].torq_a[j] 	= 0.0;
		}
	}

	for (i=0 ; i<S_NUM ; i++) {
		for (j=0 ; j<S_DOF ; j++) {
			writeDATA.stick[i].q_d[j] 		= 0.0;
			writeDATA.stick[i].q_a[j] 		= 0.0;
			writeDATA.stick[i].qdot_d[j] 	= 0.0;
			writeDATA.stick[i].qdot_a[j] 	= 0.0;
			writeDATA.stick[i].q2dot_d[j] 	= 0.0;
			writeDATA.stick[i].q2dot_a[j] 	= 0.0;
			writeDATA.stick[i].curr_d[j] 	= 0.0;
			writeDATA.stick[i].curr_a[j] 	= 0.0;
			writeDATA.stick[i].torq_d[j] 	= 0.0;
			writeDATA.stick[i].torq_a[j] 	= 0.0;
		}
	}

	/* Set the MAIN_TASK period */
	rtn = rt_task_set_periodic(NULL,
							   TM_NOW,
							   rt_timer_ns2ticks(TASK_PERIOD_MAIN));
	
	if (rtn != 0) {
		rt_errExitEN(rtn, "rt_task_set_periodic() <MAIN_TASK> mainTASK_routine");
	}
	else {
		MAIN_TASK_FLAG = EXAM_FLAG_ON;

		rt_printf("\n ---- Success setting Main_TASK period. \n");

		rt_printf("\n ====================================== Main Real-Time TASK Start! \n");
	}

	while (MAIN_TASK_FLAG) {

		rtn = rt_task_wait_period(&ovr);

		if (rtn != 0) {
			rt_errExitEN(rtn, "rt_task_wait_period() <MAIN_TASK> mainTASK_routine");
		}
		else {
			/* Read CMD_PIPE */
			rtn = rt_pipe_read(&CMD_PIPE,
							   &readCMD,
							   sizeof(cmd_t),
							   TM_NONBLOCK);

			if (rtn == sizeof(cmd_t)) {
				rt_printf("\n ------ CMD NO.: %d  Data Size : %d \n", readCMD.command, rtn);
				performCMD(readCMD);
			}

			/* Write DISP_PIPE */
			if (DISP_STAT_FLAG == EXAM_FLAG_ON) {
				fscanf(dev_file_disp, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
							&(viewDATA.stick[0].q_d[0]), 		&(viewDATA.stick[0].q_d[1]),
							&(viewDATA.stick[0].q_a[0]), 		&(viewDATA.stick[0].q_a[1]),
							&(viewDATA.stick[0].qdot_d[0]), 	&(viewDATA.stick[0].qdot_d[1]),
							&(viewDATA.stick[0].qdot_a[0]), 	&(viewDATA.stick[0].qdot_a[1]),
							&(viewDATA.stick[0].q2dot_d[0]), 	&(viewDATA.stick[0].q2dot_d[1]),
							&(viewDATA.stick[0].q2dot_a[0]), 	&(viewDATA.stick[0].q2dot_a[1]),
							&(viewDATA.stick[0].curr_d[0]), 	&(viewDATA.stick[0].curr_d[1]),
							&(viewDATA.stick[0].curr_a[0]), 	&(viewDATA.stick[0].curr_a[1]),
							&(viewDATA.stick[0].torq_d[0]), 	&(viewDATA.stick[0].torq_d[1]),
							&(viewDATA.stick[0].torq_a[0]), 	&(viewDATA.stick[0].torq_a[1]),
							&(viewDATA.stick[1].q_d[0]), 		&(viewDATA.stick[1].q_d[1]),
							&(viewDATA.stick[1].q_a[0]), 		&(viewDATA.stick[1].q_a[1]),
							&(viewDATA.stick[1].qdot_d[0]), 	&(viewDATA.stick[1].qdot_d[1]),
							&(viewDATA.stick[1].qdot_a[0]), 	&(viewDATA.stick[1].qdot_a[1]),
							&(viewDATA.stick[1].q2dot_d[0]), 	&(viewDATA.stick[1].q2dot_d[1]),
							&(viewDATA.stick[1].q2dot_a[0]), 	&(viewDATA.stick[1].q2dot_a[1]),
							&(viewDATA.stick[1].curr_d[0]), 	&(viewDATA.stick[1].curr_d[1]),
							&(viewDATA.stick[1].curr_a[0]), 	&(viewDATA.stick[1].curr_a[1]),
							&(viewDATA.stick[1].torq_d[0]), 	&(viewDATA.stick[1].torq_d[1]),
							&(viewDATA.stick[1].torq_a[0]), 	&(viewDATA.stick[1].torq_a[1]));
				
				rtn = rt_pipe_write(&DISP_PIPE,
									&viewDATA,
									sizeof(data_t),
									P_NORMAL);

				if (rtn != sizeof(data_t)) {
					rt_errExitEN(rtn, "rt_pipe_write() <DISP_PIPE> mainTASK_routine");
				}
			}

			/* Write RECD_PIPE */
			if (RECD_STAT_FLAG == EXAM_FLAG_ON) {
				fscanf(dev_file_recd, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
							&(writeDATA.stick[0].q_d[0]), 		&(writeDATA.stick[0].q_d[1]),
							&(writeDATA.stick[0].q_a[0]), 		&(writeDATA.stick[0].q_a[1]),
							&(writeDATA.stick[0].qdot_d[0]), 	&(writeDATA.stick[0].qdot_d[1]),
							&(writeDATA.stick[0].qdot_a[0]), 	&(writeDATA.stick[0].qdot_a[1]),
							&(writeDATA.stick[0].q2dot_d[0]), 	&(writeDATA.stick[0].q2dot_d[1]),
							&(writeDATA.stick[0].q2dot_a[0]), 	&(writeDATA.stick[0].q2dot_a[1]),
							&(writeDATA.stick[0].curr_d[0]), 	&(writeDATA.stick[0].curr_d[1]),
							&(writeDATA.stick[0].curr_a[0]), 	&(writeDATA.stick[0].curr_a[1]),
							&(writeDATA.stick[0].torq_d[0]), 	&(writeDATA.stick[0].torq_d[1]),
							&(writeDATA.stick[0].torq_a[0]), 	&(writeDATA.stick[0].torq_a[1]),
							&(writeDATA.stick[1].q_d[0]), 		&(writeDATA.stick[1].q_d[1]),
							&(writeDATA.stick[1].q_a[0]), 		&(writeDATA.stick[1].q_a[1]),
							&(writeDATA.stick[1].qdot_d[0]), 	&(writeDATA.stick[1].qdot_d[1]),
							&(writeDATA.stick[1].qdot_a[0]), 	&(writeDATA.stick[1].qdot_a[1]),
							&(writeDATA.stick[1].q2dot_d[0]), 	&(writeDATA.stick[1].q2dot_d[1]),
							&(writeDATA.stick[1].q2dot_a[0]), 	&(writeDATA.stick[1].q2dot_a[1]),
							&(writeDATA.stick[1].curr_d[0]), 	&(writeDATA.stick[1].curr_d[1]),
							&(writeDATA.stick[1].curr_a[0]), 	&(writeDATA.stick[1].curr_a[1]),
							&(writeDATA.stick[1].torq_d[0]), 	&(writeDATA.stick[1].torq_d[1]),
							&(writeDATA.stick[1].torq_a[0]), 	&(writeDATA.stick[1].torq_a[1]));
				
				rtn = rt_pipe_write(&RECD_PIPE,
									&writeDATA,
									sizeof(data_t),
									P_NORMAL);

				if (rtn != sizeof(data_t)) {
					rt_errExitEN(rtn, "rt_pipe_write() <RECD_PIPE> mainTASK_routine");
				}
			}
			
			/* TO DO
			   (Additional Unit Function in Main Task)
			   ......				 
			*/
		}
	} 
}


void
ctrlTASK_routine(void *cookie)
{
	int rtn;
	unsigned long ovr;
	long int turn;
	long int latency;
	long int latency_max = 0;
	double latency_over_ratio = 0.0;

	RTIME now, previous;

	/* Set the CTRL_TASK period */
	rtn = rt_task_set_periodic(NULL,
							   TM_NOW,
							   rt_timer_ns2ticks(TASK_PERIOD_CTRL));
	
	if (rtn != 0) {
		rt_errExitEN(rtn, "rt_task_set_periodic() <CTRL_TASK> ctrlTASK_routine");
	}
	else {
		CTRL_TASK_FLAG = EXAM_FLAG_ON;

		rt_printf("\n >>>> Success setting CTRL_TASK period. \n");

		rt_printf("\n =================================== Control Real-Time TASK Start! \n");
	}

	previous = rt_timer_read();

	while (CTRL_TASK_FLAG) {

		now = rt_timer_read();

		rtn = rt_task_wait_period(&ovr);
		
		if (rtn != 0) {
			rt_errExitEN(rtn, "rt_task_wait_period() <CTRL_TASK> ctrlTASK_routine");
		}
		else {
			/* Real-Time Control TASK Running.... */

			turn = (long)(now - previous);
			latency = turn - (long)(TASK_PERIOD_CTRL);	// 250,000[ns] --> 250[us] --> 0.25[ms] 

			if (latency_max < latency) latency_max = latency;

			latency_over_ratio = 100.0 * (double)(latency_max) / (double)(TASK_PERIOD_CTRL);

			if (latency_over_ratio >= 5.0 || latency_over_ratio <= -5.0) {
				rt_printf("\n %6ld [ns]   %5ld [ns]   %5ld [ns]   %4.3lf [%] \n",
						   turn, latency, latency_max, latency_over_ratio);
				exit(EXIT_FAILURE);
			}

			/* TO DO
			   (Additional Unit Function in Control Task)
			   ......				 
			*/
		}

		previous = now;
	}
}


void
performCMD(const cmd_t cmd) 
{
	int rtn;

	switch (cmd.command) {

		case EXAM_CMD_STOP :

			rt_printf("\n >> COMMAMD_STOP : %d \n", cmd.command);

			/* Device state file close */
			if (DISP_STAT_FLAG == EXAM_FLAG_ON) {

			rtn = fclose(dev_file_disp);
				
				if (rtn == -1) {
					errExit("fclose dev_file_disp %s", performCMD);
				}
				else {
					rt_printf("\n >>>> Success closing dev_file_disp.");
					DISP_STAT_FLAG = EXAM_FLAG_OFF;
				}
			}
			else {
				rt_printf("\n >>>> dev_file_disp is already closed or not open. \n");
			}
			
			if (RECD_STAT_FLAG == EXAM_FLAG_ON) {

				rtn = fclose(dev_file_recd);
				
				if (rtn == -1) {
					errExit("fclose dev_file_recd %s", performCMD);
				}
				else {
					rt_printf("\n >>>> Success closing dev_file_recd.");
					RECD_STAT_FLAG = EXAM_FLAG_OFF;
				}
			}
			else {
				rt_printf("\n >>>> dev_file_recd is already closed or not open. \n");
			}

			/* RT TASK Deletion */
			if (CTRL_TASK_FLAG == EXAM_FLAG_ON) {

				rtn = rt_task_delete(&CTRL_TASK);
		
				if (rtn != 0) {
					rt_errExitEN(rtn, "rt_task_delete() <CTRL_TASK> performCMD");
				}
				else {
					rt_printf("\n >>>> Success deleting CTRL_TASK. \n");
					CTRL_TASK_FLAG = EXAM_FLAG_OFF;
				}
			}
			else {
				rt_printf("\n >>>> CTRL_TASK is already deleted or not create. \n");
			}
	
			break;

		case EXAM_CMD_EMM_LOAD_CHK :

			break;

		case EXAM_CMD_EMM_LOAD :

			break;

		case EXAM_CMD_EMM_UNLOAD :

			break;

		case EXAM_CMD_NODE_CHK :

			break;

		case EXAM_CMD_DIST_CLK_CHK :

			break;

		case EXAM_CMD_DIST_CLK_SET :
			
			break;

		case EXAM_CMD_RDNC_CHK :

			break;

		case EXAM_CMD_ELMO_STATE :

			break;

		case EXAM_CMD_ELMO_RESET :

			break;

		case EXAM_CMD_ANGL_OFFSET :

			break;

		case EXAM_CMD_TORQ_OFFSET :

			break;

		case EXAM_CMD_DISP_ON :

			rt_printf("\n >> COMMAND_DISP_ON : %d \n", cmd.command);

			if (DISP_STAT_FLAG == EXAM_FLAG_OFF) {

				dev_file_disp = fopen("./data/device_data.txt", "r");
				
				if (dev_file_disp == NULL) {
					errExit("fopen() dev_file_disp %s", performCMD);
				}
				else {
					rt_printf("\n >>>> Success openning dev_file_disp. \n");
					DISP_STAT_FLAG = EXAM_FLAG_ON;
				}
			}
			else {
				rt_printf("\n >>>> dev_file_disp is already openned or not close. \n");
			}
				
			break;

		case EXAM_CMD_DISP_OFF :

			rt_printf("\n >> COMMAND_DISP_OFF : %d \n", cmd.command);

			if (DISP_STAT_FLAG == EXAM_FLAG_ON) {
				
				rtn = fclose(dev_file_disp);

				if (rtn == -1) {
					errExit("fclose() dev_file_disp %s", performCMD);
				}
				else {
					rt_printf("\n >>>> Success closing dev_file_disp. \n");
					DISP_STAT_FLAG = EXAM_FLAG_OFF;
				}
			}
			else {
				rt_printf("\n >>>> dev_file_disp is already closed or not open. \n");
			}

			break;
		
		case EXAM_CMD_RECD_ON :

			rt_printf("\n >> COMMAND_RECD_ON : %d \n", cmd.command);

			if (RECD_STAT_FLAG == EXAM_FLAG_OFF) {

				dev_file_recd = fopen("./data/device_data.txt", "r");

				if (dev_file_recd == NULL) {
					errExit("fopen() dev_file_recd %s", performCMD);
				}
				else {
					rt_printf("\n >>>> Success openning dev_file_recd. \n");
					RECD_STAT_FLAG = EXAM_FLAG_ON;
				}
			}
			else {
				rt_printf("\n >>>> dev_file_recd is already openned or not close. \n");
			}

			break;

		case EXAM_CMD_RECD_OFF :

			rt_printf("\n >> COMMAND_RECD_OFF : %d \n", cmd.command);
	
			if (RECD_STAT_FLAG == EXAM_FLAG_ON) {

				rtn = fclose(dev_file_recd);

				if (rtn == -1) {
					errExit("fclose() dev_file_recd %s", performCMD);
				}
				else {
					rt_printf("\n >>>> Success closing dev_file_recd. \n");
					RECD_STAT_FLAG = EXAM_FLAG_OFF;
				}
			}
			else {
				rt_printf("\n >>>> dev_file_recd is already closed or not open. \n");
			}

			break;

		case EXAM_CMD_SERVO_ON :

			break;

		case EXAM_CMD_SERVO_OFF :

			break;

		case EXAM_CMD_POS_MODE_ON :

			break;

		case EXAM_CMD_POS_MODE_OFF :

			break;

		case EXAM_CMD_VEL_MODE_ON :

			break;

		case EXAM_CMD_VEL_MODE_OFF :

			break;

		case EXAM_CMD_CUR_MODE_ON :

			break;

		case EXAM_CMD_CUR_MODE_OFF :

			break;

		case EXAM_CMD_POS_CTRL_ON :

			break;

		case EXAM_CMD_POS_CTRL_OFF :

			break;

		case EXAM_CMD_VEL_CTRL_ON :

			break;

		case EXAM_CMD_VEL_CTRL_OFF :

			break;

		case EXAM_CMD_CUR_CTRL_ON :

			break;

		case EXAM_CMD_CUR_CTRL_OFF :

			break;

		case EXAM_CMD_TRQ_CTRL_ON :

			break;

		case EXAM_CMD_TRQ_CTRL_OFF :

			break;

		case EXAM_CMD_CTRL_TASK_ON :

			rt_printf("\n >> COMMAND_CTRL_TASK_ON : %d \n", cmd.command);

			if (CTRL_TASK_FLAG == EXAM_FLAG_OFF) {

				rtn = rt_task_create(&CTRL_TASK,
									 "CTRL_TASK",
									 TASK_STKSZ,
									 TASK_PRIO_CTRL,
									 TASK_MODE);

				if (rtn != 0) {
					rt_errExitEN(rtn, "rt_task_create() <CTRL_TASK> performCMD");
				}
				else {
					rt_printf("\n >>>> Success creating CTRL_TASK. \n");

					rtn = rt_task_start(&CTRL_TASK,
										&ctrlTASK_routine,
										NULL);

					if (rtn != 0) {
						rt_errExitEN(rtn, "rt_task_start() <CTRL_TASK> performCMD");
					}
				}
			}
			else {
				rt_printf("\n >>>> CTRL_TASK is already created or not delete. \n");
			}

			break;

		case EXAM_CMD_CTRL_TASK_OFF :

			rt_printf("\n >> COMMAND_CTRL_TASK_OFF : %d \n", cmd.command);

			if (CTRL_TASK_FLAG == EXAM_FLAG_ON) {

				rtn = rt_task_delete(&CTRL_TASK);

				if (rtn != 0) {
					rt_errExitEN(rtn, "rt_task_delete() <CTRL_TASK> performCMD");
				}
				else {
					CTRL_TASK_FLAG = EXAM_FLAG_OFF;

					rt_printf("\n >>>> Success deleting CTRL_TASK. \n");

					rt_printf("\n ================================ Control Real-Time TASK Finished! \n");
				}
			}
			else {
				rt_printf("\n >>>> CTRL_TASK is already deleted or not create. \n");
			}

			break;

		case EXAM_CMD_EXIT :

			rt_printf("\n >> COMMAND_EXIT : %d \n", cmd.command);

			/* Device state file close */
			if (DISP_STAT_FLAG == EXAM_FLAG_ON) {

				rtn = fclose(dev_file_disp);
		
				if (rtn == -1) {
					errExit("fclose dev_file_disp %s", performCMD);
				}
				else {
					rt_printf("\n >>>> Success closing dev_file_disp. \n");
				}
			}
			else {
				rt_printf("\n >>>> dev_file_disp is already closed or not open. \n");
			}
	
			if (RECD_STAT_FLAG == EXAM_FLAG_ON) {

				rtn = fclose(dev_file_recd);
		
				if (rtn == -1) {
					errExit("fclose dev_file_recd %s", performCMD);
				}
				else {
					rt_printf("\n >>>> Success closing dev_file_recd. \n");
				}
			}
			else {
				rt_printf("\n >>>> dev_file_recd is already closed or not open. \n");
			}

			/* RT PIPE Deletion */
			rtn = rt_pipe_delete(&CMD_PIPE);
	
			if (rtn != 0) {
				rt_errExitEN(rtn, "rt_pipe_delete() <CMD_PIPE> performCMD");
			}
			else {
				rt_printf("\n >>>> Success deleting CMD_PIPE. \n");
			}

			if (DISP_STAT_FLAG == EXAM_FLAG_ON) {

				rtn = rt_pipe_delete(&DISP_PIPE);
	
				if (rtn != 0) {
					rt_errExitEN(rtn, "rt_pipe_delete() <DISP_PIPE> performCMD");
				}
				else {
					rt_printf("\n >>>> Success deleting DISP_PIPE. \n");
				}
			}
			else {
				rt_printf("\n >>>> DISP_PIPE is already deleted or not create. \n");
			}

			if (RECD_STAT_FLAG == EXAM_FLAG_ON) {

				rtn = rt_pipe_delete(&RECD_PIPE);
	
				if (rtn != 0) {
					rt_errExitEN(rtn, "rt_pipe_delete() <RECD_PIPE> performCMD");
				}
				else {
					rt_printf("\n >>>> Success deleting RECD_PIPE. \n");
				}
			}
			else {
				rt_printf("\n >>>> RECD_PIPE is already deleted or not create. \n");
			}
	
			/* RT TASK Deletion */
			if (CTRL_TASK_FLAG == EXAM_FLAG_ON) {

				rtn = rt_task_delete(&CTRL_TASK);
		
				if (rtn != 0) {
					rt_errExitEN(rtn, "rt_task_delete() <CTRL_TASK> performCMD");
				}
				else {
					rt_printf("\n >>>> Success deleting CTRL_TASK. \n");
					CTRL_TASK_FLAG = EXAM_FLAG_OFF;
				}
			}
			else {
				rt_printf("\n >>>> CTRL_TASK is already deleted or not create. \n");
			}

			rt_printf("\n ====================================================== Good Bye~!\n\n");

			exit(EXIT_SUCCESS);

			break;

		default :

			rt_printf("\n >> This command number is unregisterd. Please try again. \n");

			break;
	}
}


