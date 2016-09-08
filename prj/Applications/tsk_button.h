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

#ifndef _BUTTON_H
#define _BUTTON_H

typedef struct {
	uint8_t KeyCode;
	uint8_t KeyMulti;
}key_t;

int key_init(void);
uint8_t key_getcode(uint16_t tmo);

#endif //_BUTTON_H
