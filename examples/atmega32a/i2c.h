/**
 * @file i2c.h
 * @author Mohamed Hassanin
 * @brief I2C driver header file.
 * @version 0.1
 * @date 2021-04-17
 */
#ifndef I2C_H
#define I2C_H
/******************************************************************************
 * Includes
 ******************************************************************************/
#include "i2c_cfg.h"
/******************************************************************************
 * Function prototypes
 ******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

extern void I2c_Init(const I2cConfig_t * const Config);
extern uint8_t I2c_SendByte(const I2c_t I2c, 
                            const uint8_t Address,
                            const uint8_t Register, 
                            const uint8_t Data);
extern uint8_t I2c_ReceiveByte(const I2c_t I2c, 
                               const uint8_t Address,
                               const uint8_t Register, 
                               uint8_t* const Data);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
/*****************************End of File ************************************/
