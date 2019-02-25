#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

//#include <rtdk.h>

#include "manager.h"

int
main(void)
{
	int i, j;
	int rtn;

	/* Send command structure */
	cmd_t writeCMD;

	/* Initialize the command structure */
	writeCMD.command = EXAM_CMD_WAIT;

	for (i=0 ; i<S_NUM ; i++) {
		for (j=0 ; j<S_DOF ; j++) {
				writeCMD.bar[i].pst_t[j][BUF] 	= ABS[BUF];
				writeCMD.bar[i].q_t[j] 			= 0.0;
				writeCMD.bar[i].qdot_t[j] 		= 0.0;
				writeCMD.bar[i].curr_t[j] 		= 0.0;
				writeCMD.bar[i].torq_t[j] 		= 0.0;
		}
	}

	/* Get process pid */
	disp_pid = getPID("display");
	if (disp_pid == -1) {
		errExit("getPID disp_pid %s", main);
	}

	recd_pid = getPID("record");
	if (recd_pid == -1) {
		errExit("getPID recd_pid %s", main);
	}

	/* Open RT_PIPE file which was made in the Real-Time process */
	cmd_pipe = open("/dev/rtp0", O_WRONLY);
	if (cmd_pipe == -1) {
		errExit("open /dev/rtp0 %s", main);
	}

	printf("\n\n  # Open the command pipe... \n");

	while (1) {
		if (inputCMD(&writeCMD) < 0) break;
		if (sendCMD(&writeCMD)  < 0) break;
		if (writeCMD.command == EXAM_CMD_EXIT) {
			sleep(1);
			break;
		}
	}

	/* Close RT_PIPE file which was made in the Real-Time process */
	rtn = close(cmd_pipe);
	if (rtn == -1) {
		errExit("close /dev/rtp0 %s", main);
	}

	printf("\n  # Close the command pipe... \n");

	printf("\n =========================== Manager Process is finished. Good Bye~! \n\n");
	
	return 0;
}

