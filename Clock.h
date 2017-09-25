/* @owner ICosta
 *
 * Copyright of this program is the property of Ivo Costa, without
 * whose written permission reproduction in whole or in part is prohibited.
 */
/**
 * @file Clock.h
 *
 * @brief Prototypes for Time Handling
 *
 * @author I. Costa
 * @date 2016-20-02
 */
 
 
#ifndef __Clock_H__
#define __Clock_H__

 /* -- Includes */

/* Associated header */

/* Standard headers */
#include <time.h>

/* Functional headers */
#include "simplelink.h"

/* -- Constants */
/* -- Variables */
/* -- Macros */
/* -- Types */

/**
 * @brief Initializes the Clock_Init.
 *
 * @return None
 */
void	Clock_Init(void);

/**
 * @brief Start Clock.
 *
 * @return None
 */
void	Clock_Start(void);

/**
 * @brief Stop Clock.
 *
 * @return None
 */
void	Clock_Stop(void);

/**
 * @brief Clock Show Hour.
 *
 * @return None
 */
void	Clock_Show_Hour(void);

/**
 * @brief Clock Show Date.
 *
 * @return None
 */
void	Clock_Show_Date(void);	

/**
 * @brief Hour Blink Dots Left
 *
 * @return None
 */
void	Clock_Hour_Blink_Dots_Left(void);

/**
 * @brief Hour Blink Dots Right
 *
 * @return None
 */
void	Clock_Hour_Blink_Dots_Right(void);

/**
 * @brief Date Blink Dots Left
 *
 * @return None
 */
void	Clock_Date_Blink_Dots_Left(void);

/**
 * @brief Date Blink Dots Right
 *
 * @return None
 */
void	Clock_Date_Blink_Dots_Right(void);

/**
 * @brief Date No Blink Dots
 *
 * @return None
 */
void	Clock_Date_No_Blink_Dots(void);

/**
 * @brief Clock Set Time
 *
 * @return None
 */
void	Clock_SetTime(void);

/**
 * @brief Clock Set Date
 *
 * @return None
 */
void	Clock_SetDate(void);
/**
 * @brief Clock Inc Hour
 *
 * @return None
 */
void	Clock_Inc_Hour(void);

/**
 * @brief Clock Dec Hour
 *
 * @return None
 */
void	Clock_Dec_Hour(void);

/**
 * @brief Clock Inc Minutes
 *
 * @return None
 */
void	Clock_Inc_Min(void);

/**
 * @brief Clock Dec Minutes
 *
 * @return None
 */
void	Clock_Dec_Min(void);

/**
 * @brief Clock Inc Day
 *
 * @return None
 */
void	Clock_Inc_Day(void);

/**
 * @brief Clock Dec Day
 *
 * @return None
 */
void	Clock_Dec_Day(void);

/**
 * @brief Clock Inc Month
 *
 * @return None
 */
void	Clock_Inc_Month(void);

/**
 * @brief Clock Dec Day
 *
 * @return None
 */
void	Clock_Dec_Month(void);

/**
 * @brief Clock Inc Year
 *
 * @return None
 */
void	Clock_Inc_Year(void);

/**
 * @brief Clock Dec Year
 *
 * @return None
 */
void	Clock_Dec_Year(void);


#endif /* __Clock_H__ */
