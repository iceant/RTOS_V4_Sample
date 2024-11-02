#ifndef INCLUDED_BSP_USART0_H
#define INCLUDED_BSP_USART0_H

#ifndef INCLUDED_STDINT_H
#define INCLUDED_STDINT_H
#include <stdint.h>
#endif /*INCLUDED_STDINT_H*/

#ifndef INCLUDED_STDDEF_H
#define INCLUDED_STDDEF_H
#include <stddef.h>
#endif /*INCLUDED_STDDEF_H*/

#ifndef INCLUDED_GD32F30X_H
#define INCLUDED_GD32F30X_H
#include <gd32f30x.h>
#endif /*INCLUDED_GD32F30X_H*/


/* -------------------------------------------------------------------------------------------------------------- */
/* DEFINITIONS */

#ifndef BSP_USART0_RX_BUFFER_SIZE
    #define BSP_USART0_RX_BUFFER_SIZE 1024
#endif


/* -------------------------------------------------------------------------------------------------------------- */
/* TYPES */

typedef int (*BSP_USART0_RxHandler_t)(uint8_t * buffer, size_t buffer_size, void*);

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

void BSP_USART0_Init(void);

void BSP_USART0_DeInit(void);

void BSP_USART0_SetRxHandler(BSP_USART0_RxHandler_t handler, void* ud);

void BSP_USART0_DMATx(uint8_t* buffer, size_t size);

void BSP_USART0_DMARx(uint8_t* buffer, size_t size);

void BSP_USART0_Send(uint8_t* bytes, size_t size);

void BSP_USART0_SetDMARxBufferSize(size_t size);


#endif /* INCLUDED_BSP_USART0_H */
