 /* @owner ICosta
 *
 * Copyright of this program is the property of Ivo Costa, without
 * whose written permission reproduction in whole or in part is prohibited.
 */
/**
 * @file Rtc.c
 *
 * @brief Services for Rtc interface
 *
 * @author I. Costa
 * @date 2015-20-11
 */

/* -- Includes */
/* Associated header */
#include "Rtc.h"

/* Standard headers */
#include <stdarg.h>
#include <stdbool.h> 

/* Functional headers */
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#include "inc/hw_gpio.h"
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "Utils.h"

/* -- Constants */
#define SLAVE_ADDRESS 0x68  
#define SEC 0x00  
#define MIN 0x01  
#define HRS 0x02  
#define DAY 0x03  
#define DATE 0x04  
#define MONTH 0x05  
#define YEAR 0x06  
#define CNTRL 0x07 

/* -- Types */

/**
 * @brief Initializes RTC
 *
 * @return None
 */
void RTCInit(void){
  
   /** enable I2C1 peripheric */
   SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);  
   /** reset module */ 
   SysCtlPeripheralReset(SYSCTL_PERIPH_I2C1);  
   /** enable GPIO peripheral that contains I2C1 */
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);  
   /** Configure the pin for I2C1 functions on port A6 and A7. */  
   GPIOPinConfigure(GPIO_PA6_I2C1SCL);  
   GPIOPinConfigure(GPIO_PA7_I2C1SDA);  
   /** Select the I2C function for these pins. */
   GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);  
   GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);  
   /** Enable and initialize the I2C1 master module. Use the system clock for  */
   /** the I2C1 module. The last parameter sets the I2C data transfer rate.  */
   /** If false the data rate is set to 100kbps and if true the data rate will */
   /** be set to 400kbps. */
   I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), 0);  
		
   /** clear I2C FIFOs */  
   HWREG(I2C1_BASE + I2C_O_FIFOCTL) = 80008000 ;
	 
 } 
 
/**
 * @brief I2C1 Send Data
 *
 * @param[in]       SlaveAdress		  The Slave Adress to comunicate    
 * @param[in]       Adress					Adress to Send Data
 * @param[in]       NbArgs				  Number of Arguments to send
 * @param[in]       Args				    Data to send   
 *
 * @return None
 */
static void I2CSend(uint8_t slave_addr, uint8_t num_of_args, ...)
{
    /** Tell the master module what address it will place on the bus when	communicating with the slave. */
    I2CMasterSlaveAddrSet(I2C1_BASE, slave_addr, 0);
     
    /** stores list of variable number of arguments */
    va_list vargs;
     
    /** specifies the va_list to "open" and the last fixed argument so vargs knows where to start looking */
    va_start(vargs, num_of_args);
     
    /** put data to be sent into FIFO */
    I2CMasterDataPut(I2C1_BASE, va_arg(vargs, uint32_t));
     
    /** if there is only one argument, we only need to use the single send I2C function */
    if(num_of_args == 1)
    {
        /** Initiate send of data from the MCU */
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
         
        /** Wait until MCU is done transferring. */
        while(I2CMasterBusy(I2C1_BASE));
         
        /** "close" variable argument list */
        va_end(vargs);
    }
     
    /** otherwise, we start transmission of multiple bytes on the I2C bus */
    else
    {
        /** Initiate send of data from the MCU */
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
         
        /** Wait until MCU is done transferring. */
        while(I2CMasterBusy(I2C1_BASE));
         
        /** send num_of_args-2 pieces of data, using the BURST_SEND_CONT command of the I2C module */
        for(uint8_t i = 1; i < (num_of_args - 1); i++)
        {
            /** put next piece of data into I2C FIFO */
            I2CMasterDataPut(I2C1_BASE, va_arg(vargs, uint32_t));
            /** send next data that was just placed into FIFO */
            I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
     
            /** Wait until MCU is done transferring. */
            while(I2CMasterBusy(I2C1_BASE));
        }
     
        /** put last piece of data into I2C FIFO */
        I2CMasterDataPut(I2C1_BASE, va_arg(vargs, uint32_t));
        /** send next data that was just placed into FIFO */
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
        /** Wait until MCU is done transferring. */
        while(I2CMasterBusy(I2C1_BASE));
         
        /** "close" variable args list */
        va_end(vargs);
    }
}	 

/**
 * @brief I2C1 Receive Data
 *
 * @param[in]       SlaveAdress		  Slave Adress to Read    
 * @param[in]       reg							Reg data    
 *
 * @return None
 */
static uint32_t I2CReceive(uint32_t slave_addr, uint8_t reg){  
   /** specify that we are writing (a register address) to the slave device */
   I2CMasterSlaveAddrSet(I2C1_BASE, slave_addr, 0);  
   /** specify register to be read */ 
   I2CMasterDataPut(I2C1_BASE, reg);  
   /**send control byte and register address byte to slave device */
   I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);  
   /** wait for MCU to finish transaction */
   while(I2CMasterBusy(I2C1_BASE));  
   /** specify that we are going to read from slave device */  
   I2CMasterSlaveAddrSet(I2C1_BASE, slave_addr, 1);  
   /** send control byte and read from the register we specified */ 
   I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);  
   /** wait for MCU to finish transaction */
   while(I2CMasterBusy(I2C1_BASE));  
   /** return data pulled from the specified register */  
   return I2CMasterDataGet(I2C1_BASE); 
	 
 }  

/**
 * @brief Set RTC Time
 *
 * @param[in]       psTime		  Time Date Structure      
 *
 * @return None
 */ 
 void SetRtcTime(const struct tm SetTime){ 
  
	 I2CSend(SLAVE_ADDRESS,4,SEC,
	 dec2bcd(SetTime.tm_sec),dec2bcd(SetTime.tm_min),dec2bcd(SetTime.tm_hour),dec2bcd(SetTime.tm_wday),
	 dec2bcd(SetTime.tm_mday),dec2bcd(SetTime.tm_mon),dec2bcd(SetTime.tm_year));   
 } 

 /**
 * @brief Set RTC Date
 *
 * @param[in]       psTime		  Time Date Structure      
 *
 * @return None
 */ 
 void SetRtcDate(const struct tm SetTime){ 
  
	 I2CSend(SLAVE_ADDRESS,5,DAY,dec2bcd(SetTime.tm_wday),dec2bcd(SetTime.tm_mday),
	 dec2bcd(SetTime.tm_mon),dec2bcd(SetTime.tm_year));   
 } 

/**
 * @brief Get Rtc Time/Date 
 *
 * @return None
 */  
void GetRtcTimeDate(struct tm *psTime){ 
	 
	psTime->tm_sec = bcd2dec(I2CReceive(SLAVE_ADDRESS,SEC));  
  psTime->tm_min = bcd2dec(I2CReceive(SLAVE_ADDRESS,MIN));  
	psTime->tm_hour = bcd2dec(I2CReceive(SLAVE_ADDRESS,HRS));   
	psTime->tm_wday = bcd2dec(I2CReceive(SLAVE_ADDRESS,DAY));  
	psTime->tm_mday = bcd2dec(I2CReceive(SLAVE_ADDRESS,DATE));  
	psTime->tm_mon = bcd2dec(I2CReceive(SLAVE_ADDRESS,MONTH));  
	psTime->tm_year = bcd2dec(I2CReceive(SLAVE_ADDRESS,YEAR)); 
	
}
