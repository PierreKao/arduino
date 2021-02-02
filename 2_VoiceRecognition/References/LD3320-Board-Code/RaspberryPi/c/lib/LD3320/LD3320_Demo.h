/******************************************************************************
*****************************LD3320 test demo**********************************
* | file      	:	LD3320_Demo.h
* |	version			:	V1.0
* | date				:	2020-06-22
* | function		:	LD3320 test demo
******************************************************************************/
#ifndef __LD3320_DEMO_H
#define	__LD3320_DEMO_H

#include "LD3320_Driver.h"

void LD3320_test(void);
void Board_text(UBYTE Code_Val);
void LED_flash(void);
void LED_run(void);
void LED_key(void);
void Play_demo(char *path);	


#endif
