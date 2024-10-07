#include <board.h>
#include <os_config.h>
#include <os_kernel.h>
#include <cpu.h>

void Board_Init(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE0_SUB4);
    
    BSP_USART0_Init();
    
    #if defined(OS_ENABLE)
    SysTick_Config(SystemCoreClock / OS_TICK_PER_SECOND);
    
    NVIC_SetPriority(SVCall_IRQn, 0x00);
    NVIC_SetPriority(SysTick_IRQn, 0xF1);
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    #endif
    
}

void Board_DeInit(void){
    BSP_USART0_DeInit();
}

void Board_Reboot(void)
{
    os_service_reboot();
}
