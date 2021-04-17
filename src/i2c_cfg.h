/**
 * @file i2c_cfg.h
 * @author Mohamed Hassanin
 * @brief I2C driver configuration header file.
 * @version 0.1
 * @date 2021-04-17
 */

#ifndef I2C_CFG_H
#define I2C_CFG_H
/******************************************************************************
 * Includes
 ******************************************************************************/
#include <inttypes.h>
/******************************************************************************
 * Typedefs
 ******************************************************************************/
/**
* Defines an enumerated list of all the I2C pripherals on the MCU
* device. The last element is used to specify the maximum number of
* enumerated labels.
*/
typedef enum
{
  /* TODO: Populate this list based on the MCU */
  I2C_0,
  I2C_MAX
}I2c_t;

typedef struct
{
  I2c_t I2c; /**< the I2c peripheral id */
  uint32_t Speed; /**< the speed of the I2C SCL clock rate in Hz (max 400KHz) */
}I2cConfig_t;
/******************************************************************************
 * Function prototypes
 ******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

extern const I2cConfig_t* I2c_GetConfig(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
/*****************************End of File ************************************/
