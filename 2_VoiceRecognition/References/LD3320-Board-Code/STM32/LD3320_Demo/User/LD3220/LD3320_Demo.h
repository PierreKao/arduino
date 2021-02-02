/******************************************************************************
*****************************LD3320 test demo**********************************
* | file      	:	LD3320_Demo.h
* |	version			:	V1.0
* | date				:	2020-06-18
* | function		:	LD3320 test demo
******************************************************************************/
#ifndef __LD3320_DEMO_H
#define	__LD3320_DEMO_H

#include "LD3320_Driver.h"

//LED GPIO
#define LED2_0		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET)
#define LED2_1		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET)
#define ReadLED2	HAL_GPIO_ReadPin(LED2_GPIO_Port, LED2_Pin)

#define LED3_0		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET)
#define LED3_1		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET)

#define LED4_0		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET)
#define LED4_1		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET)

void LD3320_test(void);
void Board_text(UBYTE Code_Val);
void LED_flash(void);
void LED_run(void);
void LED_key(void);
void Play_demo(void);	

#endif
