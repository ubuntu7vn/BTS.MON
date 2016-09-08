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

#include "cmsis_os.h"

typedef struct{
		uint8_t s1:1;
		uint8_t s2:1;
		uint8_t unused:6;
}doors_t;

typedef struct{
	doors_t door;
	uint8_t humidity[2];
	uint8_t waterpoor;
	uint8_t clasebroken;
	uint8_t smoke;
	uint8_t moving;
	int16_t temper[3];
}sensor_t;

static void door_hw_init(void)
{
	
}
