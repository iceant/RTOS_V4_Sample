#include <thread_exit.h>
#include <os_kernel.h>
#include <stdio.h>

static uint8_t exit_thread_stack[512];
static os_thread_t exit_thread;

static void exit_thread_die(os_thread_t* thread){
    printf("thread: %s die!\n", thread->name);
}

static void exit_thread_entry(void* p){
    printf("exit_thread running...\n");
}

void thread_exit_startup(void){
    os_thread_init(&exit_thread, "exit", exit_thread_entry, 0
                   ,exit_thread_stack, OS_ARRAY_SIZE(exit_thread_stack),
                   20, 10, exit_thread_die, 0);
    
    os_thread_startup(&exit_thread);
}