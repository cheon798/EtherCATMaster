#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

//#include <rtdk.h>

#include "record.h"

unsigned int itr;

int
main(void)
{
	int rtn;
	struct sigaction act1, act2;

	memset(&act1, '\0', sizeof(act1));
	memset(&act2, '\0', sizeof(act2));

	act1.sa_handler = &createRECD_pthread;
	act2.sa_handler = &cancleRECD_pthread;

	/* Open RT_PIPE file which was made in the Real-Time process */
	recd_pipe = open("/dev/rtp2", O_RDONLY);
	if (recd_pipe == -1) {
		errExit("open /dev/rtp2 %s", main);
	}

	printf("\n @ Open the record pipe... \n");

	while(1) {
		rtn = sigaction(SIGUSR2, &act1, NULL);
		if (rtn == -1) {
			errMsg("sigaction %s", main);
			break;
		}

		rtn = sigaction(SIGRTMIN+1, &act2, NULL);
		if (rtn == -1) {
			errMsg("sigaction %s", main);
			break;
		}
	}

	/* Close RT_PIPE file which was made in the Real-Time process */
	rtn = close(recd_pipe);
	if (rtn == -1) {
		errExit("close /dev/rtp2 %s", main);
	}
	printf("\n  @ Close the record pipe... \n");

	printf("\n =========================== Record Process is finished. Good Bye~! \n\n");

	return 0;
}


static void		// Need to modify fprintf : 40 ea --> 2 ea
recordDATA(FILE *file, const data_t *data)
{
	fprintf(file, "The data of the %d th cycle is as follows. \n", itr);

	fprintf(file, " %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf \n",
			data->stick[0].q_d[0], 		data->stick[0].q_a[0], 
			data->stick[0].qdot_d[0], 	data->stick[0].qdot_a[0],
			data->stick[0].q2dot_d[0], 	data->stick[0].q2dot_a[0],
			data->stick[0].curr_d[0], 	data->stick[0].curr_a[0],
			data->stick[0].torq_d[0], 	data->stick[0].torq_a[0]);

	fprintf(file, " %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf \n",
			data->stick[0].q_d[1], 		data->stick[0].q_a[1], 
			data->stick[0].qdot_d[1], 	data->stick[0].qdot_a[1],
			data->stick[0].q2dot_d[1], 	data->stick[0].q2dot_a[1],
			data->stick[0].curr_d[1], 	data->stick[0].curr_a[1],
			data->stick[0].torq_d[1], 	data->stick[0].torq_a[1]);

	fprintf(file, " %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf \n",
			data->stick[1].q_d[0], 		data->stick[1].q_a[0], 
			data->stick[1].qdot_d[0], 	data->stick[1].qdot_a[0],
			data->stick[1].q2dot_d[0], 	data->stick[1].q2dot_a[0],
			data->stick[1].curr_d[0], 	data->stick[1].curr_a[0],
			data->stick[1].torq_d[0], 	data->stick[1].torq_a[0]);

	fprintf(file, " %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf %7.4lf \n",
			data->stick[1].q_d[1], 		data->stick[1].q_a[1], 
			data->stick[1].qdot_d[1], 	data->stick[1].qdot_a[1],
			data->stick[1].q2dot_d[1], 	data->stick[1].q2dot_a[1],
			data->stick[1].curr_d[1], 	data->stick[1].curr_a[1],
			data->stick[1].torq_d[1], 	data->stick[1].torq_a[1]);

	fprintf(file, "\n");

	itr++;

		/*fprintf(file, "%7.4lf ", data->stick[0].q_d[0]);
		fprintf(file, "%7.4lf ", data->stick[0].q_d[1]);
		fprintf(file, "%7.4lf ", data->stick[0].q_a[0]);
		fprintf(file, "%7.4lf ", data->stick[0].q_a[1]);
		fprintf(file, "%7.4lf ", data->stick[0].qdot_d[0]);
		fprintf(file, "%7.4lf ", data->stick[0].qdot_d[1]);
		fprintf(file, "%7.4lf ", data->stick[0].qdot_a[0]);
		fprintf(file, "%7.4lf ", data->stick[0].qdot_a[1]);
		fprintf(file, "%7.4lf ", data->stick[0].q2dot_d[0]);
		fprintf(file, "%7.4lf ", data->stick[0].q2dot_d[1]);
		fprintf(file, "%7.4lf ", data->stick[0].q2dot_a[0]);
		fprintf(file, "%7.4lf ", data->stick[0].q2dot_a[1]);
		fprintf(file, "%7.4lf ", data->stick[0].curr_d[0]);
		fprintf(file, "%7.4lf ", data->stick[0].curr_d[1]);
		fprintf(file, "%7.4lf ", data->stick[0].curr_a[0]);
		fprintf(file, "%7.4lf ", data->stick[0].curr_a[1]);
		fprintf(file, "%7.4lf ", data->stick[0].torq_d[0]);
		fprintf(file, "%7.4lf ", data->stick[0].torq_d[1]);
		fprintf(file, "%7.4lf ", data->stick[0].torq_a[0]);
		fprintf(file, "%7.4lf ", data->stick[0].torq_a[1]);
		fprintf(file, "\n");
		fprintf(file, "%7.4lf ", data->stick[1].q_d[0]);
		fprintf(file, "%7.4lf ", data->stick[1].q_d[1]);
		fprintf(file, "%7.4lf ", data->stick[1].q_a[0]);
		fprintf(file, "%7.4lf ", data->stick[1].q_a[1]);
		fprintf(file, "%7.4lf ", data->stick[1].qdot_d[0]);
		fprintf(file, "%7.4lf ", data->stick[1].qdot_d[1]);
		fprintf(file, "%7.4lf ", data->stick[1].qdot_a[0]);
		fprintf(file, "%7.4lf ", data->stick[1].qdot_a[1]);
		fprintf(file, "%7.4lf ", data->stick[1].q2dot_d[0]);
		fprintf(file, "%7.4lf ", data->stick[1].q2dot_d[1]);
		fprintf(file, "%7.4lf ", data->stick[1].q2dot_a[0]);
		fprintf(file, "%7.4lf ", data->stick[1].q2dot_a[1]);
		fprintf(file, "%7.4lf ", data->stick[1].curr_d[0]);
		fprintf(file, "%7.4lf ", data->stick[1].curr_d[1]);
		fprintf(file, "%7.4lf ", data->stick[1].curr_a[0]);
		fprintf(file, "%7.4lf ", data->stick[1].curr_a[1]);
		fprintf(file, "%7.4lf ", data->stick[1].torq_d[0]);
		fprintf(file, "%7.4lf ", data->stick[1].torq_d[1]);
		fprintf(file, "%7.4lf ", data->stick[1].torq_a[0]);
		fprintf(file, "%7.4lf ", data->stick[1].torq_a[1]);
		fprintf(file, "\n");*/
}

