#include <switch_to_thread.h>
#include <os_kernel.h>
#include "printf.h"

/* -------------------------------------------------------------------------------------------------------------- */
/*  */
#define SWITCH_TO_THREAD_STACK_SIZE 1024
static uint8_t switch_to_thread_stack[SWITCH_TO_THREAD_STACK_SIZE];

static os_thread_t switch_to_thread;
static os_sem_t switch_sem;


static void switch_to_thread_entry(void* p){
    os_err_t err;
    while(1){
        err = os_sem_take(&switch_sem, os_tick_from_millisecond(5000));
        if(err==OS_ERR_OK){
            printf("I'm Switched To Thread Running...\n");
        }else{
            printf("[TIMEOUT] SwitchToThread\n");
        }
    }
}
/* -------------------------------------------------------------------------------------------------------------- */
/*  */

void switch_to_thread_notify(void){
    printf("[Notify] Switch ...\n");
    os_sem_release(&switch_sem);
}

void switch_to_thread_run(void){
    os_sem_init(&switch_sem, "switch_sem", 0, OS_SEM_FLAG_FIFO);
    os_thread_init(&switch_to_thread, "switchToThd"
                   , switch_to_thread_entry, 0
                   , switch_to_thread_stack, OS_ARRAY_SIZE(switch_to_thread_stack)
                   , 20, 10, 0, 0);
    os_thread_startup(&switch_to_thread);
}
