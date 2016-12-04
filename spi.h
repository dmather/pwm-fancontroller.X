/* 
 * File:   spi.h
 * Author: daniel
 *
 * Created on November 13, 2016, 8:36 PM
 */

#ifndef SPI_H
#define	SPI_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define DUMMY_DATA 0x0

void SPI1_Initialize(void);

uint8_t SPI1_Exchange8bit(uint8_t data);

uint8_t SPI1_Exchange8bitBuffer(uint8_t *dataIn, uint8_t bufLen, uint8_t *dataOut);

bool SPI1_IsBufferFull(void);

bool SPI1_HasWriteCollisionOccured(void);

void SPI1_ClearWriteCollisionStatus(void);

#endif	/* SPI_H */

