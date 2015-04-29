#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <slog.h>


/* Main struct variable */
SLogValues slog_val = {
	0,
	NULL,
	LOG_INFO,
	0,
	0,
	NULL,
	PTHREAD_MUTEX_INITIALIZER,
	NULL,
};

const char* loglevel2string(loglevel_e lvl){
	switch(lvl){
		case (1UL<<0) : 
			return "INFO";
			break;
		case (1UL<<1):
			return "DEBUG";
			break;
		case (1UL<<2):
			return "TRACE";
			break;
		case (1UL<<3):
			return "NOTICE";
			break;
		case (1UL<<4):
			return "WARN";
			break;
		case (1UL<<5):
			return "ERROR";
			break;
		case (1UL<<6):
			return "FATAL";
			break;
		default:
			return NULL;
	}
}


/*---------------------------------------------
| Get slog version
---------------------------------------------*/
const char* slog_version(){
    static char verstr[128];
    sprintf(verstr, "%s Build %d (%s)", SLOGVERSION, SLOGBUILD, __DATE__);
    return verstr;
}

/*---------------------------------------------
| Initialise log level
---------------------------------------------*/
int init_slog(char* fname,int minlvl, int to_file, int max, int append){
	pthread_mutex_lock(&(slog_val.mutex));
	if(slog_val.fname == NULL){
		slog_val.level = minlvl;
		size_t len = strlen(fname);
		slog_val.fname = malloc(sizeof(char)*(len + 1));
		if(slog_val.fname == NULL){
			printf("strdup file name error!\n");
			pthread_mutex_unlock(&(slog_val.mutex));
			return 0;
		}
		strncpy(slog_val.fname, fname, len);
		slog_val.fname[len] = 0;
		slog_val.l_max = max;
		slog_val.to_file = to_file;
		/***m_buffer init****/
		if(slog_val.m_buffer != NULL){
			free(slog_val.m_buffer);
		}
		slog_val.m_buffer = malloc(sizeof(char)*_LOG_BUFFSIZE);
		if(slog_val.m_buffer == NULL){
			free(slog_val.fname);
			pthread_mutex_unlock(&(slog_val.mutex));
			return 0;
		}
		/***open file****/
		slog_val.fp = fopen(fname, append ? "a":"w");
		if(slog_val.fp == NULL){
			//init failed
			free(slog_val.fname);
			free(slog_val.m_buffer);
			pthread_mutex_unlock(&(slog_val.mutex));
			return 0;
		}
		if(slog_val.at_exit == 0){
			atexit(uninit_slog);
		}
	}
	pthread_mutex_unlock(&(slog_val.mutex));
	return 1;
}

void uninit_slog(){
	pthread_mutex_lock(&(slog_val.mutex));
	if(slog_val.fname){
		free(slog_val.fname);
		slog_val.fname = NULL;
	}
	if(slog_val.fp){
		fclose(slog_val.fp);
		slog_val.fp = NULL;
	}
	if(slog_val.m_buffer != NULL){
		free(slog_val.m_buffer);
	}
	pthread_mutex_unlock(&(slog_val.mutex));
}

/*---------------------------------------------
| make date with level infomation
---------------------------------------------*/
int __premakestr(char* m_buffer, loglevel_e lvl){
	time_t now = time(NULL);
	struct tm* vtm = localtime(&now);
    return snprintf(m_buffer, 256, "[%s]%04d-%02d-%02d %02d:%02d:%02d ",loglevel2string(lvl),
            vtm->tm_year+ 1900, vtm->tm_mon + 1, vtm->tm_mday, vtm->tm_hour, vtm->tm_min, vtm->tm_sec);
}

/*---------------------------------------------
| Log exiting process
---------------------------------------------*/
int slog(int level, char *msg, ...){
	int ret = 0;
	pthread_mutex_lock(&(slog_val.mutex));
	if(loglevel2string(level) != NULL){
		char* tmp = NULL;
		int vslen = 0;
		
		/* initialise system date */
		size_t len = __premakestr(slog_val.m_buffer, level);
		tmp = slog_val.m_buffer + len;

		/* Read args */
		va_list args;
		va_start(args, msg);
		vslen = vsnprintf(tmp, _LOG_BUFFSIZE - len, msg, args);
		va_end(args);
		slog_val.m_buffer[len + vslen] = '\n';
		slog_val.m_buffer[len + vslen + 1] = 0;
		ret = fwrite(slog_val.m_buffer, vslen + 1 + len,  1, slog_val.fp);
		fflush(slog_val.fp);
	}
	pthread_mutex_unlock(&(slog_val.mutex));
	return ret;
}



