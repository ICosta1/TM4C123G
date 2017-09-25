/* @owner ICosta
 *
 * Copyright of this program is the property of Ivo Costa, without
 * whose written permission reproduction in whole or in part is prohibited.
 */
/**
 * @file Timer.h
 *
 * @brief Prototypes for Timer initialization Services
 *
 * @author I. Costa
 * @date 2015-16-10
 */

#ifndef __TIMER_H__
#define __TIMER_H__

/* Private function prototypes -----------------------------------------------*/

/**
 * @brief Timer0A Init.
 *
 * @param[in]       Period			Interrupt Period
 *
 * @return None
 */
void Timer0A_Init(unsigned long Period);

/**
 * @brief Enable All Interrupts.
 *
 * @return None
 */
void Enable_Interrupts(void);

/**
 * @brief Disable All Interrupts.
 *
 * @return None
 */
void Disable_Interrupts(void);


#endif /* __TIMER_H__ */
