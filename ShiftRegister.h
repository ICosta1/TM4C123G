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

#ifndef __SHIFTREGISTER_H__
#define __SHIFTREGISTER_H__

/* -- Includes */

/* Type headers */

/* Functional headers */

/* -- Constants */
#define HIGH			1
#define LOW				0
#define LSBFIRST 	0 
#define MSBFIRST 	1
#define MaxNBits	24		


/**
 * @brief Initializes the Shift Register Driver.
 *
 * @return None
 */
void ShiftRegister_Init(unsigned int  bitOrder );

/**
 * @brief Output Data to Shift Register
 *
 * @param[in]       Data			Array with Bits to output to Shift Register
 * @param[in]       NBits			Number of bits to output(Max == Size of Data)       
 *
 * @return None
 */
void ShiftRegister_Output(unsigned const Data[],unsigned int const NBits);

#endif /* __SHIFTREGISTER_H__ */
