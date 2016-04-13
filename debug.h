#ifndef __DEBUG_H
#define __DEBUG_H

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
#define D(fmt, ...) do {\
			printf(fmt" func:%s line:%d err:%s\n",\
				##__VA_ARGS__, __func__, __LINE__,\
				strerror(errno));\
			exit(EXIT_FAILURE);\
		} while(0)
#else
#define D(fmt, ...)
#endif

#endif
