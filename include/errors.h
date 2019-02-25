#ifndef _ERRORS_H_
#define _ERRORS_H_

#include "emp_hdr.h"
#include "ename.c.inc"

#ifdef __GNUC__
#define NORETURN __attribute__ ((__noreturn__))
#else
#define NORETURN
#endif

void errMsg(const char *format, ...);
void errExit(const char *format, ...) NORETURN;
void err_exit(const char *format, ...) NORETURN;
void errExitEN(int errnum, const char *format, ...) NORETURN;
void rt_errExitEN(int errnum, const char *format, ...) NORETURN;

void fatal(const char *format, ...) NORETURN;
void usageErr(const char *format, ...) NORETURN;
void cmdLineErr(const char *format, ...) NORETURN;

#endif /*_ERRORS_H_*/

/*
void errMsg(const char *format, ...)
	: errno을 기억하고 에러 메세지를 출력한다.
	: 프로그램을 종료시키지 않는다.

void errExit(const char *format, ...)
	: errMsg(...)와 비슷하지만, 에러 메세지를 출력한뒤,
	  exit(3) 또는 _exit(2)를 호출하여 프로그램을 종료시킨다.
	  EF_DUMPCORE가있으면 abort(3)를 호출하여 디버그용 
	  core dump파일을 만든다.

void err_exit(const char *format, ...)
	: errExit(...)와 비슷하지만, 표준 출력버퍼의 남은 내용을
	  flush하지 않는다.
	: exit(3) 대신 _exit(2)를 호출하여 프로그램을 종료시킨다.

void errExitEN(const char *format, ...)
	: errExit(...)와 비슷하지만, errno에 따라 에러 메세지를
	  출력하지 않고, errnum이란 인자로 넘겨 받아 에러 메세지를
	  출력한다.
	: 주로 POSIX 스레드 API를 채택한 프로그램에서 사용한다.
	  POSIX 스레드함수 --> 성공 0, 실패 errno(양정수)
	  UNIX 시스템 함수 --> 성공 양정수 or NULL, 실패 -1
	  					   에러정보 errno(양정수)
	-----< example>---------------------------------------------
	int s;

	s = pthread_create(&thread, NULL, func, &arg)
	if (s != 0)
		errExit(s, "pthread_create");
	------------------------------------------------------------

void fatal(const char *format, ...)
	: errno를 정의하지 않는 라이브러리 함수, 일반적인 에러
	: format된 메세지를 에러로 출력하고 exit(3)을 호출하고 종료 

void usageErr(const char *format, ...)
	: 명령행 인자의 용법이 잘못되었을 때 사용한다.
	: format된 메세지를 Usage: 와 함께 출력하고 exit(3)으로 종료

void cmdLineErr(const char *format, ...)
	: usageErr(..)와 동일하며, format된 메세지를
	  Command-line usage error: 와 함께 출력하고 exit(3)로 종료
*/

