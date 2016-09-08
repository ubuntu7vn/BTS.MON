/*
 * os_test.c
 *
 *  Created on: Aug 30, 2016
 *      Author: Nami
 */

#include "unittest.h"

#ifdef _OS_TEST_THREAD_H

#include <stdio.h>
#include <stdlib.h>

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t osTestID = NULL;
void os_test_thread(void *arg);

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *  
  */
void LED_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	__GPIOB_CLK_ENABLE();
	
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
}

void os_test_thread(void *arg)
{
	(void)arg;

	LED_Init();
	
	for(;;){
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
		vTaskDelay(1000);
	}
}

void os_test_init(void)
{
	xTaskCreate(os_test_thread, "LED", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &osTestID);
}

#endif //_OS_TEST_THREAD_H

