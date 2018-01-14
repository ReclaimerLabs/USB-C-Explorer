/* This source file is part of the ATMEL QTouch Library 5.0.8 */

/*****************************************************************************
 *
 * \file
 *
 * \brief  This file contains the SAMD QTouch library sample user application.
 *
 *
 * - Userguide:          QTouch Library Peripheral Touch Controller User Guide.
 * - Support email:      www.atmel.com/design-support/
 *
 *
 * Copyright (c) 2013-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 ******************************************************************************/

/**
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include "ugui.h"
#include "tcpm_driver.h"
#include "usb_pd_driver.h"

/**
 * Device Revision definition
 */
#define DEVICE_REVISION_MASK (0xFu << 8)
#define DEVICE_REVISION_MASK_POS (8u)
#define DEV_REV_A (0x0)
#define DEV_REV_B (0x1)
#define DEV_REV_C (0x2)

struct rtc_module rtc_instance;
struct tc_module tc_instance;
uint32_t g_us_timestamp_upper_32bit;
struct i2c_master_module i2c_master_instance;
uint32_t device_rev = 0;

// USB-C Specific - TCPM start 1
const struct tcpc_config_t tcpc_config[CONFIG_USB_PD_PORT_COUNT] = {
	{&i2c_master_instance, fusb302_I2C_SLAVE_ADDR, &fusb302_tcpm_drv},
};
// USB-C Specific - TCPM end 1

uint8_t display_buffer[DISP_MEM_SIZE];
UG_GUI gui;

/*
 * switch_main_clock 
 */
void switch_main_clock(void);

/*! \brief Initialize timer
 *
 */
void rtc_init( void );

/*! \brief RTC timer overflow callback
 *
 */
void rtc_overflow_callback(void);

/*! \brief Configure the RTC timer callback
 *
 */
void configure_rtc_callbacks(void);

/*! \brief Configure the RTC timer count after which interrupts comes
 *
 */
void configure_rtc_count(void);

/*! \brief Set timer period.Called from Qdebug when application
 *     has to change the touch time measurement
 */
void set_timer_period(void);

/*! \brief RTC timer overflow callback
 *
 */
void rtc_overflow_callback(void)
{
	/* Do something on RTC overflow here */
	touch_time.time_to_measure_touch = 1;
	touch_time.current_time_ms = touch_time.current_time_ms +
			touch_time.measurement_period_ms;
}

/*! \brief Configure the RTC timer callback
 *
 */
void configure_rtc_callbacks(void)
{
	/* register callback */
	rtc_count_register_callback(&rtc_instance,
			rtc_overflow_callback, RTC_COUNT_CALLBACK_OVERFLOW);
	/* Enable callback */
	rtc_count_enable_callback(&rtc_instance, RTC_COUNT_CALLBACK_OVERFLOW);
}

/*! \brief Configure the RTC timer count after which interrupts comes
 *
 */
void configure_rtc_count(void)
{
	struct rtc_count_config config_rtc_count;
	rtc_count_get_config_defaults(&config_rtc_count);
    #if ((SAMC20) || (SAMC21))
	config_rtc_count.prescaler           = RTC_MODE0_CTRLA_PRESCALER_DIV32;
    #else
	config_rtc_count.prescaler           = RTC_MODE0_CTRL_PRESCALER_DIV2;
    #endif
	config_rtc_count.mode                = RTC_COUNT_MODE_16BIT;
	#ifdef FEATURE_RTC_CONTINUOUSLY_UPDATED
	config_rtc_count.continuously_update = true;
	#endif
	/* initialize rtc */
	rtc_count_init(&rtc_instance, RTC, &config_rtc_count);

	/* enable rtc */
	rtc_count_enable(&rtc_instance);
}

/*! \brief Initialize timer
 *
 */
