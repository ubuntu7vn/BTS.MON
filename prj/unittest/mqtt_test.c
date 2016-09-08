/*
 * os_test.c
 *
 *  Created on: Aug 30, 2016
 *      Author: Nami
 */

#include "unittest.h"

#ifdef _MQTT_TEST_THREAD_H

#include <stdio.h>
#include <stdlib.h>

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "mosquitto.h"

osThreadId mqttTestID = NULL;
void mqtt_test_thread(void const *arg);

osThreadDef(MQTT, mqtt_test_thread, osPriorityNormal, 0,
			configMINIMAL_STACK_SIZE);

void mqtt_test_thread(void const *arg)
{
	int ret, mid;
	struct mosquitto *mosq = NULL;
	(void)arg;
	
	mosq = mosquitto_new("hanv14", 0, NULL);
	if(!mosq){
		// error
		return;
	}
	
	ret = mosquitto_connect(mosq, "127.0.0.1", 80, 60);
	if(ret != MOSQ_ERR_SUCCESS){
		return;
	}
	
	mosquitto_publish(mosq, &mid, "hanv/mqtt/test", 3, "abc", 1, 0);
	
	for(;;){
		osDelay(1000);
	}
}

void mqtt_test_init(void)
{
	mqttTestID = osThreadCreate(osThread(MQTT), NULL);
}

#endif //_OS_TEST_THREAD_H