static void
showMENU(void)
{
	printf("\n");
	printf(" ===================== STATUS & COMMAND MENU ======================= \n");
	printf("                                         Designed by Seyoung Cheon   \n");
	printf("                        belong to Team. LAMECH, OCT. 21. 2017, Sat   \n");
	printf("\n");
	printf(" == STATUS ========================================================= \n");
	printf("  ---------------------------------------[ Auto Set & Check Entry ]  \n");
	printf("   # IgH EtherCAT Master Module is ");
	if(M_FLAG_STAT_LOAD_EMM == EXAM_FLAG_ON)	printf("loaded.        \n");
	if(M_FLAG_STAT_LOAD_EMM == EXAM_FLAG_OFF)	printf("unloaed.       \n");
	printf("   # Node is ");
	if(M_FLAG_STAT_NODE == EXAM_FLAG_ON)		printf("checked.       \n");
	if(M_FLAG_STAT_NODE == EXAM_FLAG_OFF)		printf("not checked.   \n");
	printf("  ----------------------------------------------[ EtherCAT UNIQUE ]  \n");
	printf("   # Distribute Clock is ");
	if(M_FLAG_STAT_DIST_CLK == EXAM_FLAG_ON)	printf("operated.      \n");
	if(M_FLAG_STAT_DIST_CLK == EXAM_FLAG_OFF)	printf("not operated.  \n");
	printf("   # Redundancy is ");
	if(M_FLAG_STAT_RDNC == EXAM_FLAG_ON)		printf("activated.     \n");
	if(M_FLAG_STAT_RDNC == EXAM_FLAG_OFF)		printf("not activated. \n");
	printf("  -------------------------------------------------------[ States ]  \n");
	printf("   # Command Wait State  : ");
	if(M_FLAG_STAT_WAIT == EXAM_FLAG_ON)		printf("ON          \n");
	if(M_FLAG_STAT_WAIT == EXAM_FLAG_OFF)		printf("OFF         \n");
	printf("   # Display State       : ");
	if(M_FLAG_DISP_STAT == EXAM_FLAG_ON)		printf("ON          \n");
	if(M_FLAG_DISP_STAT == EXAM_FLAG_OFF)		printf("OFF         \n");
	printf("   # Recode State        : ");
	if(M_FLAG_RECD_STAT == EXAM_FLAG_ON)		printf("ON          \n");
	if(M_FLAG_RECD_STAT == EXAM_FLAG_OFF)		printf("OFF         \n");
	printf("  --------------------------------------------------[ Servo State ]  \n");
	printf("   # Slave's Servo Power : ");
	if(M_FLAG_SERV_STAT == EXAM_FLAG_ON)		printf("ON          \n");
	if(M_FLAG_SERV_STAT == EXAM_FLAG_OFF)		printf("OFF         \n");
	printf("  -------------------------------------------------[ Control Mode ]  \n");
	printf("   # Servo Control Mode  : ");
	if(M_FLAG_POS_MODE == EXAM_FLAG_ON && M_FLAG_VEL_MODE == EXAM_FLAG_OFF && 
	   M_FLAG_CUR_MODE == EXAM_FLAG_OFF)		printf("Position Mode \n");

	if(M_FLAG_VEL_MODE == EXAM_FLAG_ON && M_FLAG_POS_MODE == EXAM_FLAG_OFF &&
	   M_FLAG_CUR_MODE == EXAM_FLAG_OFF)		printf("Velocity Mode \n");

	if(M_FLAG_CUR_MODE == EXAM_FLAG_ON && M_FLAG_POS_MODE == EXAM_FLAG_OFF &&
	   M_FLAG_VEL_MODE == EXAM_FLAG_OFF)		printf("Current Mode  \n");

	if(M_FLAG_POS_MODE == EXAM_FLAG_OFF && M_FLAG_VEL_MODE == EXAM_FLAG_OFF && 
	   M_FLAG_CUR_MODE == EXAM_FLAG_OFF)		printf("Not Selected  \n");
	printf("  -------------------------------------------[ Task Control State ]  \n");
	printf("   # Task Space Control State : ");
	if(M_FLAG_CTRL_TASK == EXAM_FLAG_ON)	printf("ON          \n");
	if(M_FLAG_CTRL_TASK == EXAM_FLAG_OFF) 	printf("OFF         \n");
	printf("\n");
	printf(" == COMMAND ======================================================== \n");
	printf("  ---------------------------------------------------[ Initialize ]  \n");
	printf("  [01] ALL Command Stop                                              \n");
	printf("  [02] Check EtherCAT Module Load                                    \n");
	printf("  [03] Load EtherCAT Module          [04] Unload EtherCAT Module     \n");
	printf("\n");
	printf("  ------------------------------------------------[ Check & Reset ]  \n");
	printf("  [11] Check Slave's Node                                            \n");
	printf("  [12] Check Distribute Cloc         [13] Reset Distribute Clock     \n");
	printf("  [14] Check Redundancy Facility                                     \n");
	printf("  [15] Check ELMO Driver's State     [16] Reset ELMO Driver          \n");
	printf("  [17] Zero Angle Offset             [18] Zero Force/Torque Offset   \n");
	printf("\n");
	printf("  ---------------------------------------------[ Display & Recode ]  \n");
	printf("  [21] States Display On             [22] States Dispaly Off         \n");
	printf("  [23] States Recode On              [24] States Recode Off          \n");
	printf("\n");
	printf("  -------------------------------------------------[ Servo States ]  \n");
	printf("  [31] Slave's Servo On              [32] Slave's Servo Off          \n");
	printf("\n");
	printf("  -----------------------------------------------[ Mode Selection ]  \n");
	printf("  [41] Position Mode On              [42] Position Mode Off          \n");
	printf("  [43] Velocity Mdoe On              [44] Velocity Mode Off          \n");
	printf("  [45] Current Mode On               [46] Current Mode Off           \n");
	printf("\n");
	printf("  -------------------------------------------------[ Mode Control ]  \n");
	printf("  [51] Position Control On           [52] Position Control Off       \n");
	printf("  [53] Velocity Control On           [54] Velocity Control Off       \n");
	printf("  [55] Current Control On            [56] Current Control Off        \n");
	printf("  [57] Torque Control On             [58] Torque Control Off         \n"); 
	printf("\n");
	printf("  -------------------------------------------------[ Task Control ]  \n");
	printf("  [61] Control Task On               [62] Control Task Off           \n");
	printf("\n");
	/*
	additional command list
	printf(" ...... \n");
	*/
	printf("  -------------------------------------------------------[ Finish ]  \n");
	printf("  [999] Program Exit                                                 \n");
	printf("\n");
	printf(" == INPUT COMMAND ================================================== \n");
	printf("                            Watching your foot before touch the key! \n");
}

