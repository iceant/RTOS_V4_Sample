#include <bsp_memory.h>

#define MEMORY_DENSITY_BASE_ADDRESS 0x1FFFF7E0



uint16_t BSP_Memory_GetSRAMDensity(void){
    uint32_t value = REG32(MEMORY_DENSITY_BASE_ADDRESS);
    return (value >> 16);
}

uint16_t BSP_Memory_GetFlashDensity(void){
    uint32_t value = REG32(MEMORY_DENSITY_BASE_ADDRESS);
    return (value & 0xFFFF);
}

