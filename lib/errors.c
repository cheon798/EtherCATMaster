#include "errors.h"

#ifdef __GNUC__
__attribute__ ((__noreturn__))
#endif

static void
terminate(Boolean useExit3)
{
	char *s;

	s = getenv("EF_DUMPCORE");

	if (s != NULL && *s != '\0') 
		abort();
	else if (useExit3)			// TRUE
		exit(EXIT_FAILURE);		// library exit function, return EXIT_FAILURE to parent process
	else 						// FALSE
		_exit(EXIT_FAILURE);	// system call function, return EXIT_FAILURE to parent process
}

static void
outputError(Boolean usrErr,			// TRUE or FALSE
			int err,				// error number
			Boolean flushStdout,	// for flusing stdout buffer
			const char *format,		// function name
			va_list ap)
{
	#define BUF_SIZE 500

	char buf[BUF_SIZE],
		 userMsg[BUF_SIZE],
		 errText[BUF_SIZE];

	// userMsg[BUF_SIZE] <-- format
	vsnprintf(userMsg, BUF_SIZE, format, ap);

	// errText[BUF_SIZE] <-- ename[err](or ?UNKNOWN?), strerror(err)
	if (usrErr) 
		snprintf(errText,
				 BUF_SIZE,
				 " [%s %s]",
				 (err > 0 && err <= MAX_ENAME) ? ename[err] : "?UNKNOWN?",
				 strerror(err));
	else
		snprintf(errText, BUF_SIZE, ":");

	// buf[BUF_SIZE] <-- errText, userMsg
	snprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, userMsg);

	if (flushStdout)
		fflush(stdout);
	
	fputs(buf, stderr);

	fflush(stderr);
}

void
errMsg(const char *format, ...)
{
	va_list argList;
	int savedErrno;

	savedErrno = errno;

	va_start(argList, format);

	outputError(TRUE,		// outputError(Boolean usrErr,
				errno,		// 			   int err,
				TRUE,		//			   Boolean flushStdout	 
				format,		//			   const char *format
				argList);	//			   va_list ap_;
	/*
	vsnprintf(userMsg, 500, format, argList);

	snprintf(errText,
			 500,
			 " [%s %s]",
			 (errno > 0 && errno <= MAX_ENAME) ? ename[errno] : "?UNKNOWN?",
			 strerror(errno));

	snprintf(buf, 500, "ERROR%s %s\n", errText, userMsg);
	fflush(stdout);

	fput(buf, stderr);
	fflush(stderr);
	*/

	va_end(argList);

	errno = savedErrno;
}

void
errExit(const char *format, ...)
{
	va_list argList;

	va_start(argList, format);

	outputError(TRUE,		// outputError(Boolean usrErr,
				errno,		//			   int err,
				TRUE,		//			   Boolean flushStdout,
				format,		//			   const char *format,
				argList);	//			   va_list ap);
	/*
	vsnprintf(userMsg, 500, format, argList);

	snprintf(errText,
			 500,
			 " [%s %s]",
			 (errno > 0 && errno <= MAX_ENAME) ? ename[errno] : "?UNKNOWN?",
			 strerror(errno));

	snprintf(buf, 500, "ERROR%s %s\n", errText, userMsg);
	fflush(stdout);

	fput(buf, stderr);
	fflush(stderr);
	*/
	va_end(argList);

	terminate(TRUE);		// terminate(Boolean useExit3)
	/*
	else if (TRUE)	
		exit(EXIT_FAILURE);	--> "EXIT_FAILURE" send to parent process or OS
	*/
}