void rtc_init(void)
{
	/* Configure and enable RTC */
	configure_rtc_count();

	/* Configure and enable callback */
	configure_rtc_callbacks();

	/* Set Timer Period */

	rtc_count_set_period(&rtc_instance, DEF_TOUCH_MEASUREMENT_PERIOD_MS);
}

/*! \brief Set timer period.Called from Qdebug when application
 *     has to change the touch time measurement
 */
void set_timer_period(void )
{
	rtc_count_set_period(&rtc_instance, touch_time.measurement_period_ms);
}

void display_init(void);
void display_set_pixel(UG_S16 x, UG_S16 y, UG_COLOR color);
void i2c_init(void);
void timer_init(void);
void tc_callback_overflow(void);
void measure_nonPD_current(void);

/*! \brief Main function
 *
 */
int main(void)
{
	system_init();
	irq_initialize_vectors();
	cpu_irq_enable();
	ioport_init();
	delay_init();
	rtc_init();
	touch_sensors_init();
	system_interrupt_enable_global();
	
	display_init();
	i2c_init();
	timer_init();
	delay_ms(50);
	// USB-C Specific - TCPM start 2
	tcpm_init(0);
	delay_ms(50);
	pd_init(0);
	delay_ms(50);
	ioport_set_pin_dir(USBC_INT_PIN, IOPORT_DIR_INPUT);
	// USB-C Specific - TCPM end 2
	
	//system_set_sleepmode(SYSTEM_SLEEPMODE_STANDBY);

	// Start USB stack to authorize VBus monitoring
	udc_start();
	
	measure_nonPD_current();
	delay_ms(50);

	while (1) {
		//system_sleep();

		touch_sensors_measure();
		
		if (!ioport_get_pin_level(USBC_INT_PIN)) {
			tcpc_alert(0);
		}
		
		pd_run_state_machine(0);
		
		delay_ms(2);

		/**
		 * Update touch status once measurement complete flag is set.
		 */

		/**
		 * Self Cap method
		 * if ((p_selfcap_measure_data->measurement_done_touch == 1u))
		 * for self cap
		 * Touch sensor ON/OFF status or rotor/slider position.
		 *
		 * Self Cap method
		 * uint8_t sensor_state =
		 * GET_SELFCAP_SENSOR_STATE(SENSOR_NUMBER);
		 * uint8_t rotor_slider_position =
		 * GET_SELFCAP_ROTOR_SLIDER_POSITION(SENSOR_NUMBER);
		 *
		 */

		/**
		 * Mutual Cap method
		 * if ((p_mutlcap_measure_data->measurement_done_touch == 1u))
		 * for mutual cap
		 * Touch sensor ON/OFF status or rotor/slider position.
		 *
		 *
		 * uint8_t sensor_state =
		 * GET_MUTLCAP_SENSOR_STATE(SENSOR_NUMBER);
		 * uint8_t rotor_slider_position =
		 * GET_MUTLCAP_ROTOR_SLIDER_POSITION(SENSOR_NUMBER);
		 */
	}
}

void display_init(void)
{
	ssd1306_init();

	// 1/64 Duty (0x0F~0x3F)
	ssd1306_write_command(SSD1306_CMD_SET_MULTIPLEX_RATIO);
	ssd1306_write_command(0x3F);

	//ssd1306_write_command(SSD1306_CMD_SET_COM_OUTPUT_SCAN_UP);

	// Set COM Pins hardware configuration, based on example from display datasheet
	ssd1306_write_command(SSD1306_CMD_SET_COM_PINS);
	ssd1306_write_command(0x12);

	// set horizontal addressing mode
	ssd1306_write_command(SSD1306_CMD_SET_MEMORY_ADDRESSING_MODE);
	ssd1306_write_command(1);

	// set contrast, based on example from display datasheet
	ssd1306_write_command(SSD1306_CMD_SET_CONTRAST_CONTROL_FOR_BANK0);
	ssd1306_write_command(0xCF);

	// set normal display
	ssd1306_write_command(SSD1306_CMD_SET_NORMAL_DISPLAY);
	
	memset(display_buffer, 0x00, DISP_MEM_SIZE);
	ssd1306_write_data_n(display_buffer, DISP_MEM_SIZE);

	UG_Init(&gui, display_set_pixel, 128, 64);
	UG_SelectGUI(&gui);
	UG_FontSelect(&FONT_6X8);
	//UG_PutChar('a', 0, 0, 1, 0);
	//UG_PutChar('B', 0, 8, 1, 0);
	
	//char str[256];
	//sprintf(str, "0x%02X", 0x2);
	//sprintf(str, "Hello World! Hello World! Hello World!");
	//UG_PutString(0, 16, str);
	
	ssd1306_write_data_n(display_buffer, DISP_MEM_SIZE);
}

