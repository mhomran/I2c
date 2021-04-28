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

//Status register codes:

//master transmitter

#define I2C_SR_MT_STA 0x08 /**< the start bit is sent successfully */
#define I2C_SR_MT_AACK 0x18 /**< ACK is received after sending the address */
#define I2C_SR_MT_ACK 0x28 /**< ACK is received after sending a byte */
#define I2C_SR_MT_RSTA 0x10 /**< the restart bit is sent successfully */

#define I2C_PRESCALER_NUM 4 /**< Number of the prescalers */

/**
 * @brief The difference between two successive prescalers.
 */
#define I2C_PRESCALER_STEP 4

/**
 * @brief Convert from a frequency and prescaler to a register value.
 * This formula is described in the datasheet.
 */
#define I2C_FREQ_TO_REG(__FREQUENCY__, __PRESCALER__) \
(uint32_t)(((SYSTEM_CLK / __FREQUENCY__) - 16) / (2 * __PRESCALER__));
/******************************************************************************
 * Includes
 ******************************************************************************/
#include <inttypes.h>
#include "i2c.h"
#include "i2c_memmap.h"
/******************************************************************************
 * typedefs 
 ******************************************************************************/
typedef enum {
  I2C_FLAG_STA,    /**< Start bit is sent successfully */
  I2C_FLAG_ACK,   /**< Acknowledge is received/sent */
}I2cFlag_t;
/******************************************************************************
 * module variables definitions
 ******************************************************************************/
static volatile uint8_t* const gControlReg[I2C_MAX] =
{
  TWCR
};

static volatile uint8_t* const gBitrateReg[I2C_MAX] =
{
  TWBR
};

static volatile uint8_t* const gStatusReg[I2C_MAX] =
{
  TWSR
};

static volatile uint8_t* const gDataReg[I2C_MAX] =
{
  TWDR
};


/******************************************************************************
 * functions prototypes
 ******************************************************************************/
inline static uint8_t I2c_SetSclFreq(const I2c_t I2c, const uint32_t Frequency);
inline static void I2c_Enable(const I2c_t I2c);
inline static void I2c_SendStartBit(const I2c_t I2c);
inline static void I2c_SendStopBit(const I2c_t I2c);
inline static void I2c_WriteDataReg(const I2c_t I2c, const uint8_t Data);
static uint8_t I2C_WaitOnFlagUntilTimeout(const I2c_t I2c, const I2cFlag_t Flag);
/******************************************************************************
 * functions definitions
 ******************************************************************************/
/******************************************************************************
* Function : I2c_Init()
*//**
* \b Description:
* initialize the I2C peripherals <br>
* PRE-CONDITION: The SCL and SDA Pins are configured input with pull-up
* enabled <br>
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
      I2c_Enable(i);
    }
}

/******************************************************************************
* Function : I2c_SetSclFreq()
*//**
* \b Description:
* Utility function to set the SCL frequency <br>
* POST-CONDITION: The SCL frequency is set up <br>
* @param I2c the id of the I2c peripheral
* @param Frequency the frequency of the SCL in Hz. It must be less than or
* equal 400000 Hz.
* @return uint8_t 1 if the frequency is set up, 0 otherwise.
 ******************************************************************************/
inline static uint8_t 
I2c_SetSclFreq(const I2c_t I2c, const uint32_t Frequency)
{
  if(!(Frequency <= 400000ul))
    {
      return 0; 
    }

  uint32_t BitrateReg;
  uint8_t PrescalerIndex;
  uint8_t Prescaler;
  uint8_t i;

  for(PrescalerIndex = 0; PrescalerIndex < I2C_PRESCALER_NUM; PrescalerIndex++)
    {
      Prescaler = 1;
      for(i = 0; i < PrescalerIndex; i++)
        {
          Prescaler *= I2C_PRESCALER_STEP;
        }

      BitrateReg = I2C_FREQ_TO_REG(Frequency, Prescaler);
      if(BitrateReg < 255)
        {
          *(gBitrateReg[I2c]) = (uint8_t)BitrateReg;
          *(gStatusReg[I2c]) = PrescalerIndex;
          return 1;
        }
    }

  return 0;
}

/******************************************************************************
* Function : I2c_Enable()
*//**
* \b Description:
* Utility function to enable the peripheral <br>
* POST-CONDITION: The I2C peripheral clock is enabled <br>
* @param I2c the id of the I2c peripheral
* @return void
 ******************************************************************************/
