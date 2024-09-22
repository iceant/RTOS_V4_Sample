#ifndef INCLUDED_PRINTF_H
#define INCLUDED_PRINTF_H

#ifndef INCLUDED_STDARG_H
#define INCLUDED_STDARG_H
#include <stdarg.h>
#endif /*INCLUDED_STDARG_H*/

/* -------------------------------------------------------------------------------------------------------------- */
/* METHODS */


int my_printf(const char* fmt, ...);

#define printf my_printf

#endif /* INCLUDED_PRINTF_H */
