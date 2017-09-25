/**
 * @file Interface.h
 *
 * @brief Prototypes Interface Services
 *
 * @author I. Costa
 * @date 2016-10-02
 */

#ifndef __INTERFACE_H__
#define __INTERFACE_H__
/* -- Includes */

/* Associated header */

/* Standard headers */       
#include <time.h>

/* Functional headers */
#include "simplelink.h" /** UINT8 */
#include <stdbool.h>

/* -- Constants */
enum Action
{
	NoAction = 0,
  SW1SmallPress,
  SW1LongPress,
	SW2SmallPress,
  SW2LongPress,
};
/* -- Types */

 /**
 * @brief Initializes the Buttons interface
 *
 * @return None
 */
void Buttons_Init(void); 

 /**
 * @brief Acknowledege SW
 *
 * @return None
 */
void Sw_Ack(void);

/**
 * @brief Get Switch Ation
 *
 * @return Switch Action
 */
uint8_t Get_Switch_Action(void);

#endif /* __INTERFACE_H__ */

