/* @owner ICosta
 *
 * Copyright of this program is the property of Ivo Costa, without
 * whose written permission reproduction in whole or in part is prohibited.
 */
/**
 * @file Delay.h
 *
 * @brief Delay Services
 *
 * @author I. Costa
 * @date 2015-10-10
 */

/**
 * @brief Delay 12ns (1 clock Cycle at 80MHZ Sysclk)
 * 
 * @param[in]				NbDelay	Number of peiod to delay (12ns)      
 *
 * @return None
 */
void Delay_12ns(unsigned long count);

/**
 * @brief Delay 1ms
 * 
 * @param[in]				NbDelay	Number of peiod to delay (1ms)      
 *
 * @return None
 */
void Delay_1ms(unsigned long count);

/**
 * @brief Delay 100ms
 * 
 * @param[in]				NbDelay	Number of peiod to delay (100ms)      
 *
 * @return None
 */
void Delay_100ms(unsigned long count);
