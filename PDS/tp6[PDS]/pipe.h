/* ------------------------------
   $Id: pipe.h,v 1.2 2005/03/29 09:46:52 marquet Exp $
   ------------------------------------------------------------

   mshell - a job manager
   
*/

#ifndef _PIPE_H_
#define _PIPE_H_

#include "common.h"

extern void do_pipe(char *cmds[MAXCMDS][MAXARGS], int nbcmd, int bg);

#endif
