/**
 * \file
 *
 * \brief SAM DMA cyclic redundancy check (CRC) Driver
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#ifndef DMA_CRC_H_INCLUDED
#define DMA_CRC_H_INCLUDED

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/** DMA channel n offset. */
#define DMA_CRC_CHANNEL_N_OFFSET 0x20

/** CRC Polynomial Type. */
enum crc_polynomial_type {
	/** CRC16 (CRC-CCITT). */
	CRC_TYPE_16,
	/** CRC32 (IEEE 802.3). */
	CRC_TYPE_32,
};

/** CRC Beat Type. */
enum crc_beat_size {
	/** Byte bus access. */
	CRC_BEAT_SIZE_BYTE,
	/** Half-word bus access. */
	CRC_BEAT_SIZE_HWORD,
	/** Word bus access. */
	CRC_BEAT_SIZE_WORD,
};

/** Configurations for CRC calculation. */
struct dma_crc_config {
	/** CRC polynomial type. */
	enum crc_polynomial_type type;
	/** CRC beat size. */
	enum crc_beat_size size;
};

/**
 * \brief Get DMA CRC default configurations.
 *
 * The default configuration is as follows:
 *  \li Polynomial type is set to CRC-16(CRC-CCITT)
 *  \li CRC Beat size: BYTE
 *
 * \param[in] config default configurations
 */
static inline void dma_crc_get_config_defaults(struct dma_crc_config *config)
{
	Assert(config);

	config->type = CRC_TYPE_16;
	config->size = CRC_BEAT_SIZE_BYTE;
}

/**
 * \brief Enable DMA CRC module with an DMA channel.
 *
 * This function enables a CRC calculation with an allocated DMA channel. This channel ID
 * can be gotten from a successful \ref dma_allocate.
 *
 * \param[in] channel_id DMA channel expected with CRC calculation
 * \param[in] config CRC calculation configurations
 *
 * \return Status of the DMC CRC.
 * \retval STATUS_OK Get the DMA CRC module
 * \retval STATUS_BUSY DMA CRC module is already taken and not ready yet
 */
static inline enum status_code dma_crc_channel_enable(uint32_t channel_id,
		struct dma_crc_config *config)
{
	if (DMAC->CRCSTATUS.reg & DMAC_CRCSTATUS_CRCBUSY) {
		return STATUS_BUSY;
	}

	DMAC->CRCCTRL.reg = DMAC_CRCCTRL_CRCBEATSIZE(config->size) |
		DMAC_CRCCTRL_CRCPOLY(config->type) |
		DMAC_CRCCTRL_CRCSRC(channel_id+DMA_CRC_CHANNEL_N_OFFSET);

	DMAC->CTRL.reg |= DMAC_CTRL_CRCENABLE;

	return STATUS_OK;
}

/**
 * \brief Disable DMA CRC module.
 *
 */
static inline void dma_crc_disable(void)
{
	DMAC->CTRL.reg &= ~DMAC_CTRL_CRCENABLE;
	DMAC->CRCCTRL.reg = 0;
}

/**
 * \brief Get DMA CRC checksum value.
 *
 * \return Calculated CRC checksum.
 */
static inline uint32_t dma_crc_get_checksum(void)
{
	if (DMAC->CRCCTRL.bit.CRCSRC == DMAC_CRCCTRL_CRCSRC_IO_Val) {
		DMAC->CRCSTATUS.reg = DMAC_CRCSTATUS_CRCBUSY;
	}

	return DMAC->CRCCHKSUM.reg;
}

/**
 * \brief Enable DMA CRC module with I/O.
 *
 * This function enables a CRC calculation with I/O mode.
 *
 * \param[in] config CRC calculation configurations.
 *
 * \return Status of the DMC CRC.
 * \retval STATUS_OK Get the DMA CRC module
 * \retval STATUS_BUSY DMA CRC module is already taken and not ready yet
 */
static inline enum status_code dma_crc_io_enable(
		struct dma_crc_config *config)
{
	if (DMAC->CRCSTATUS.reg & DMAC_CRCSTATUS_CRCBUSY) {
		return STATUS_BUSY;
	}

	if (DMAC->CTRL.reg & DMAC_CTRL_CRCENABLE) {
		return STATUS_BUSY;
	}

	DMAC->CRCCTRL.reg = DMAC_CRCCTRL_CRCBEATSIZE(config->size) |
		DMAC_CRCCTRL_CRCPOLY(config->type) |
		DMAC_CRCCTRL_CRCSRC_IO;

	if (config->type == CRC_TYPE_32) {
		DMAC->CRCCHKSUM.reg = 0xFFFFFFFF;
	}

	DMAC->CTRL.reg |= DMAC_CTRL_CRCENABLE;

	return STATUS_OK;
}

/**
 * \brief Calculate CRC with I/O.
 *
 * This function calculate the CRC of the input data buffer.
 *
 * \param[in] buffer CRC Pointer to calculation buffer
 * \param[in] total_beat_size Total beat size to be calculated
 *
 * \return Calculated CRC checksum value.
 */
static inline void dma_crc_io_calculation(void *buffer,
		 uint32_t total_beat_size)
{
	uint32_t counter = total_beat_size;
	uint8_t *buffer_8;
	uint16_t *buffer_16;
	uint32_t *buffer_32;

	for (counter=0; counter<total_beat_size; counter++) {
		if (DMAC->CRCCTRL.bit.CRCBEATSIZE == CRC_BEAT_SIZE_BYTE) {
			buffer_8 = buffer;
			DMAC->CRCDATAIN.reg = buffer_8[counter];
		} else if (DMAC->CRCCTRL.bit.CRCBEATSIZE == CRC_BEAT_SIZE_HWORD) {
			buffer_16 = buffer;
			DMAC->CRCDATAIN.reg = buffer_16[counter];
		} else if (DMAC->CRCCTRL.bit.CRCBEATSIZE == CRC_BEAT_SIZE_WORD) {
			buffer_32 = buffer;
			DMAC->CRCDATAIN.reg = buffer_32[counter];
		}
		/* Wait several cycle to make sure CRC complete */
		nop();
		nop();
		nop();
		nop();
	}
}

#ifdef __cplusplus
}
#endif

#endif /* DMA_CRC_H_INCLUDED */
