/* @owner ICosta
 *
 * Copyright of this program is the property of Ivo Costa, without
 * whose written permission reproduction in whole or in part is prohibited.
 */
/**
 * @file Random.c
 *
 * @brief Linear congruential generator from Numerical Recipes
 *
 * @author I. Costa
 * @date 2015-16-10
 */

/* -- Includes */
/* Associated header */
#include "Random.h"

/* Standard headers */           

/* Functional headers */

/* -- Constants */

/* -- Types */
uint32_t M;


/**
 * @brief Random_Init
 *				Initialize the random number generator with the given seed.
 *
 * @param[in]       seed						32-bit number to initialize random generator
 *
 * @return None
 */
void Random_Init(uint32_t seed){
  M = seed;
}


/**
 * @brief Generate a semi-random number The lower bits might not be random, so right 
 *				shift by some amount before scaling to the desired range.
 *
 * @return 32-bit semi-random number
 */
unsigned long Random(void){
  M = 1664525*M+1013904223;
  return(M);
}

/**
 * @brief Generate a semi-random number, on a given Range.
 *
 * @param[in]       RandomRange				Range of the Generator
 *
 * @return 32-bit semi-random number
 */
 unsigned long RandomX(unsigned int RandomRange){
  return ((Random()>>24)%RandomRange);  
}
