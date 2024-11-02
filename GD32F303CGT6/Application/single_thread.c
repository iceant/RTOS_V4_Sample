#include <single_thread.h>
#include <os_kernel.h>
#include <printf.h>

static uint8_t stack[1024];
static os_thread_t single_thread;

static void thread_entry(void* p){
    uint32_t nCount = 0;
    while(1){
        printf("nCount: %lu, Tick:%lu\n\n", nCount++, os_tick_get());
        os_thread_mdelay(1000);
    }
}

void single_thread_run(void){
    os_thread_init(&single_thread, "single_thd"
                   , thread_entry, 0
                   , stack, OS_ARRAY_SIZE(stack)
                   , 20, 10, 0, 0);
    os_thread_startup(&single_thread);
}
