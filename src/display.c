#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>

//#include <rtdk.h>

#include "display.h"

int
main(void)
{
	int rtn;
	struct sigaction act1, act2;

	memset(&act1, '\0', sizeof(act1));
	memset(&act2, '\0', sizeof(act2));

	act1.sa_handler = &createDISP_pthread;
	act2.sa_handler = &cancleDISP_pthread;

	/* Open RT_PIPE file which was made in the Real-Time process */
	disp_pipe = open("/dev/rtp1", O_RDONLY);
	if (disp_pipe == -1) {
		errExit("open /dev/rtp1 %s", main);
	}

	printf("\n @ Open the display pipe... \n");

	while(1) {
		rtn = sigaction(SIGUSR1, &act1, NULL);
		if (rtn == -1) {
			errMsg("sigaction %s", main);
			break;
		}

		rtn = sigaction(SIGRTMIN, &act2, NULL); 
		if (rtn == -1) {
			errMsg("sigaction %s", main);
			break;
		}
	}

	/* Close RT_PIPE file which was made in the Real-Time process */
	rtn = close(disp_pipe);
	if (rtn == -1) {
		errExit("close /dev/rtp1 %s", main);
	}
	printf("\n @ Close the display pipe... \n");

	printf("\n =========================== Display Process is finished. Good Bye~! \n\n");

	return 0;
}


static void
displayDATA(const data_t *data)
{
	//nanosleep(tv_nsec);

	printf("\n %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf \n",
			data->stick[0].q_d[0], data->stick[0].q_d[1], 
			data->stick[0].q_a[0], data->stick[0].q_a[1], 
			data->stick[0].qdot_d[0], data->stick[0].qdot_d[1], 
			data->stick[0].qdot_a[0], data->stick[0].qdot_a[1], 
			data->stick[0].q2dot_d[0], data->stick[0].q2dot_d[1], 
			data->stick[0].q2dot_a[0], data->stick[0].q2dot_a[1],
			data->stick[0].curr_d[0], data->stick[0].curr_d[1],
			data->stick[0].curr_a[0], data->stick[0].curr_a[1],
			data->stick[0].torq_d[0], data->stick[0].torq_d[1],
			data->stick[0].torq_a[0], data->stick[0].torq_a[1],
			data->stick[1].q_d[0], data->stick[1].q_d[1], 
			data->stick[1].q_a[0], data->stick[1].q_a[1], 
			data->stick[1].qdot_d[0], data->stick[1].qdot_d[1], 
			data->stick[1].qdot_a[0], data->stick[1].qdot_a[1], 
			data->stick[1].q2dot_d[0], data->stick[1].q2dot_d[1], 
			data->stick[1].q2dot_a[0], data->stick[1].q2dot_a[1],
			data->stick[1].curr_d[0], data->stick[1].curr_d[1],
			data->stick[1].curr_a[0], data->stick[1].curr_a[1],
			data->stick[1].torq_d[0], data->stick[1].torq_d[1],
			data->stick[1].torq_a[0], data->stick[1].torq_a[1]);

	/*printf("\n");
	printf(" @ Hanlder Stick States : \n");
	printf("  -Actual Horizontal Angle      : %8.4lf, -Actual Vertical Angle      : %8.4lf \n",
				data->stick[0].q_a[0], data->stick[0].q_a[1]);
	printf("  -Actual Horizontal Velocity   : %8.4lf, -Actual Vertical Velocity   : %8.4lf \n",
				data->stick[0].qdot_a[0], data->stick[0].qdot_a[1]);
	printf("  -Actual Horizontal Current    : %8.4lf, -Actual Vertical Current    : %8.4lf \n",
				data->stick[0].curr_a[0], data->stick[0].curr_a[1]);
	printf("  -Actual Horizontal Torque     : %8.4lf, -Actual Vertical Torque     : %8.4lf \n",
				data->stick[0].torq_a[0], data->stick[0].torq_a[1]);
	printf("\n");
	printf(" @ Operated Stick States : \n");
	printf("  -Designed Horizontal Angle    : %8.4lf, -Designed Vertical Angle    : %8.4lf \n",
				data->stick[1].q_d[0], data->stick[1].q_d[1]);
	printf("  -Actual Horizontal Angle      : %8.4lf, -Actual Vertical Angle      : %8.4lf \n",
				data->stick[1].q_a[0], data->stick[1].q_a[1]);
	printf("  -Designed Horizontal Velocity : %8.4lf, -Designed Vertical Velocity : %8.4lf \n",
				data->stick[1].q_d[0], data->stick[1].q_d[1]);
	printf("  -Actual Horizontal Velocity   : %8.4lf, -Actual Vertical Velocity   : %8.4lf \n",
				data->stick[1].q_a[0], data->stick[1].q_a[1]);
	printf("  -Designed Horizontal Current  : %8.4lf, -Designed Vertical Current  : %8.4lf \n",
				data->stick[1].curr_d[0], data->stick[1].curr_d[1]);
	printf("  -Actual Horizontal Current    : %8.4lf, -Actual Vertical Current    : %8.4lf \n",
				data->stick[1].curr_a[0], data->stick[1].curr_a[1]);
	printf("  -Designed Horizontal Torque   : %8.4lf, -Designed Vertical Torque   : %8.4lf \n",
				data->stick[1].torq_d[0], data->stick[1].torq_d[1]);
	printf("  -Actual Horizontal Torque     : %8.4lf, -Actual Vertical Torque     : %8.4lf \n",
				data->stick[1].torq_a[0], data->stick[1].torq_a[1]);
	printf("\n");
*/
}

static void
*dispDATA_routine(void *arg)
{
	int i, j;
	int rtn;
//	struct timespec ts;

//	ts.tv_sec = 0;
//	ts.tv_nsec = 100000000;

	data_t dispDATA;
	
	for (i = 0 ; i < S_NUM ; i++) {
		for (j = 0 ; j < S_DOF ; j++) {
			dispDATA.stick[i].q_d[j] 	= 0.0;
			dispDATA.stick[i].q_a[j] 	= 0.0;
			dispDATA.stick[i].qdot_d[j] = 0.0;
			dispDATA.stick[i].qdot_a[j] = 0.0;
			dispDATA.stick[i].curr_d[j]	= 0.0;
			dispDATA.stick[i].curr_a[j]	= 0.0;
			dispDATA.stick[i].torq_d[j]	= 0.0;
			dispDATA.stick[i].torq_a[j]	= 0.0;
		}
	}

	while (1) {
		rtn = read(disp_pipe, &dispDATA, sizeof(data_t));
		if (rtn == -1) {
			errExit("read %s", dispDATA_routine);
		}
		else {
			//nanosleep(&ts, &ts);
			displayDATA(&dispDATA);
		}
	}
}

static void
createDISP_pthread(void)
{
	int rtn;

	rtn = pthread_create(&disp_pthread, NULL, dispDATA_routine, (void*)NULL);
	if (rtn != 0) {
		errExitEN(rtn, "pthread_create");
	}
	else {
		printf("\n @ Create the display pthread... \n");
	}
}

static void
cancleDISP_pthread(void)
{
	int rtn;
	
	rtn = pthread_cancel(disp_pthread);
	if (rtn != 0) {
		errExitEN(rtn, "pthread_cancle");
	}
	else {
		printf("\n @ Cancle the display pthread... \n");
	}
}

