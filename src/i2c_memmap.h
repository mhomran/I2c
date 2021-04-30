/**
 * @file i2c_memmap.h
 * @author Mohamed Hassanin
 * @brief A memory map for the I2C peripherals
 * @version 0.1
 * @date 2021-04-26
 */

#ifndef I2C_MEMMAP_H
#define I2C_MEMMAP_H

#define TWBR    ((volatile uint8_t*) 0x20)
#define TWSR    ((volatile uint8_t*) 0x21)
#define TWAR    ((volatile uint8_t*) 0x22)
#define TWDR    ((volatile uint8_t*) 0x23)

#define TWCR    ((volatile uint8_t*) 0x56)

/* TWCR */
#define TWINT   7
#define TWEA    6
#define TWSTA   5
#define TWSTO   4
#define TWWC    3
#define TWEN    2
/* bit 1 reserved */
#define TWIE    0

#endif
/*****************************End of File ************************************/
