#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include <pthread.h>	//pthread_create()
#include "LD3320_Demo.h"

volatile UBYTE flag_irq = 0;
pthread_t t1;
UBYTE flag_t = 1;	
	
void Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:exit\r\n");
	flag_t = 0;
	pthread_join(t1, NULL);
    DEV_ModuleExit();
    exit(0);
}

void *pthread_irq(void *arg)
{

	while(flag_t) {		
		if(DEV_Digital_Read(LD_IRQ) == 0) {
			flag_irq = 1;
		}
		else {
			flag_irq = 0;
		}		
	}
	printf("thread:exit\r\n");
	pthread_exit(NULL);
}

int main()
{
	// Exception handling:ctrl + c
    signal(SIGINT, Handler);
	DEV_ModuleInit();
	pthread_create(&t1, NULL, pthread_irq, NULL);
	
    printf("Demo Begin\r\n");
	LD3320_test();

	return 0;
}

