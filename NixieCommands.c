/* @owner ICosta
 *
 * Copyright of this program is the property of Ivo Costa, without
 * whose written permission reproduction in whole or in part is prohibited.
 */
/**
 * @file NixieCommands.c
 *
 * @brief Command line functionality implementation
 *
 * @author I. Costa
 * @date 2015-26-11
 */

/* -- Includes */

/* Associated header */
#include "NixieCommands.h"

/* Standard headers */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>   

/* Functional headers */

#include "commandline.h"

#include "Delay.h"

#include "inc/hw_types.h"

#include "main.h"

#include "Nixie.h"

#include "Random.h"

#include "Rtc.h"

#include "utils/uartstdio.h"
#include "utils/ustdlib.h"

/* -- Constants */

/* -- Types */
extern struct tm psTime;


/**
 * @brief Table of valid command strings, callback functions and help messages.  This
 *				is used by the cmdline module.	
 *
 */
tCmdLineEntry g_psCmdTable[] =
{
    {"help",     	CMD_Help,      	" : Display list of commands" },
		{"SetTime",   CMD_SetTime,   	" : Set Time (Format: HH MM SS)"},
		{"SetDate",		CMD_SetDate,		" : Set Date (Format: DD MM YY)"},
    { 0, 0, 0 }
};


/**
 * @brief Print the help strings for all commands.
 *
 * @return 0
 */
int CMD_Help(int argc, char **argv){
	
	int32_t Index;

  /** Start at the beginning of the command table */
  Index = 0;

  /** Get to the start of a clean line on the serial output. */
  UARTprintf("\nAvailable Commands\n------------------\n");

  /** Display strings until we run out of them. */
  while(g_psCmdTable[Index].pcCmd){
	
		UARTprintf("%17s %s\n", g_psCmdTable[Index].pcCmd,g_psCmdTable[Index].pcHelp);
    Index++;
		
	}

  /** Leave a blank line after the help strings. */
  UARTprintf("\n");

	return (0);
}

/**
 * @brief Set Time.
 *
 * @return 0
 */
int CMD_SetTime(int argc, char **argv){
	
	int8_t hour=0;
	int8_t minutes=0;
	int8_t seconds=0;
	
    /** This command requires one parameter. */
    if(argc == 4)
    {
			/** Set time from the command line parameter. */
      hour 		= ustrtoul(argv[1], 0, 10);
			minutes = ustrtoul(argv[2], 0, 10);
			seconds = ustrtoul(argv[3], 0, 10);
			
      if(hour > 23 || hour < 0 ){
				UARTprintf("Invalid Hour \n");
				Delay_1ms(2);				
				return (0);
			}							
      if(minutes > 59 || minutes < 0 ){
				UARTprintf("Invalid Minutes \n");
				Delay_1ms(2);				
				return (0);
			}
      if(seconds > 59 || seconds < 0 ){
				UARTprintf("Invalid Seconds \n");	
				Delay_1ms(2);
				return (0);
			}				

			psTime.tm_hour	= hour;
			psTime.tm_min 	= minutes;
			psTime.tm_sec 	= seconds;
			// ** Set RTC */
			SetRtcTime(psTime);
			return (0);
    }
	
	UARTprintf("Invalid argument \n");	
	return (0);
}

/**
 * @brief Set Date.
 *
 * @return 0
 */
int CMD_SetDate(int argc, char **argv){
	
	int8_t day=0;
	int8_t month=0;
	int8_t year=0;
	
    /** This command requires one parameter. */
    if(argc == 4)
    {
			/** Set Date from the command line parameter. */
      day 		= ustrtoul(argv[1], 0, 10);
			month 	= ustrtoul(argv[2], 0, 10);
			year		= ustrtoul(argv[3], 0, 10);
			
      if(day > 31 || day < 0 ){
				UARTprintf("Invalid Day \n");	
				return (0);
			}							
      if(month > 12 || month < 0 ){
				UARTprintf("Invalid Month \n");	
				return (0);
			}
      if(year > 99 || year < 0 ){
				UARTprintf("Invalid Year \n");	
				return (0);
			}				

			psTime.tm_mday 	= day;
			psTime.tm_mon 	= month;
			psTime.tm_year 	= year;
			// ** Set RTC */
			SetRtcDate(psTime);
			return (0);
    }
	
	UARTprintf("Invalid argument \n");	
	return (0);
}
