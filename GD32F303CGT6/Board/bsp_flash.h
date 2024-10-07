#ifndef INCLUDED_BSP_FLASH_H
#define INCLUDED_BSP_FLASH_H

#ifndef INCLUDED_GD32F30X_H
#define INCLUDED_GD32F30X_H
#include <gd32f30x.h>
#endif /*INCLUDED_GD32F30X_H*/

#ifndef INCLUDED_STDINT_H
#define INCLUDED_STDINT_H
#include <stdint.h>
#endif /*INCLUDED_STDINT_H*/

#ifndef INCLUDED_STDDEF_H
#define INCLUDED_STDDEF_H
#include <stddef.h>
#endif /*INCLUDED_STDDEF_H*/



/* -------------------------------------------------------------------------------------------------------------- */
/*  */

int bsp_flash_write_uint32(uint32_t address, uint32_t* data, size_t data_size);
int bsp_flash_read_uint32(uint32_t address, uint32_t* data, size_t data_size);
int bsp_flash_write_uint8(uint32_t address, uint8_t* data, size_t data_size);
int bsp_flash_read_uint8(uint32_t address, uint8_t* data, size_t data_size);
int bsp_flash_write_uint16(uint32_t address, uint16_t* data, size_t data_size);

#endif /* INCLUDED_BSP_FLASH_H */
