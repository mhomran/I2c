/**
 * @file i2c_cfg.c
 * @author Mohamed Hassanin
 * @brief I2C driver configuration file.
 * @version 0.1
 * @date 2021-04-17
 */

/*****************************************************************************
* Includes
*****************************************************************************/
#include "i2c_cfg.h"

/*****************************************************************************
* Module Variable Definitions
*****************************************************************************/
/**
* The following array contains the configuration data for each
* I2C Peripheral. Each row represents I2C peripheral. Each column is
* representing a member of the I2cConfig_t
* structure. This table is read in by I2c_Init, where each channel is then
* set up based on this table.
*/
static const I2cConfig_t I2cConfig[] =
{
  //TODO: configure your UART peripherals
  { I2C_0, 100000 }
};
/******************************************************************************
* Function Definitions
*****************************************************************************/
/******************************************************************************
* Function : I2c_GetConfig()
*//**
* \b Description:
* This function is used to get the cofiguration handle of the I2C <br>
* POST-CONDITION: A constant pointer to the first member of the
* configuration table will be returned. <br>
* @return A pointer to the configuration table.
*
* \b Example Example:
* @code
* const I2c_ConfigType* I2cConfig = I2c_GetConfig();
* I2c_Init(I2cConfig);
* @endcode
* @see I2c_Init
 ******************************************************************************/
extern const I2cConfig_t * 
I2c_GetConfig(void)
{
  /*
  * The cast is performed to ensure that the address of the first element
  * of configuration table is returned as a constant pointer and NOT a
  * pointer that can be modified.
  */
  return (const I2cConfig_t *) I2cConfig;
}
/*****************************End of File ************************************/