static int
inputCMD(cmd_t *cmd)
{
	int rtn;
	int num, i, j;
	char pst[BUF];
	double q, qdot, q2dot, curr, torq;

	showMENU();
		
	printf(" >>>> Insert Command : ");
	scanf("%d", &num);
		
	switch(num)
	{
		case 1 :
			cmd->command = EXAM_CMD_STOP;
			M_FLAG_DISP_STAT = EXAM_FLAG_OFF;
			M_FLAG_RECD_STAT = EXAM_FLAG_OFF;
			M_FLAG_CTRL_TASK = EXAM_FLAG_OFF;
			M_FLAG_SERV_STAT = EXAM_FLAG_OFF;
			break;

		case 2 :
			cmd->command = EXAM_CMD_EMM_LOAD_CHK;
			break;

		case 3 :
			cmd->command = EXAM_CMD_EMM_LOAD;
			M_FLAG_STAT_LOAD_EMM = EXAM_FLAG_ON;
			break;

		case 4 :
			cmd->command = EXAM_CMD_EMM_UNLOAD;
			M_FLAG_STAT_LOAD_EMM = EXAM_FLAG_OFF;
			break;

		/*
		case 5 ~ 10 :
			cmd->command = EXAM_CMD_XXXX_XXXX;
			M_FLAG_XXXX_XXXX = EXAM_FLAG_ON; (or EXAM_FLAG_OFF;)
			...
			break;
		*/

		case 11 :
			cmd->command = EXAM_CMD_NODE_CHK;
			//M_FLAG_STAT_NODE = EXAM_FLAG_ON; (or EXAM_FLAG_OFF;)
			break;

		case 12 :
			cmd->command = EXAM_CMD_DIST_CLK_CHK;
			//M_FLAG_STAT_DIST_CLK = EXAM_FLAG_ON; (or EXAM_FLAG_OFF;)
			break;

		case 13 :
			cmd->command = EXAM_CMD_DIST_CLK_SET;
			break;

		case 14 :
			cmd->command = EXAM_CMD_RDNC_CHK;
			//M_FLAG_STAT_RDNC = EXAM_FLAG_ON; (or EXAM_FLAG_OFF;)
			break;

		case 15 :
			cmd->command = EXAM_CMD_ELMO_STATE;
			//M_FLAG_STAT_ELMO = EXAM_FLAG_ON; (or EXAM_FLAG_OFF;)
			break;

		case 16 :
			cmd->command = EXAM_CMD_ELMO_RESET;
			break;

		case 17 :
			cmd->command = EXAM_CMD_ANGL_OFFSET;
			break;

		case 18 :
			cmd->command = EXAM_CMD_TORQ_OFFSET;
			break;

		/*
		case 19 ~ 20 :
			cmd->command = EXAM_CMD_XXXX_XXXX;
			M_FLAG_XXXX_XXXX = EXAM_FLAG_ON; (or EXAM_FLAG_OFF;)
			...
			break;
		*/

		case 21 :
			cmd->command = EXAM_CMD_DISP_ON;

			if (M_FLAG_DISP_STAT == EXAM_FLAG_OFF) {
				rtn = sendSGN(disp_pid, 10);	// signal number 10 : SIGUSR1
				if (rtn == -1) {
					errMsg("EXAM_CMD_DISP_ON %s", inputCMD);
					return rtn;
				}
				else {
					M_FLAG_DISP_STAT = EXAM_FLAG_ON;
				}
			}
			else {
				printf("\n # disp_pthread is already start or not stop... \n");
			}

			break;

		case 22 :
			cmd->command = EXAM_CMD_DISP_OFF;

			if (M_FLAG_DISP_STAT == EXAM_FLAG_ON) {
				rtn = sendSGN(disp_pid, 34);	// signal number 34 : SIGRTMIN
				if (rtn == -1) {
					errMsg("EXAM_CMD_DISP_OFF %s", inputCMD);
					return rtn;
				}
				else {
					M_FLAG_DISP_STAT = EXAM_FLAG_OFF;
				}
			}
			else {
				printf("\n # disp_pthread is already stop or not start... \n");
			}

			break;

		case 23 :
			cmd->command = EXAM_CMD_RECD_ON;
			
			if (M_FLAG_RECD_STAT == EXAM_FLAG_OFF) {
				rtn = sendSGN(recd_pid, 12);	// signal number 12 : SIGUSR2
				if (rtn == -1) {
					errMsg("EXAM_CMD_RECD_ON %s", inputCMD);
					return rtn;
				}
				else {
					M_FLAG_RECD_STAT = EXAM_FLAG_ON;
				}
			}
			else {
				printf("\n  # recd_pthread is already start or not stop... \n");
			}

			break;

		case 24 :
			cmd->command = EXAM_CMD_RECD_OFF;

			if (M_FLAG_RECD_STAT == EXAM_FLAG_ON) {
				rtn = sendSGN(recd_pid, 35);	// signal number 35 : SIGRTMIN+1
				if (rtn == -1) {
					errMsg("EXAM_CMD_RECD_OFF %s", inputCMD);
					return rtn;
				}
				else {
					M_FLAG_RECD_STAT = EXAM_FLAG_OFF;
				}
			}
			else {
				printf("\n  # recd_pthread is already stop or not start... \n");
			}

			break;

		/*
		case 25 ~ 30 :
			cmd->command = EXAM_CMD_XXXX_XXXX;
			M_FLAG_XXXX_XXXX = EXAM_FLAG_ON; (or EXAM_FLAG_OFF;)
			...
			break;
		*/

		case 31 :
			cmd->command = EXAM_CMD_SERVO_ON;
			M_FLAG_SERV_STAT = EXAM_FLAG_ON;
			break;

		case 32 :
			cmd->command = EXAM_CMD_SERVO_OFF;
			M_FLAG_SERV_STAT = EXAM_FLAG_OFF;
			break;

		/*
		case 33 ~ 40 :
			cmd->command = EXAM_CMD_XXXX_XXXX;
			M_FLAG_XXXX_XXXX = EXAM_FLAG_ON; (or EXAM_FLAG_OFF;)
			...
			break;
		*/

		case 41 :
			cmd->command = EXAM_CMD_POS_MODE_ON;
			M_FLAG_POS_MODE = EXAM_FLAG_ON;
			break;

		case 42 :
			cmd->command = EXAM_CMD_POS_MODE_OFF;
			M_FLAG_POS_MODE = EXAM_FLAG_OFF;
			break;

		case 43 :
			cmd->command = EXAM_CMD_VEL_MODE_ON;
			M_FLAG_VEL_MODE = EXAM_FLAG_ON;
			break;

		case 44 :
			cmd->command = EXAM_CMD_VEL_MODE_OFF;
			M_FLAG_VEL_MODE = EXAM_FLAG_OFF;
			break;

		case 45 :
			cmd->command = EXAM_CMD_CUR_MODE_ON;
			M_FLAG_CUR_MODE = EXAM_FLAG_ON;
			break;

		case 46 :
			cmd->command = EXAM_CMD_CUR_MODE_OFF;
			M_FLAG_CUR_MODE = EXAM_FLAG_OFF;
			break;

		/*
		case 47 ~ 50 :
			cmd->command = EXAM_CMD_XXXX_XXXX;
			M_FLAG_XXXX_XXXX = EXAM_FLAG_ON; (or EXAM_FLAG_OFF;)
			...
			break;
		*/

		case 51 :
			cmd->command = EXAM_CMD_POS_CTRL_ON;
			break;

		case 52 :
			cmd->command = EXAM_CMD_POS_CTRL_OFF;
			break;

		case 53 :
			cmd->command = EXAM_CMD_VEL_CTRL_ON;
			break;

		case 54 :
			cmd->command = EXAM_CMD_VEL_CTRL_OFF;
			break;

		case 55 :
			cmd->command = EXAM_CMD_CUR_CTRL_ON;
			break;

		case 56 :
			cmd->command = EXAM_CMD_CUR_CTRL_OFF;
			break;

		case 57 :
			cmd->command = EXAM_CMD_TRQ_CTRL_ON;
			break;

		case 58 :
			cmd->command = EXAM_CMD_TRQ_CTRL_OFF;
			break;

		/*
		case 59 ~ 60 :
			cmd->command = EXAM_CMD_XXXX_XXXX;
			M_FLAG_XXXX_XXXX = EXAM_FLAG_ON; (or EXAM_FLAG_OFF;)
			...
			break;
		*/

		case 61 :
			cmd->command = EXAM_CMD_CTRL_TASK_ON;
			M_FLAG_CTRL_TASK = EXAM_FLAG_ON;
			break;

		case 62 :
			cmd->command = EXAM_CMD_CTRL_TASK_OFF;
			M_FLAG_CTRL_TASK = EXAM_FLAG_OFF;
			break;

		/*
		case 63 ~ 70 :
			cmd->command = EXAM_CMD_XXXX_XXXX;
			M_FLAG_XXXX_XXXX = EXAM_FLAG_ON; (or EXAM_FLAG_OFF;)
			...
			break;
		*/

		case 999 :
			cmd->command = EXAM_CMD_EXIT;
			
			if (M_FLAG_DISP_STAT == EXAM_FLAG_ON) {
				rtn = sendSGN(disp_pid, 34);	// signal number 34 : SIGRTMIN
				if (rtn == -1) {
					errMsg("EXAM_CMD_EXIT %s", inputCMD);
					return rtn;
				}
				else {
					M_FLAG_DISP_STAT = EXAM_FLAG_OFF;
				}
			}
			else {
				printf("\n # disp_pthread is already stop or not start... \n");
			}

			if (M_FLAG_RECD_STAT == EXAM_FLAG_ON) {
				rtn = sendSGN(recd_pid, 35);	// signal number 35 : SIGRTMIN+1
				if (rtn == -1) {
					errMsg("EXAM_CMD_EXIT %s", inputCMD);
					return rtn;
				}
				else {
					M_FLAG_RECD_STAT = EXAM_FLAG_OFF;
				}
			}
			else {
				printf("\n  # recd_pthread is already stop or not start... \n");
			}

			M_FLAG_EXIT = EXAM_FLAG_ON;

			break;

		default :
			printf("\n !! This command number is not registered. \n");
			break;
	}

	if (cmd->command == EXAM_CMD_POS_CTRL_ON) {
		printf(" Additional Position Information is required! \n");
		printf(" (ex) REL 15.55[deg] 5.55[deg/s] >> "); // Each joint moves for 3.11 seconds. 
		scanf("%s %lf %lf", pst, &q, &qdot);
		for (i=0 ; i<S_NUM ; i++) {
			for (j=0 ; j<S_DOF ; j++) {
				cmd->bar[i].pst_t[j][BUF] = pst[BUF];
				cmd->bar[i].q_t[j] 		= q;
				cmd->bar[i].qdot_t[j] 	= qdot;
				cmd->bar[i].q2dot_t[j] 	= 0.0;
				cmd->bar[i].curr_t[j] 	= 0.0;
				cmd->bar[i].torq_t[j] 	= 0.0;
			}
		}
	}

	if (cmd->command == EXAM_CMD_VEL_CTRL_ON) {
		printf(" Additional Velocity Information is required! \n");
		printf(" (ex) 5.00[deg/s] 2.50[deg/s^2] >> "); // Each joint reaches 5[deg/s] in 2 senconds. 
		scanf("%lf %lf", &qdot, &q2dot);
		for (i=0 ; i<S_NUM ; i++) {
			for (j=0 ; j<S_DOF ; j++) {
				cmd->bar[i].pst_t[j][BUF] 	= ABS[BUF];
				cmd->bar[i].q_t[j] 			= 0.0;
				cmd->bar[i].qdot_t[j] 		= qdot;
				cmd->bar[i].q2dot_t[j] 		= q2dot;
				cmd->bar[i].curr_t[j] 		= 0.0;
				cmd->bar[i].torq_t[j] 		= 0.0;
			}
		}
	}

	if (cmd->command == EXAM_CMD_CUR_CTRL_ON) {
		printf(" Additional Current Information is required! \n");
		printf(" (ex) 1.65[A] >> "); // Each joint motor is moved under 2A. 
		scanf("%lf", &curr);
		for (i=0 ; i<S_NUM ; i++) {
			for (j=0 ; j<S_DOF ; j++) {
				cmd->bar[i].pst_t[j][BUF] 	= ABS[BUF];
				cmd->bar[i].q_t[j] 			= 0.0;
				cmd->bar[i].qdot_t[j] 		= 0.0;
				cmd->bar[i].q2dot_t[j] 		= 0.0;
				cmd->bar[i].curr_t[j] 		= curr;
				cmd->bar[i].torq_t[j] 		= 0.0;
			}
		}
	}

	if (cmd->command == EXAM_CMD_TRQ_CTRL_ON) {
		printf(" Additional Torque Information is required! \n");
		printf(" (ex) 2.13[Nm] >> "); // Each joint motor is moved under 2.13 Nm. 
		scanf("%lf", &torq);
		for (i=0 ; i<S_NUM ; i++) {
			for (j=0 ; j<S_DOF ; j++) {
				cmd->bar[i].pst_t[j][BUF] 	= ABS[BUF];
				cmd->bar[i].q_t[j] 			= 0.0;
				cmd->bar[i].qdot_t[j] 		= 0.0;
				cmd->bar[i].q2dot_t[j] 		= 0.0;
				cmd->bar[i].curr_t[j] 		= 0.0;
				cmd->bar[i].torq_t[j] 		= torq;
			}
		}
	}

	return 0;
}

