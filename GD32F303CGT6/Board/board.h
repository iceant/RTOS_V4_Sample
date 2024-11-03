#ifndef INCLUDED_BOARD_H
#define INCLUDED_BOARD_H

#ifndef INCLUDED_GD32F30X_H
#define INCLUDED_GD32F30X_H
#include <gd32f30x.h>
#endif /*INCLUDED_GD32F30X_H*/

#ifndef INCLUDED_OS_CONFIG_H
#include <os_config.h>
#endif /*INCLUDED_OS_CONFIG_H*/

#ifndef INCLUDED_BSP_USART1_H
#include <bsp_usart0.h>
#endif /*INCLUDED_BSP_USART1_H*/

#ifndef INCLUDED_BSP_FLASH_H
#include <bsp_flash.h>
#endif /*INCLUDED_BSP_FLASH_H*/

#ifndef INCLUDED_DWT_DELAY_H
#include <dwt_delay.h>
#endif /*INCLUDED_DWT_DELAY_H*/

#ifndef INCLUDED_BSP_MEMORY_H
#include <bsp_memory.h>
#endif /*INCLUDED_BSP_MEMORY_H*/



/* -------------------------------------------------------------------------------------------------------------- */
/*  */

void Board_Init(void);

void Board_DeInit(void);

void Board_Reboot(void);


#endif /* INCLUDED_BOARD_H */
