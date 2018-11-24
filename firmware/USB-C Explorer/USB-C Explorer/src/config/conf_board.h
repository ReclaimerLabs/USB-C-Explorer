/**
 * \file
 *
 * \brief User board configuration template
 *
 * Copyright (C) 2013-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
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
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

#define SW_USER_EIC_PIN			PIN_PA15A_EIC_EXTINT15
#define SW_USER_EIC_MUX			MUX_PA15A_EIC_EXTINT15
#define SW_USER_EIC_PINMUX		PINMUX_PA15A_EIC_EXTINT15
#define SW_USER_EIC_LINE		15
#define SW_USER_PIN				PIN_PA15

#define I2C_MODULE				SERCOM3
#define I2C_SDA_PIN				PIN_PA22
#define I2C_SCL_PIN				PIN_PA23
#define I2C_SDA_PINMUX          PINMUX_PA22C_SERCOM3_PAD0
#define I2C_SCL_PINMUX          PINMUX_PA23C_SERCOM3_PAD1

#define USB20_DM_PIN			PIN_PA24
#define USB20_DP_PIN			PIN_PA25
#define USBC_INT_PIN			PIN_PA19
//#define USBC_INT_PINMUX         PINMUX_PA19A_EIC_EXTINT3

#define DISP_SIZE_X				128
#define DISP_SIZE_Y				64
#define DISP_MEM_SIZE			1024

#define TC_TIMESTAMP			TC4

#endif // CONF_BOARD_H
