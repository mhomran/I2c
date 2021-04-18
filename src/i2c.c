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
#define SYSTEM_CLK (12000000ul) /**< TODO: the system clock */

#define I2C_WRITE 0 /**< A mask to OR with the address for write operation */
#define I2C_READ 1 /**< A mask to OR with the address for read operation */
/******************************************************************************
 * Includes
 ******************************************************************************/
#include <inttypes.h>
#include "i2c.h"
/******************************************************************************
 * typedefs 
 ******************************************************************************/
typedef enum {
  I2C_FLAG_SB,    /**< Start bit is sent successfully */
  I2C_FLAG_ACK,   /**< Acknowledge is received/sent */
}I2cFlag_t;
/******************************************************************************
 * functions prototypes
 ******************************************************************************/
inline static uint8_t I2c_SetSclFreq(const I2c_t I2c, const uint32_t Freq);
inline static uint8_t I2c_Enable(const I2c_t I2c);
inline static uint8_t I2c_SendStartBit(const I2c_t I2c);
inline static uint8_t I2c_SendStopBit(const I2c_t I2c);
inline static uint8_t I2c_WriteDataReg(const I2c_t I2c, const uint8_t Data);
static uint8_t I2C_WaitOnFlagUntilTimeout(const I2c_t I2c, const I2cFlag_t Flag);
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
I2c_SetSclFreq(const I2c_t I2c, const uint32_t Freq)
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
I2c_Enable(const I2c_t I2c)
{
  if(!(I2c < I2C_MAX))
    {
      return 0; 
    }
  //TODO: implement
  return 1;
}

/******************************************************************************
* Function : I2c_SendByte()
*//**
* \b Description: Write one byte into a device register using I2C <br>
* POST-CONDITION: A byte is saved inside the device register <br>
* @param I2c the id of the I2C peripheral
* @param Address the address of the register to write using I2C peripheral
* @param Data the byte to write 
* @return uint8_t 1 if everything is okay, 0 otherwise.
 ******************************************************************************/
extern uint8_t 
I2c_SendByte(const I2c_t I2c, 
             const uint8_t Address,
             const uint8_t Register, 
             const uint8_t Data)
{
  if(!(I2c < I2C_MAX)) return 0; 
    
  uint16_t Timeout;
  uint8_t res;

  I2c_SendStartBit(I2c);
  res = I2C_WaitOnFlagUntilTimeout(I2c, I2C_FLAG_SB);
  if(res == 0) return 0;

  I2c_WriteDataReg(I2c, (Address << 1) | I2C_WRITE);
  res = I2C_WaitOnFlagUntilTimeout(I2c, I2C_FLAG_ACK);
  if(res == 0) return 0;

  I2c_WriteDataReg(I2c, Register);
  res = I2C_WaitOnFlagUntilTimeout(I2c, I2C_FLAG_ACK);
  if(res == 0) return 0;
  
  I2c_WriteDataReg(I2c, Data);
  res = I2C_WaitOnFlagUntilTimeout(I2c, I2C_FLAG_ACK);
  if(res == 0) return 0;

  I2c_SendStopBit(I2c);

  return 1;
}


/******************************************************************************
* Function : I2C_WaitOnFlagUntilTimeout()
*//**
* \b Description: Utility function handles I2C Communication Timeout. It also clears
* the flag if set. <br>
* @param  I2c the id of the I2c peripheral
* @param  Flag flag to check.
* @return uint8_t 1 if there's no timeout, 0 otherwise
******************************************************************************/
static uint8_t 
I2C_WaitOnFlagUntilTimeout(const I2c_t I2c, const I2cFlag_t Flag)
{
  uint16_t Timeout = 0;
  uint8_t Status = 0;
   
  while (Status == 0 && Timeout < I2C_TIMEOUT)  
    {
      //TODO: implement
      switch(Flag) 
      {
        case I2C_FLAG_SB:
        break;
        
        case I2C_FLAG_ACK:
        break;

        default:
        break;
      }
      Timeout++;
    }

  if(Timeout == I2C_TIMEOUT) return 0;
  return 1;
}

/******************************************************************************
* Function : I2c_SendStartBit()
*//**
* \b Description: Utility function to send a start bit on the I2c bus. <br>
* @param  I2c the id of the I2c peripheral
* @param  Flag flag to check.
* @return uint8_t 1 if there's no timeout, 0 otherwise
******************************************************************************/
inline static uint8_t 
I2c_SendStartBit(const I2c_t I2c)
{
  //TODO: implement
}

/******************************************************************************
* Function : I2c_SendStopBit()
*//**
* \b Description: Utility function to send a stop bit on the I2c bus. <br>
* @param  I2c the id of the I2c peripheral
* @param  Flag flag to check.
* @return uint8_t 1 if there's no timeout, 0 otherwise
******************************************************************************/
inline static uint8_t 
I2c_SendStopBit(const I2c_t I2c)
{
  //TODO: implement
}

/******************************************************************************
* Function : I2c_SendStopBit()
*//**
* \b Description: Utility function to set a byte in the I2C data register
* to be send by the peripheral.<br>
* @param  I2c the id of the I2c peripheral
* @param  Flag flag to check.
* @return uint8_t 1 if there's no timeout, 0 otherwise
******************************************************************************/
inline static uint8_t 
I2c_WriteDataReg(const I2c_t I2c, const uint8_t Data)
{
  //TODO: implement
}

/*****************************End of File ************************************/
