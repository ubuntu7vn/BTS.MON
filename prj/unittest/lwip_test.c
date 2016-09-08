/*
 * os_test.c
 *
 *  Created on: Aug 30, 2016
 *      Author: Nami
 */

#include "unittest.h"

#ifdef _LWIP_TEST_THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "lwip/opt.h"

#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/tcpip.h"


#ifndef SOCK_TARGET_HOST
#define SOCK_TARGET_HOST  "192.168.1.124"
#endif

#ifndef SOCK_TARGET_PORT
#define SOCK_TARGET_PORT  8003
#endif

osThreadId lwipTestID = NULL;
void lwip_test_thread(void const *arg);

osThreadDef(LWIP, lwip_test_thread, osPriorityNormal, 0,
			configMINIMAL_STACK_SIZE);

void lwip_test_thread(void const *arg)
{
	int s;
	int ret;
	struct sockaddr_in addr;
	
	/* set up address to connect to */
	memset(&addr, 0, sizeof(addr));
	addr.sin_len = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_port = PP_HTONS(SOCK_TARGET_PORT);
	addr.sin_addr.s_addr = inet_addr(SOCK_TARGET_HOST);
	/* create the socket */
	s = lwip_socket(AF_INET, SOCK_STREAM, 0);
	/* connect */
	ret = lwip_connect(s, (struct sockaddr*)&addr, sizeof(addr));
	if(ret){
		while(1);
	}
	/* write something */
	ret = lwip_write(s, "test", 4);
	
	for(;;){
		char buffer[24];
		int nbytes;

		nbytes=lwip_recv(s, buffer, sizeof(buffer), 0x08);
		if (nbytes>0) {
			lwip_send(s, buffer, nbytes, 0);
			//term_printk("\nLwip recv: %d, %s\n", nbytes, buffer);
		}
		vTaskDelay(1000);
	}
}

void lwip_test_init(void)
{
	lwipTestID = osThreadCreate(osThread(LWIP), NULL);
}

#endif //_LWIP_TEST_THREAD_H