static int
sendCMD(cmd_t *cmd)
{
	int rtn;

	rtn = write(cmd_pipe, cmd, sizeof(cmd_t));
	if (rtn == -1) {
		errMsg("write %s", sendCMD);
		return rtn;
	}

	return rtn;
}

static pid_t
getPID(const char *pname)
{
	DIR *dp;
	FILE *fp;
	struct dirent *dir;
	char buf[100], line[1024], tag[100], name[100];
	pid_t pid;

	dp = opendir("/proc");
	if (dp == NULL) {
		errExit("opendir %s", getPID);
	}
	else {
		while ((dir = readdir(dp)) != NULL)
		{
			pid = atoi(dir->d_name);
			
			snprintf(buf, 100, "/proc/%d/status", pid);

			fp = fopen(buf, "r");
			if (fp == NULL) continue;

			fgets(line, 1024, fp);

			fclose(fp);

			sscanf(line, "%s %s", tag, name);

			if (!strcmp(name, pname)) {
				closedir(dp);
				return pid;
			}
		}
	}

	closedir(dp);

	return -1;
}

static int
sendSGN(const pid_t pid, const int signum)
{
	int rtn;
/*	pid_t pid;

	pid = getPID(pname);
	if (pid == -1) {
		errMsg("getPID %s", sendSGN);
		return pid;
	}
*/
	rtn = kill(pid, signum);
	if (rtn == -1) {
		errMsg("kill %s", sendSGN);
		return rtn;
	}

	return rtn;
}