void
err_exit(const char *format, ...)
{
	va_list argList;

	va_start(argList, format);

	outputError(TRUE,		// outputError(Boolean usrErr,
				errno,		//			   int err,
				FALSE,		//			   Boolean flushStdout,
				format,		//			   const char *format,
				argList);	//			   va_list ap);
	/*
	vsnprintf(userMsg, 500, format, argList);

	snprintf(errText,
			 500,
			 " [%s %s]",
			 (errno > 0 && errno <= MAX_ENAME) ? ename[errno] : "?UNKNOWN?",
			 strerror(errno)};

	snprintf(buf, 500, "ERROR%s %s\n", errText, userMsg);
	
	fput(buf, stderr);
	fflush(stderr);
	*/
	va_end(argList);

	terminate(FALSE);
	/*
	else // FALSE
		_exit(EXIT_FAILURE); --> "EXIT_FAILURE" send to parent process or OS
	*/
}

void
errExitEN(int errnum, const char *format, ...)
{
	va_list argList;

	va_start(argList, format);

	outputError(TRUE,		// outputError(Boolean usrErr,
				errnum,		//			   int err,
				TRUE,		//			   Boolean flushStdout,
				format,		//			   const char *format,
				argList);	//			   va_list ap);
	/*
	vsnprintf(userMsg, 500, format, ap);

	snprintf(errText,
			 500,
			 "[ %s %s]",
			 (errnum > 0 && errnum <= MAX_ENAME) ? ename[errnum] : "?UNKNOWN?",
			 strerror(errnum));
	
	snprintf(buf, 500, "ERROR%s %s\n", errText, userMsg);
	fflush(stdout);

	fput(buf, stderr);
	fflush(stderr);
	*/
	va_end(argList);

	terminate(TRUE);
	/*
	else if (TRUE)
		exit(EXIT_FAILURE); --> "EXIT_FAILURE" send to parent process or OS
	*/
}

void 
rt_errExitEN(int errnum, const char *format, ...)
{
	va_list argList;

	va_start(argList, format);

	outputError(TRUE,		// outputError(Boolean usrErr,
				-errnum,	//			   int err,
				TRUE,		//			   Boolean flushStdout,
				format,		//			   const char *format,
				argList);	//			   va_list ap);
	/*
	vsnprintf(userMsg, 500, format, ap);

	snprintf(errText,
			 500,
			 "[ %s %s]",
			 (errnum > 0 && errnum <= MAX_ENAME) ? ename[errnum] : "?UNKNOWN?",
			 strerror(errnum));
	
	snprintf(buf, 500, "ERROR%s %s\n", errText, userMsg);
	fflush(stdout);

	fput(buf, stderr);
	fflush(stderr);
	*/
	va_end(argList);

	terminate(TRUE);
	/*
	else if (TRUE)
		exit(EXIT_FAILURE); --> "EXIT_FAILURE" send to parent process or OS
	*/
}

void
fatal(const char *format, ...)
{
	va_list argList;

	va_start(argList, format);

	outputError(FALSE,		// outputError(Boolean usrErr,
				0,			//			   int err,
				TRUE,		//			   Boolean flushStdout,
				format,		//			   const char *format,
				argList);	//			   va_list ap);
	/*
	vsnprintf(userMsg, 500, format, ap);

	snprintf(errText, 500, ":");

	snprintf(buf, 500, "ERROR%s %s\n", errText, userMsg);
	fflush(stdout);

	fput(buf, stderr);
	fflush(stderr);
	*/
	va_end(argList);

	terminate(TRUE);
	/*
	else if (TRUE)
		exit(EXIT_FAILURE); --> "EXIT_FAILURE" send to parent process or OS
	*/
}

void
usageErr(const char *format, ...)
{
	va_list argList;

	fflush(stdout);
	
	fprintf(stderr, "Usage: ");

	va_start(argList, format);

	vfprintf(stderr, format, argList);

	va_end(argList);

	fflush(stderr);

	exit(EXIT_FAILURE);
}

void
cmdLineErr(const char *format, ...)
{
	va_list argList;

	fflush(stdout);

	fprintf(stderr, "Command-line usage error: ");

	va_start(argList, format);

	vfprintf(stderr, format, argList);

	va_end(argList);

	fflush(stderr);

	exit(EXIT_FAILURE);
}
