#ifndef INCLUDED_DWT_DELAY_H
#define INCLUDED_DWT_DELAY_H

#ifndef INCLUDED_GD32F30X_H
#define INCLUDED_GD32F30X_H
#include <gd32f30x.h>
#endif /*INCLUDED_GD32F30X_H*/

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

int DWT_Delay_Init(void);

__STATIC_INLINE void DWT_Delay_us(volatile uint32_t au32_microseconds)
{
    uint32_t au32_initial_ticks = DWT->CYCCNT;
    uint32_t au32_ticks = (SystemCoreClock / 1000000);
    au32_microseconds *= au32_ticks;
    while ((DWT->CYCCNT - au32_initial_ticks) < au32_microseconds-au32_ticks);
}

__STATIC_INLINE void DWT_Delay_ms(volatile uint32_t au32_milliseconds)
{
    uint32_t au32_initial_ticks = DWT->CYCCNT;
    uint32_t au32_ticks = (SystemCoreClock / 1000);
    au32_milliseconds *= au32_ticks;
    while ((DWT->CYCCNT - au32_initial_ticks) < au32_milliseconds);
}

#endif /* INCLUDED_DWT_DELAY_H */
