#include <main.h>
#include <stdint.h>
#include <stddef.h>
#include <board.h>
#include <stdio.h>
#include <os_kernel.h>
#include <nvic_show_priority.h>
#include <single_thread.h>
//#include "printf.h"
#include "two_yield_thread.h"
#include "sdk_hex.h"
#include "sdk_ringbuffer.h"

static uint8_t use_usart0_thread_stack[1024];
static os_thread_t use_usart0_thread;
static os_sem_t use_usart0_sem;
static uint8_t use_usart0_rxblock[1024];
static sdk_ringbuffer_t use_usart0_buffer;

static int use_usart0_rx_handler(uint8_t* data, size_t size, void* ud){
    sdk_ringbuffer_write(&use_usart0_buffer, data, size);
    os_sem_release(&use_usart0_sem);
    return 0;
}

static void use_usart0_thread_entry(void* p){
    sdk_ringbuffer_init(&use_usart0_buffer, use_usart0_rxblock, OS_ARRAY_SIZE(use_usart0_rxblock));
    BSP_USART0_SetRxHandler(use_usart0_rx_handler, 0);
     
    while(1){
        os_sem_take(&use_usart0_sem, OS_WAIT_FOREVER);
        os_size_t used = sdk_ringbuffer_used(&use_usart0_buffer);
        if(used>0){
            SDK_HEX_DUMP("USE_USART0", use_usart0_buffer.buffer, used);
        }
        
        if(sdk_ringbuffer_find_str(&use_usart0_buffer, 0, "reboot")>=SDK_ERR_ERROR){
            Board_Reboot();
        }
        
        if(used>0){
            sdk_ringbuffer_reset(&use_usart0_buffer);
        }
        
    }
}

/* EXCEPTION TEST CASE */

int illegal_instruction_execution(void) {
  int (*bad_instruction)(void) = (void *)0xE0000000;
  return bad_instruction();
}

uint32_t read_from_bad_address(void) {
  return *(volatile uint32_t *)0xbadcafe;
}

__asm
void access_disabled_coprocessor(void) {
  // FreeRTOS will automatically enable the FPU co-processor.
  // Let's disable it for the purposes of this example
    
  ldr r0, =0xE000ED88
  mov r1, #0
  str r1, [r0]
  dsb
  vmov r0, s0

}

void bad_addr_double_word_write(void) {
  volatile uint64_t *buf = (volatile uint64_t *)0x30000000;
  *buf = 0x1122334455667788;
}

void unaligned_double_word_read(void) {
  static void *g_unaligned_buffer;
  uint64_t *buf = g_unaligned_buffer;
  *buf = 0x1122334455667788;
}

/*  ---------- */

void recover_from_task_fault(void){
    while(1){
        os_thread_yield();
    }
}

static uint8_t illegal_thread_stack[1024];
static os_thread_t illegal_thread;
static void illegal_thread_entry(void*p){
    illegal_instruction_execution();
    while(1){
        printf("illegal_instruction_execution loop...\n");
    }
}

int main(void)
{
    Board_Init();
   
    nvic_show_priority();
         
    
    #if defined(OS_ENABLE)
    
    os_kernel_init();
    
    single_thread_run();
    
    two_yield_thread_startup();
       
    os_sem_init(&use_usart0_sem, "use_usart0_sem", 0, OS_WAIT_FLAG_FIFO);
    
    os_thread_init(&use_usart0_thread, "use_usart0_thd", use_usart0_thread_entry, 0
        , use_usart0_thread_stack, OS_ARRAY_SIZE(use_usart0_thread_stack)
        , 20, 10, 0, 0);
    os_thread_startup(&use_usart0_thread);
    
        
    os_thread_init(&illegal_thread, "illegal_thd", illegal_thread_entry, 0
        , illegal_thread_stack, OS_ARRAY_SIZE(illegal_thread_stack)
        , 20, 10, 0, 0);
    os_thread_startup(&illegal_thread);    
        
    os_kernel_startup();
    
    #endif

    size_t nCount = 0;
    
    while(1){
        printf("Hello\n");
        for(int i=0; i<0x3ffffff; i++);
    }
}