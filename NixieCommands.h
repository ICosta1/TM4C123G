/* @owner ICosta
 *
 * Copyright of this program is the property of Ivo Costa, without
 * whose written permission reproduction in whole or in part is prohibited.
 */
/**
 * @file NixieCommands.c
 *
 * @brief Prototypes for Command line functionality implementation
 *
 * @author I. Costa
 * @date 2015-26-11
 */

#ifndef __NIXIECOMMANDS_H__
#define __NIXIECOMMANDS_H__

/* -- Includes */

/* Associated header */
/* Standard headers */           
/* Functional headers */

/* -- Constants */

/* Defines for the command line argument parser */
#define CMDLINE_MAX_ARGS 					3
#define APP_INPUT_BUF_SIZE        128

/* -- Types */

/** Declaration for the callback functions that will implement the command line
	  functionality.  These functions get called by the command line interpreter
    when the corresponding command is typed into the command line. */
extern int CMD_Help (int argc, char **argv);
extern int CMD_SetTime (int argc, char **argv);
extern int CMD_SetDate (int argc, char **argv);

#endif /** __NIXIECOMMANDS_H__ */
