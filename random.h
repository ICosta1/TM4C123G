/* @owner ICosta
 *
 * Copyright of this program is the property of Ivo Costa, without
 * whose written permission reproduction in whole or in part is prohibited.
 */
/**
 * @file Random.h
 *
 * @brief Prototypes for Random Generator
 *
 * @author I. Costa
 * @date 2015-17-10
 */
 
#ifndef __RANDOM_H__
#define __RANDOM_H__

 /* -- Includes */

/* Associated header */

/* Standard headers */       
//#include <stdint.h>
#include "simplelink.h" /** UINT8 */

/* Functional headers */

/* -- Constants */

/* -- Variables */

/* -- Macros */

/* -- Types */


/**
 * @brief Random_Init
 *				Initialize the random number generator with the given seed.
 *
 * @param[in]       seed						32-bit number to initialize random generator
 *
 * @return None
 */
 void Random_Init(uint32_t seed);

/**
 * @brief Generate a semi-random number The lower bits might not be random, so right 
 *				shift by some amount before scaling to the desired range.
 *
 * @return 32-bit semi-random number
 */
unsigned long Random(void);

/**
 * @brief Generate a semi-random number, on a given Range.
 *
 * @param[in]       RandomRange				Range of the Generator
 *
 * @return 32-bit semi-random number
 */
 unsigned long RandomX(unsigned int RandomRange);

#endif /* __RANDOM_H__ */
