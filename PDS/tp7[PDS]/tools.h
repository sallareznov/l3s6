/* ------------------------------
   $Id: tools.h,v 1.2 2007/04/04 15:18:50 marquet Exp $
   ------------------------------------------------------------

   mtcs - a multithreaded chat serveur
   Philippe Marquet, Apr 2005

   Outils divers
*/

#ifndef _TOOLS_H_
#define _TOOLS_H_

/* Booleans */
#define FALSE 0
#define TRUE  (!FALSE)

/* Not yet */
extern void nyi_(const char*funcname, const char *s);
#define nyi(s) nyi_(__func__,s)

/* Progression trace */
extern int verbose;
extern void pgrs_(const char *funcname, const char *s);

#define pgrs(s) pgrs_(__func__,s)
#define pgrs_in() pgrs_(__func__,"--> entering")
#define pgrs_out() pgrs_(__func__,"-- exiting")

#endif
