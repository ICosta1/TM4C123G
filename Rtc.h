/* @owner ICosta
 *
 * Copyright of this program is the property of Ivo Costa, without
 * whose written permission reproduction in whole or in part is prohibited.
 */
/**
 * @file Rtc.h
 *
 * @brief Prototypes for Rtc interface Services
 *
 * @author I. Costa
 * @date 2015-20-11
 */

#ifndef __RTC_H__
#define __RTC_H__

/* -- Includes */

/* Standard headers */    
#include <stdbool.h>

#include <time.h> 

/* Functional headers */
#include "simplelink.h"


/* -- Constants */

/* -- Types */

/**
 * @brief Initializes I2C1
 *
 * @return None
 */
void RTCInit(void);
 
/**
 * @brief Set RTC Date 
 *
 * @return None
 */ 
void SetRtcDate(const struct tm SetTime);

 /**
 * @brief Set RTC Time
 *
 * @param[in]       psTime		  Time Date Structure      
 *
 * @return None
 */ 
 void SetRtcTime(const struct tm SetTime);
	 
/**
 * @brief Get Rtc Time/Date   
 *
 * @return None
 */  
void GetRtcTimeDate(struct tm *psTime);

#endif /* __RTC_H__ */