void display_set_pixel(UG_S16 x, UG_S16 y, UG_COLOR color)
{
	if ((x >= DISP_SIZE_X) || (y >= DISP_SIZE_Y) || (x < 0) || (y < 0))
	{
		return; // error
	}
	
	if (color)
	{
		display_buffer[((x*DISP_SIZE_X)>>4) + (y>>3)] |= (1<<(y%8));
	}
	else
	{
		display_buffer[((x*DISP_SIZE_X)>>4) + (y>>3)] &= ~(1<<(y%8));
	}
}

void i2c_init(void)
{
	struct i2c_master_config config_i2c_master;
	i2c_master_get_config_defaults(&config_i2c_master);
	config_i2c_master.buffer_timeout = 10000;
	config_i2c_master.baud_rate = I2C_MASTER_BAUD_RATE_400KHZ;
	config_i2c_master.pinmux_pad0 = I2C_SDA_PINMUX;
	config_i2c_master.pinmux_pad1 = I2C_SCL_PINMUX;
	i2c_master_init(&i2c_master_instance, I2C_MODULE, &config_i2c_master);
	i2c_master_enable(&i2c_master_instance);
}

void timer_init(void)
{
	struct tc_config config_tc;
	tc_get_config_defaults(&config_tc);

	config_tc.clock_source    = GCLK_GENERATOR_4;
	config_tc.counter_size    = TC_COUNTER_SIZE_32BIT;
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV1;
	config_tc.run_in_standby  = true;

	tc_init(&tc_instance, TC_TIMESTAMP, &config_tc);

	g_us_timestamp_upper_32bit = 0;
	tc_enable(&tc_instance);
	
	tc_register_callback(
		&tc_instance, 
		tc_callback_overflow, 
		TC_CALLBACK_CC_CHANNEL0);

	tc_enable_callback(&tc_instance, TC_CALLBACK_CC_CHANNEL0);
}

void tc_callback_overflow(void)
{
	g_us_timestamp_upper_32bit++;
}

void measure_nonPD_current(void)
{
	char str[256];
	int cc1, cc2;
	
	cc1 = 0;
	cc2 = 0;
	tcpm_get_cc(0, &cc1, &cc2);
	
	sprintf(str, "Non-PD Current");
	UG_PutString(0, 8, str);
	if ((cc1 == TYPEC_CC_VOLT_SNK_DEF) || (cc2 == TYPEC_CC_VOLT_SNK_DEF))
	{
		sprintf(str, "Default USB Power");
	}
	else if ((cc1 == TYPEC_CC_VOLT_SNK_1_5) || (cc2 == TYPEC_CC_VOLT_SNK_1_5))
	{
		sprintf(str, "Type-C 1.5 A");
	}
	else if ((cc1 == TYPEC_CC_VOLT_SNK_3_0) || (cc2 == TYPEC_CC_VOLT_SNK_3_0))
	{
		sprintf(str, "Type-C 3.0 A");
	}
	else
	{
		sprintf(str, "Unknown Error");
	}
	UG_PutString(0, 16, str);
	ssd1306_write_data_n(display_buffer, DISP_MEM_SIZE);
}