inline static void
I2c_Enable(const I2c_t I2c)
{
  *(gControlReg[I2c]) |= 1 << TWEN;
}

/******************************************************************************
* Function : I2c_SendByte()
*//**
* \b Description: Write one byte into a device register using I2C <br>
* POST-CONDITION: A byte is saved inside the device register <br>
* @param I2c the id of the I2C peripheral
* @param Address the address of the register to write using I2C peripheral
* @param Data the byte to write 
* @return uint8_t 1 the operations is done successfully
*                 2 start bit error
*                 3 address error
*                 4 reg/data sending error
 ******************************************************************************/
extern uint8_t 
I2c_SendByte(const I2c_t I2c, 
             const uint8_t Address,
             const uint8_t Register, 
             const uint8_t Data)
{
  if(!(I2c < I2C_MAX)) return 0; 
    
  uint8_t res;

  I2c_SendStartBit(I2c);
  res = I2C_WaitOnFlagUntilTimeout(I2c, I2C_FLAG_STA);
  if(res == 0) return 2;

  I2c_WriteDataReg(I2c, (Address << 1) | I2C_WRITE);
  res = I2C_WaitOnFlagUntilTimeout(I2c, I2C_FLAG_ACK);
  if(res == 0) return 3;

  I2c_WriteDataReg(I2c, Register);
  res = I2C_WaitOnFlagUntilTimeout(I2c, I2C_FLAG_ACK);
  if(res == 0) return 4;
  
  I2c_WriteDataReg(I2c, Data);
  res = I2C_WaitOnFlagUntilTimeout(I2c, I2C_FLAG_ACK);
  if(res == 0) return 4;

  I2c_SendStopBit(I2c);

  return 1;
}

/******************************************************************************
* Function : I2C_WaitOnFlagUntilTimeout()
*//**
* \b Description: Utility function handles I2C Communication Timeout.
* It also clears the flag if set. <br>
* @param  I2c the id of the I2c peripheral
* @param  Flag flag to check.
* @return uint8_t 1 if there's no timeout, 0 otherwise
******************************************************************************/
static uint8_t 
I2C_WaitOnFlagUntilTimeout(const I2c_t I2c, const I2cFlag_t Flag)
{
  uint16_t Timeout = 0;
  uint8_t Status = 0;
  uint8_t StatusReg;
  uint8_t FinishOp;

  while (Status == 0 && Timeout < I2C_TIMEOUT)  
    {
      FinishOp = *(gControlReg[I2c]) & (1 << TWINT);
      StatusReg = *(gStatusReg[I2c]);
      //mask the first three bits which are not related to status.
      StatusReg &= 0xF8;

      switch(Flag) 
      {
        case I2C_FLAG_STA:
          if(FinishOp != 0 && StatusReg == I2C_SR_MT_STA)
            {
              Status = 1;
            }
        break;
        
        case I2C_FLAG_ACK:
          if(FinishOp != 0 && (StatusReg == I2C_SR_MT_AACK
              || StatusReg == I2C_SR_MT_ACK))
            {
              Status = 1;
            }
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
* @return void
******************************************************************************/
inline static void
I2c_SendStartBit(const I2c_t I2c)
{
  *(gControlReg[I2c]) |= 1 << TWSTA | 1 << TWINT;
}

/******************************************************************************
* Function : I2c_SendStopBit()
*//**
* \b Description: Utility function to send a stop bit on the I2c bus. <br>
* @param  I2c the id of the I2c peripheral
* @param  Flag flag to check.
* @return void
******************************************************************************/
inline static void
I2c_SendStopBit(const I2c_t I2c)
{
  *(gControlReg[I2c]) |= 1 << TWSTO | 1 << TWINT;
}

/******************************************************************************
* Function : I2c_SendStopBit()
*//**
* \b Description: Utility function to set a byte in the I2C data register
* to be send by the peripheral.<br>
* @param  I2c the id of the I2c peripheral
* @param  Flag flag to check.
* @return void
******************************************************************************/
inline static void
I2c_WriteDataReg(const I2c_t I2c, const uint8_t Data)
{
  *(gDataReg[I2c]) = Data;
  //clear flags for proper operation.
  *(gControlReg[I2c]) |= 1 << TWINT; 
  *(gControlReg[I2c]) &= ~(1 << TWSTA);
}

/*****************************End of File ************************************/
