#ifndef INCLUDED_BSP_CPUID_H
#define INCLUDED_BSP_CPUID_H

#ifndef INCLUDED_GD32F30X_H
#define INCLUDED_GD32F30X_H
#include <gd32f30x.h>
#endif /*INCLUDED_GD32F30X_H*/

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

typedef struct BSP_CPUID_S{
    union {
        uint8_t U8[12];
        struct {
            uint32_t U32_1;
            uint32_t U32_2;
            uint32_t U32_3;
        }U32;
    }UID;
}BSP_CPUID_T;

void BSP_CPUID_GetDeviceID(BSP_CPUID_T* ID);


#endif /* INCLUDED_BSP_CPUID_H */
