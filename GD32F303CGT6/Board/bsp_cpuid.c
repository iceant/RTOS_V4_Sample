#include <bsp_cpuid.h>

void BSP_CPUID_GetDeviceID(BSP_CPUID_T* ID)
{
    uint32_t U1 = REG32(0x1FFFF7E8);
    uint32_t U2 = REG32(0x1FFFF7EC);
    uint32_t U3 = REG32(0x1FFFF7F0);
    
    ID->UID.U32.U32_1 = U1;
    ID->UID.U32.U32_2 = U2;
    ID->UID.U32.U32_3 = U3;
}
