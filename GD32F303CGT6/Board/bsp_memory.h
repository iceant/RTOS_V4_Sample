#ifndef INCLUDED_BSP_MEMORY_H
#define INCLUDED_BSP_MEMORY_H

#ifndef INCLUDED_GD32F30X_H
#define INCLUDED_GD32F30X_H
#include <gd32f30x.h>
#endif /*INCLUDED_GD32F30X_H*/

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

uint16_t BSP_Memory_GetSRAMDensity(void);
uint16_t BSP_Memory_GetFlashDensity(void);



#endif /* INCLUDED_BSP_MEMORY_H */
