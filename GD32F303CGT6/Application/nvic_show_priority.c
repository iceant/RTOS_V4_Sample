#include <nvic_show_priority.h>
#include <stdio.h>
#include <gd32f30x.h>

#define SHOW_SYS_IRQ(SysIRQn)   \
printf("%s: %d(0x%x)\n" \
       , #SysIRQn   \
       , SCB->SHP[(((uint32_t)SysIRQn) & 0xFUL)-4UL]   \
       , SCB->SHP[(((uint32_t)SysIRQn) & 0xFUL)-4UL]   \
       )

void nvic_show_priority(void )
{
    
    SHOW_SYS_IRQ(MemoryManagement_IRQn);
    SHOW_SYS_IRQ(BusFault_IRQn);
    SHOW_SYS_IRQ(UsageFault_IRQn);
    SHOW_SYS_IRQ(SVCall_IRQn);
    SHOW_SYS_IRQ(DebugMonitor_IRQn);
    SHOW_SYS_IRQ(PendSV_IRQn);
    SHOW_SYS_IRQ(SysTick_IRQn);

    for(int i=WWDGT_IRQn; i<=DMA1_Channel3_Channel4_IRQn; i++){
        if(NVIC->IP[i]!=0){
            printf("IRQ_%d: Priority = %d(0x%x)\n", i, NVIC->IP[i], NVIC->IP[i]);
        }
    }
}


