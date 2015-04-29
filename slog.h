#ifndef __SLOG_H__
#define __SLOG_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <pthread.h>

/* Definations for version info */
#define SLOGVERSION "0.0.1 beta"
#define SLOGBUILD 14
#define _LOG_BUFFSIZE  (4UL<<20)
#define _LOG_STORE_BUFFSIZE	(4UL<<20)
#define _LOG_FNAME_LENGTH 1024

typedef  enum {  
	LOG_INFO 	= 1UL<<0,
	LOG_DEBUG 	= 1UL<<1,
	LOG_TRACE 	= 1UL<<2,
	LOG_NOTICE 	= 1UL<<3, 
	LOG_WARNING = 1UL<<4, 
	LOG_ERROR 	= 1UL<<5,
	LOG_FATAL 	= 1UL<<6,
}loglevel_e;
/*---------------------------------------------
| Structure for log level
---------------------------------------------*/
typedef struct {
	int	 at_exit;
    char* fname;
    int level;
    int l_max;
    int to_file;
	FILE* fp;
	pthread_mutex_t mutex;
	char *m_buffer;
	//char store_buf[_LOG_STORE_BUFFSIZE];
} SLogValues;

#define CONN(X,Y) X##Y          //不能用，出错，##不能直接连接字符串？
#define TO_STR(X) #X
#define TOSTR(X) TO_STR(X)

/*---------------------------------------------
| Get slog version
---------------------------------------------*/
const char* slog_version();

extern SLogValues slog_val;
/*---------------------------------------------
| Initialise log level
---------------------------------------------*/
int init_slog(char* fname,int minlvl, int to_file, int max, int append);
/*---------------------------------------------
| Log exiting process
---------------------------------------------*/
int slog(int level, char *msg, ...);

#define INFO(fmt,...) do{												\
		if(slog_val.fname == NULL){										\
			char tmp[_LOG_FNAME_LENGTH] = {0};							\
			snprintf(tmp, _LOG_FNAME_LENGTH -1, "%s.log", __FILE__);	\
			init_slog(tmp, LOG_INFO, 1, LOG_FATAL, 1);					\
		}																\
		slog(LOG_INFO, fmt, ##__VA_ARGS__);								\
	}while(0)
#define DEBUG(fmt,...) do{												\
		if(slog_val.fname == NULL){										\
			char tmp[_LOG_FNAME_LENGTH] = {0};							\
			snprintf(tmp, _LOG_FNAME_LENGTH -1, "%s.log", __FILE__);	\
			init_slog(tmp, LOG_INFO, 1, LOG_FATAL, 1);					\
		}																\
		slog(LOG_DEBUG, fmt, ##__VA_ARGS__);							\
	}while(0)
#define TRACE(fmt,...) do{												\
		if(slog_val.fname == NULL){										\
			char tmp[_LOG_FNAME_LENGTH] = {0};							\
			snprintf(tmp, _LOG_FNAME_LENGTH -1, "%s.log", __FILE__);	\
			init_slog(tmp, LOG_INFO, 1, LOG_FATAL, 1);					\
		}																\
		slog(LOG_TRACE, fmt, ##__VA_ARGS__);							\
	}while(0)
	

#define NOTICE(fmt,...) do{												\
		if(slog_val.fname == NULL){										\
			char tmp[_LOG_FNAME_LENGTH] = {0};							\
			snprintf(tmp, _LOG_FNAME_LENGTH -1, "%s.log", __FILE__);	\
			init_slog(tmp, LOG_INFO, 1, LOG_FATAL, 1);					\
		}																\
		slog(LOG_NOTICE, fmt, ##__VA_ARGS__);							\
	}while(0)

#define WARN(fmt,...) do{												\
		if(slog_val.fname == NULL){										\
			char tmp[_LOG_FNAME_LENGTH] = {0};							\
			snprintf(tmp, _LOG_FNAME_LENGTH -1, "%s.log", __FILE__);	\
			init_slog(tmp, LOG_INFO, 1, LOG_FATAL, 1);					\
		}																\
		slog(LOG_WARNING, fmt, ##__VA_ARGS__);							\
	}while(0)
#define ERROR(fmt,...) do{												\
		if(slog_val.fname == NULL){										\
			char tmp[_LOG_FNAME_LENGTH] = {0};							\
			snprintf(tmp, _LOG_FNAME_LENGTH -1, "%s.log", __FILE__);	\
			init_slog(tmp, LOG_INFO, 1, LOG_FATAL, 1);					\
		}																\
		slog(LOG_ERROR, fmt, ##__VA_ARGS__);							\
	}while(0)
#define FATAL(fmt,...) do{												\
		if(slog_val.fname == NULL){										\
			char tmp[_LOG_FNAME_LENGTH] = {0};							\
			snprintf(tmp, _LOG_FNAME_LENGTH -1, "%s.log", __FILE__);	\
			init_slog(tmp, LOG_INFO, 1, LOG_FATAL, 1);					\
		}																\
		slog(LOG_FATAL, fmt, ##__VA_ARGS__);							\
	}while(0)


void uninit_slog();


#ifdef __cplusplus
}
#endif
#endif /***__SLOG_H__**/