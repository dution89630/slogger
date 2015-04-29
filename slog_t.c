#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <slog.h>
#include <unistd.h>

void* do_log(){
	int i = 10000;
	do{
		DEBUG("test logtest logtest logtest log%s %x %d","aaaaaa", pthread_self(), i);
	}while(i--);
}

/*---------------------------------------------
| Main function
---------------------------------------------*/
#define THREAD_COUNT 10
int main(int argc, char *argv[]){

	pthread_t thid[THREAD_COUNT];
	int i = 0;
	for(i = 0; i < THREAD_COUNT; i++){
		pthread_create(&(thid[i]), NULL, do_log, NULL);
	}
	
	for(i = 0; i < THREAD_COUNT; i++){
		pthread_join(thid[i], NULL);
	}
    /* Get and print slog version */
    
	/*
	while(1){
		usleep(1000);
	}
	*/
    return 0;
}