static void
*recdDATA_routine(void *arg)
{
	int i, j;
	int rtn;

	data_t recdDATA;
	
	for (i = 0 ; i < S_NUM ; i++) {
		for (j = 0 ; j < S_DOF ; j++) {
			recdDATA.stick[i].q_d[j] 	= 0.0;
			recdDATA.stick[i].q_a[j] 	= 0.0;
			recdDATA.stick[i].qdot_d[j] = 0.0;
			recdDATA.stick[i].qdot_a[j] = 0.0;
			recdDATA.stick[i].curr_d[j] = 0.0;
			recdDATA.stick[i].curr_a[j] = 0.0;
			recdDATA.stick[i].torq_d[j] = 0.0;
			recdDATA.stick[i].torq_a[j] = 0.0;
		}
	}

	while (1) {
		rtn = read(recd_pipe, &recdDATA, sizeof(data_t));
		if (rtn == -1) {
			errExit("read %s", recdDATA_routine);
		}
		else {
			recordDATA(recd_file, &recdDATA);
		}
	}
}

static void
createRECD_pthread(void)
{
	int rtn;

	recd_file = fopen("./data/recording_file.txt", "w");
	if (recd_file == NULL) {
		errExit("fopen %s", createRECD_pthread);
	}
	else {
		printf("\n @ Open the record file... \n");
	}

	rtn = pthread_create(&recd_pthread, NULL, recdDATA_routine, (void*)NULL);
	if (rtn != 0) {
		fclose(recd_file);
		errExitEN(rtn, "pthread_create");
	}
	else {
		printf("\n @ Create the record pthread... \n");
	}

}

static void
cancleRECD_pthread(void)
{
	int rtn;
	
	rtn = pthread_cancel(recd_pthread);
	if (rtn != 0) {
		errExitEN(rtn, "pthread_cancle");
	}
	else {
		printf("\n @ Cancle the record pthread... \n");
		rtn = fclose(recd_file);
		if (rtn == EOF) {
			errExit("fclose %s", cancleRECD_pthread);
		}
		else {
			printf("\n @ Close the record file... \n");
		}
	}
}

