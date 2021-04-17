/**
 * @file i2c.c
 * @author Mohamed Hassanin
 * @brief I2C driver header file.
 * @version 0.1
 * @date 2021-04-17
 */
/******************************************************************************
 * Definitions
 ******************************************************************************/
#define SYS_CLK (12000000ul) /**< TODO: the system clock */

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <inttypes.h>
#include "i2c.h"
/******************************************************************************
 * functions prototypes
 ******************************************************************************/
inline static uint8_t I2c_SetSclFreq(I2c_t I2c, uint32_t Freq);
inline static uint8_t I2c_Enable(I2c_t I2c);
/******************************************************************************
 * functions definitions
 ******************************************************************************/
/******************************************************************************
* Function : I2c_Init()
*//**
* \b Description:
* initialize the I2C peripherals <br>
* PRE-CONDITION: The SCL and SDA Pins are configured properly <br>
* PRE-CONDITION: I2C peripherals clocks are enabled <br>
* POST-CONDITION: I2C driver is set up <br>
* @return A pointer to the configuration table.
 ******************************************************************************/
extern void 
I2c_Init(const I2cConfig_t * const Config)
{
  if(!(Config != 0x0))
    {
      //TODO: handle this error
      return;
    }

  uint8_t i;
  uint8_t res;

  for(i = 0; i < I2C_MAX; i++)
    {
      res = I2c_SetSclFreq(i, Config[i].Speed);
      if(res == 0) 
        {
          //TODO: handle this error
          return;
        }
      res = I2c_Enable(i);
      if(res == 0) 
        {
          //TODO: handle this error
          return;
        }
    }
}

/******************************************************************************
* Function : I2c_SetSclFreq()
*//**
* \b Description:
* Utility function to set the SCL frequency <br>
* POST-CONDITION: The SCL frequency is set up <br>
* @param I2c the id of the I2c peripheral
* @param Freq the frequency of the SCL.
* @return uint8_t 1 if everything is okay, 0 otherwise.
 ******************************************************************************/
inline static uint8_t 
I2c_SetSclFreq(I2c_t I2c, uint32_t Freq)
{
  if(!(I2c < I2C_MAX && Freq < 400000ul))
    {
      return 0; 
    }

  //TODO: implement

  return 1;
}

/******************************************************************************
* Function : I2c_Enable()
*//**
* \b Description:
* Utility function to enable the peripheral <br>
* POST-CONDITION: The I2C peripheral clock is enabled <br>
* @param I2c the id of the I2c peripheral
* @return uint8_t 1 if everything is okay, 0 otherwise.
 ******************************************************************************/
inline static uint8_t 
I2c_Enable(I2c_t I2c)
{
  if(!(I2c < I2C_MAX))
    {
      return 0; 
    }
  //TODO: implement
  return 1;
}

/*****************************End of File ************************************/
