/*
 * usb_pd_driver.c
 *
 * Created: 11/11/2017 23:55:12
 *  Author: jason
 */ 

#include "usb_pd_driver.h"
#include "usb_pd.h"

uint32_t pd_task_set_event(uint32_t event, int wait_for_reply)
{
	switch (event)
	{
		case PD_EVENT_TX:
			break;
		default:
			break;
	}
	return 0;
}

timestamp_t get_time(void)
{
	timestamp_t t;
	
	// TODO: set up a timer and read the value out of it
	//t.le.lo = STM32_TIM32_CNT(2);
	//t.le.hi = clksrc_high;
	return t;
}
