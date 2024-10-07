#include <iap_boot.h>
#include <os_kernel.h>
#include <gd32f30x.h>

static os_thread_t IAP__Thread;
static uint8_t IAP__ThreadStack[1024];
static os_sem_t IAP__Sem;

static void IAP__ThreadEntry(void*p){
    while(1){
        os_sem_take(&IAP__Sem, OS_WAIT_FOREVER);
        
    }
}

void IAP_Startup(void){
    os_sem_init(&IAP__Sem, "IAP_Sem", 0, OS_WAIT_FLAG_FIFO);
    os_thread_init(&IAP__Thread, "IAP_Thd"
        , IAP__ThreadEntry, 0
        , IAP__ThreadStack, OS_ARRAY_SIZE(IAP__ThreadStack)
        , 20, 10, 0, 0);
    os_thread_startup(&IAP__Thread);
}

static void IAP_ClearPendingIRQ(IRQn_Type IRQn)
{
    NVIC->ICPR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F)); /* Clear pending interrupt */
}

static void IAP_ClearPendingIRQs(void){
//    for(IRQn_Type IRQn = WWDGT_IRQn; IRQn<=DMA1_Channel3_Channel4_IRQn; IRQn++){
//        IAP_ClearPendingIRQ(IRQn);
//    }
    for(int i=0; i< OS_ARRAY_SIZE(NVIC->ICPR); i++){
        NVIC->ICPR[i] = 0;
    }
}

void IAP_JumpToApplication(unsigned int address)
{
    if(((*(volatile uint32_t*)address) & 0x2FFE0000)==0x20000000){
        cpu_set_primask(1);
        IAP_Function_T JumpAddress = (IAP_Function_T)*(volatile uint32_t*)(address + 4);
        cpu_disable_irq();
        cpu_set_msp(*(volatile uint32_t*)address);
        IAP_ClearPendingIRQs();
        cpu_set_primask(0);
        JumpAddress();
        while(1);
    }
}
