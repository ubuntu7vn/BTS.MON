#ifndef _UNIT_TEST
#define _UNIT_TEST

#define _OS_TEST_THREAD_H		// unit test rtos
//#define _USB_TEST_THREAD_H	// unit test usb
//#define _MQTT_TEST_THREAD_H
//#define _LWIP_TEST_THREAD_H

#ifdef _OS_TEST_THREAD_H
void os_test_init(void);
#endif

#ifdef _MQTT_TEST_THREAD_H
void mqtt_test_init(void);
#endif
#ifdef _LWIP_TEST_THREAD_H
void lwip_test_init(void);
#endif

#endif //_UNIT_TEST
