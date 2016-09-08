/*
    BTS.MON 1.0 - Copyright (C) 2016 ethings Ltd.
    All rights reserved

    VISIT http://www.ethings.vn TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the BTS.MON distribution.

    BTS.MON is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the BTS.MON exception.
		
		Author: hanv, quannv	
*/
 
#include <stdio.h>
#include <stdlib.h>

#include "stm32f4xx.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"

#include "tsk_button.h"

#define KEY_GPIO		GPIOE
#define KEY_RCC_CLOCK_ENABLE	__HAL_RCC_GPIOE_CLK_ENABLE

#define KEY_COL1		GPIO_PIN_0
#define KEY_COL2		GPIO_PIN_1
#define KEY_COL3		GPIO_PIN_2
#define KEY_COL4		GPIO_PIN_3
#define KEY_ROW1		GPIO_PIN_4
#define KEY_ROW2		GPIO_PIN_5
#define KEY_ROW3		GPIO_PIN_6
#define KEY_ROW4		GPIO_PIN_7

#define KEY_COL1_OUT_HIGH		HAL_GPIO_WritePin(KEY_GPIO, KEY_COL1, GPIO_PIN_SET)
#define KEY_COL1_OUT_LOW		HAL_GPIO_WritePin(KEY_GPIO, KEY_COL1, GPIO_PIN_RESET)

#define KEY_COL2_OUT_HIGH		HAL_GPIO_WritePin(KEY_GPIO, KEY_COL2, GPIO_PIN_SET)
#define KEY_COL2_OUT_LOW		HAL_GPIO_WritePin(KEY_GPIO, KEY_COL2, GPIO_PIN_RESET)

#define KEY_COL3_OUT_HIGH		HAL_GPIO_WritePin(KEY_GPIO, KEY_COL3, GPIO_PIN_SET)
#define KEY_COL3_OUT_LOW		HAL_GPIO_WritePin(KEY_GPIO, KEY_COL3, GPIO_PIN_RESET)

#define KEY_COL4_OUT_HIGH		HAL_GPIO_WritePin(KEY_GPIO, KEY_COL4, GPIO_PIN_SET)
#define KEY_COL4_OUT_LOW		HAL_GPIO_WritePin(KEY_GPIO, KEY_COL4, GPIO_PIN_RESET)

#define KEY_ROW1_IN					!HAL_GPIO_ReadPin(KEY_GPIO, KEY_ROW1)
#define KEY_ROW2_IN					!HAL_GPIO_ReadPin(KEY_GPIO, KEY_ROW2)
#define KEY_ROW3_IN					!HAL_GPIO_ReadPin(KEY_GPIO, KEY_ROW3)
#define KEY_ROW4_IN					!HAL_GPIO_ReadPin(KEY_GPIO, KEY_ROW4)

/* key mapping */
static const uint8_t KEY_MAPS[4][4] = {
	{0x01, 0x02, 0x03, 0x0C},
	{0x04, 0x05, 0x06, 0x0D},
	{0x07, 0x08, 0x09, 0x0E},
	{0x0A, 0x00, 0x0B, 0x0F},
};
/* queue store key code */
static QueueHandle_t gKeyQueueId;
/* key task id */
TaskHandle_t osKeyID = NULL;

/* private functions */
void os_key_thread(void *arg);

/**
 * @brief assert hardware column PIN for scan key press
 * @param col as column in pad
 * @retval none
 **/ 
static void key_hw_col_set(uint8_t col)
{
/* reset all hardware pin */
	KEY_COL1_OUT_HIGH;
	KEY_COL2_OUT_HIGH;
	KEY_COL3_OUT_HIGH;
	KEY_COL4_OUT_HIGH;
	
	/* assert column */
	switch(col){
	case 1: KEY_COL1_OUT_LOW; break;
	case 2: KEY_COL2_OUT_LOW; break;
	case 3: KEY_COL3_OUT_LOW; break;
	case 4: KEY_COL4_OUT_LOW; break;
	default: break;
	}
}

/**
 * @brief check row has pressed and return it's code
 * @param row as row in pad
 * @retval code of key pressed.
 **/ 
static uint8_t key_hw_row_read(uint8_t row)
{
	if(KEY_ROW1_IN){ return KEY_MAPS[0][row-1];}
	if(KEY_ROW2_IN){ return KEY_MAPS[1][row-1];}
	if(KEY_ROW3_IN){ return KEY_MAPS[2][row-1];}
	if(KEY_ROW4_IN){ return KEY_MAPS[3][row-1];}
	
	return 0xFF;
}

/**
 * @brief assert low to column and get code of row which has pressed.
 * @param none
 * @retval code of key
 **/ 
static uint8_t key_scan(void)
{
	uint8_t key;
	int i;
	
	for(i=1;i<5;i++){
		/* scan col i*/
		key_hw_col_set(i);
		/* check input as low */
		key = key_hw_row_read(i);
		if(key != 0xFF) return key;
	}
	return 0xFF;
}

/**
 * @brief put key code to queue, this functions must call in loop function with frequecy ms
 * @param time which due pressed time.
 * @retval none
 **/ 
static void key_update(uint16_t time)
{
	static uint8_t keyCount = 0;	
	key_t myKey;
	
	myKey.KeyCode = key_scan();
	
	if(0xFF != myKey.KeyCode){
		keyCount = (keyCount > time)? keyCount:(keyCount+1);
		if(keyCount > time){
			/* put to queue for other task access */
			if(xQueueSend(gKeyQueueId, &myKey, 0) != pdTRUE){
				//
			}else{
				//
			}
			keyCount = 0;
		}
	}else{		
		keyCount = 0;
	}
}

/**
 * @brief hardware init pin, port for keypad
 * @param  none
 * @retval none
 **/ 
static void key_hw_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	KEY_RCC_CLOCK_ENABLE();
	
	GPIO_InitStructure.Pin = KEY_ROW1;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(KEY_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = KEY_ROW2;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(KEY_GPIO, &GPIO_InitStructure);	
	
	GPIO_InitStructure.Pin = KEY_ROW3;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(KEY_GPIO, &GPIO_InitStructure);	
	
	GPIO_InitStructure.Pin = KEY_ROW4;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(KEY_GPIO, &GPIO_InitStructure);	
	
	GPIO_InitStructure.Pin = KEY_COL1;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(KEY_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = KEY_COL2;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(KEY_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = KEY_COL3;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(KEY_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = KEY_COL4;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(KEY_GPIO, &GPIO_InitStructure);
}

/**
 * @brief init keypad module
 * @param none
 * @retval none
 **/ 
int key_init(void)
{
	// init hardware define.
	key_hw_init();
	// init memory and queue
	gKeyQueueId = xQueueCreate(1, sizeof(key_t));
	if(!gKeyQueueId) return 0;
	
	if(xTaskCreate(os_key_thread, "Key", configMINIMAL_STACK_SIZE, NULL,
									tskIDLE_PRIORITY+1, &osKeyID)!=pdTRUE)
		return 0;
		
	return 1;
}

/**
 * @brief get key code which pressed and store in queue.
 * @param tmo as time out for wait queue ready.
 * @retval key code
 **/ 
uint8_t key_getcode(uint16_t tmo)
{
	key_t key;
	if(xQueueReceive(gKeyQueueId, &key, tmo) == pdTRUE)
		return key.KeyCode;
	
	return 0xFF;
}

/**
 * @brief thread scan keypad
 * @param arg as unused.
 * @retval none
 **/ 
void os_key_thread(void *arg)
{
	(void)arg;

	for(;;){
		key_update(20);
		// delay 10ms.
		vTaskDelay(10);
	}
}
