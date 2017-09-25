/* @owner ICosta
 *
 * Copyright of this program is the property of Ivo Costa, without
 * whose written permission reproduction in whole or in part is prohibited.
 */
/**
 * @file Nixe.h
 *
 * @brief Prototypes for Nixie Clock Services
 *
 * @author I. Costa
 * @date 2015-17-10
 */
 
#ifndef __NIXIE_H__
#define __NIXIE_H__

 /* -- Includes */

/* Associated header */

/* Standard headers */  
#include <time.h>
#include <stdbool.h>

/* Functional headers */
#include "simplelink.h" /** UINT8 */

/* -- Constants */
enum Dots
{
	DotsOff		= 0,
  DotsOn		= 1,
  BlinkDots = 3,
	LeftDots 	= 0x20,
  RightDots	= 0x08,
  BoothDots = 0x28,
};

/* -- Variables */

/* -- Macros */
#define NbEffects 3

/* -- Types */

/**
 * @brief Initializes the Nixie Clock.
 *
 * @return None
 */
void NixieClock_Init(void);
 
/**
 * @brief Toggle Dots.
 *
 * @param[in]				Dots 
 * @param[in]				Action
 *
 * @return None
 */
void NixieDots_toggle(uint8_t dots, uint8_t action);

/**
 * @brief Display Time on Nixie Tubes
 *
 * @param[in]				First2digits to Display  
 * @param[in]				Middle2digits to Display    
 * @param[in]				Last2digits to Display  
 *
 * @return None
 */
void NixieDisplay_Time(uint16_t Hours, uint16_t Minuts, uint16_t Seconds);

/**
 * @brief Display Date on Nixie Tubes
 *	
 * @param[in]				Day to Display  
 * @param[in]				Month to Display    
 * @param[in]				Year to Display  
 *
 * @return None
 */
void NixieDisplay_Date(uint16_t Day, uint16_t Month, uint16_t Year);

/**
 * @brief Display Effects on Nixie Tubes 
 *
 * @param[in]				Effect Number to Display 
 *
 * @return None
 */
void NixieDisplay_Effects(uint8_t Effects);
																			 
#endif /* __NIXIE_H__ */
