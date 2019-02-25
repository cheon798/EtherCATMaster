#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

/* Degree Of Freedom */
#define S_NUM	2	// Number of stick
#define S_DOF	2	// Operated stick DOF

/* Posture Buffer Size */
#define BUF		4	// because "ABS","REL"

/* FLAG */
enum EXAM_FLAG {
	EXAM_FLAG_OFF = 0,
	EXAM_FLAG_ON  = 1
};

/* COMMAND */
enum EXAM_CMD {
	// Standby Status
	EXAM_CMD_WAIT			= 0,
	EXAM_CMD_STOP			= 1,
	EXAM_CMD_EMM_LOAD_CHK	= 2,
	EXAM_CMD_EMM_LOAD		= 3,
	EXAM_CMD_EMM_UNLOAD		= 4,
	// Check & Reset
	EXAM_CMD_NODE_CHK		= 11,
	EXAM_CMD_DIST_CLK_CHK	= 12,
	EXAM_CMD_DIST_CLK_SET	= 13,
	EXAM_CMD_RDNC_CHK		= 14,
	EXAM_CMD_ELMO_STATE		= 15,
	EXAM_CMD_ELMO_RESET		= 16,
	EXAM_CMD_ANGL_OFFSET	= 17,
	EXAM_CMD_TORQ_OFFSET	= 18,
	// Display & Record
	EXAM_CMD_DISP_ON		= 21,
	EXAM_CMD_DISP_OFF		= 22,
	EXAM_CMD_RECD_ON		= 23,
	EXAM_CMD_RECD_OFF		= 24,
	// Servo Status
	EXAM_CMD_SERVO_ON		= 31,
	EXAM_CMD_SERVO_OFF		= 32,
	// Control Mode Select
	EXAM_CMD_POS_MODE_ON	= 41,
	EXAM_CMD_POS_MODE_OFF	= 42,
	EXAM_CMD_VEL_MODE_ON	= 43,
	EXAM_CMD_VEL_MODE_OFF	= 44,
	EXAM_CMD_CUR_MODE_ON	= 45,
	EXAM_CMD_CUR_MODE_OFF	= 46,
	// Control in Selected Mode
	EXAM_CMD_POS_CTRL_ON	= 51,
	EXAM_CMD_POS_CTRL_OFF	= 52,
	EXAM_CMD_VEL_CTRL_ON	= 53,
	EXAM_CMD_VEL_CTRL_OFF	= 54,
	EXAM_CMD_CUR_CTRL_ON	= 55,
	EXAM_CMD_CUR_CTRL_OFF	= 56,
	EXAM_CMD_TRQ_CTRL_ON	= 57,
	EXAM_CMD_TRQ_CTRL_OFF	= 58,
	// Control TASK
	EXAM_CMD_CTRL_TASK_ON	= 61,
	EXAM_CMD_CTRL_TASK_OFF	= 62,
	// Exit Program
	EXAM_CMD_EXIT			= 999
};

/* System Target Joint Structure 
   (Control in Selected Mode : 51 ~ 58) */
const char ABS[BUF] = "ABS";
const char REL[BUF] = "REL";

typedef struct _bar {
	char   pst_t[S_DOF][BUF];	// posture:'ABS'||'REL'
	double q_t[S_DOF];			// Target Angle
	double qdot_t[S_DOF];		// Target Velocity
	double q2dot_t[S_DOF];		// Target Acceleration
	double curr_t[S_DOF];		// Target Current
	double torq_t[S_DOF];		// Target Torque
} bar_t;

/* System Command Structure */
typedef struct _cmd {
	enum EXAM_CMD command;
	bar_t bar[S_NUM];
} cmd_t;

/* System Joint Structure */
typedef struct _stick {
	double q_d[S_DOF];			// Desired Angle
	double q_a[S_DOF];			// Actual Angle
	double qdot_d[S_DOF];		// Desired Velocity
	double qdot_a[S_DOF];		// Actual Velociy
	double q2dot_d[S_DOF];		// Desired Acceleration
	double q2dot_a[S_DOF];		// Actual Acceleration
	double curr_d[S_DOF];		// Desired Current
	double curr_a[S_DOF];		// Actual Current
	double torq_d[S_DOF];		// Desired Torque
	double torq_a[S_DOF];		// Actual Torque
} stick_t;

/* System Data Structure */
typedef struct _data {
	stick_t stick[S_NUM];
} data_t;

#endif /*_PARAMETERS_H_*/
