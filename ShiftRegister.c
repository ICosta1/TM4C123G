/* @owner ICosta
 *
 * Copyright of this program is the property of Ivo Costa, without
 * whose written permission reproduction in whole or in part is prohibited.
 */
/**
 * @file ShiftRegister.c
 *
 * @brief Services for Shift Register Interface
 *
 * @author I. Costa
 * @date 2015-10-10
 */

/* -- Includes */

/* Associated header */
#include "ShiftRegister.h"

/* Standard headers */
#include <stdio.h>           

/* Functional headers */
#include "Delay.h"							/** For delay Services */

#include "tm4c123gh6pm.h"

/* -- Constants */

/* -- Types */
unsigned int  bitOrder = LSBFIRST;

/**
 * @brief Strcuture of Shift Register Pins.
 */
typedef enum 
{
  DS 			= 0x02,
  ST_CP 	= 0x04,
  SH_CP 	= 0x08,
} ShifReg_t;

/* -- Variables */

/* -- Macros */

/* -- Static Function Prototypes */
static void SetPin(ShifReg_t Pin, unsigned int const State);


/**
 * @brief Initializes the Shift Register Driver.
 *
 * @return None
 */
void ShiftRegister_Init(unsigned int  bitOrder ){ 
	
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;     // 1) E Clock
  delay = SYSCTL_RCGC2_R;           				// 2) delay   
	
	/*GPIOE Configuratons*/
  GPIO_PORTE_AMSEL_R &= ~0x0E; 							// 3) disable analog function on PE1-3
  GPIO_PORTE_PCTL_R &= ~0x00FFFFFF; 				// 4) enable regular 
  GPIO_PORTE_DIR_R |= 0x0E;    							// 5) outputs on PE1-3
  GPIO_PORTE_AFSEL_R &= ~0x0E; 							// 6) regular function on PE1-3
  GPIO_PORTE_DEN_R |= 0x0E;    							// 7) enable digital on PE1-3

}


/**
 * @brief Output Data to Shift Register
 *
 * @param[in]       Data			Array with Bits to output to Shift Register
 * @param[in]       NBits			Number of bits to output(Max == Size of Data)       
 *
 * @return None
 */
void ShiftRegister_Output(unsigned int const Data[],unsigned int const NBits){
	
	int i=0; 
	
		SetPin(ST_CP, LOW);
		for (i = (NBits-1) ; i>=0; i--){
			SetPin(SH_CP, LOW);
			Delay_12ns(80);
			SetPin(DS, Data[i]);
			SetPin(SH_CP, HIGH);
		}
	 SetPin(ST_CP, HIGH);
	}


 
 /**
 * @brief Set Pin for Shift Register comunication Data to Shift Register
 *
 * @param[in]       Pin			Pin to Set
 * @param[in]       State		State to output (Low or High)       
 *
 * @return None
 */
static void SetPin(ShifReg_t Pin, unsigned int const State){
        
	if (State == LOW) {
		GPIO_PORTE_DATA_R &=~ Pin;	
  }else {
		GPIO_PORTE_DATA_R |= Pin;
  }

}
