#include <two_yield_thread.h>
#include <os_kernel.h>
#include <stdio.h>

#define STACK_SIZE 1024
static uint8_t yield_thread1_stack[STACK_SIZE];
static uint8_t yield_thread2_stack[STACK_SIZE];
static os_thread_t yield_thread1;
static os_thread_t yield_thread2;

void yield_thread_entry(void* p){
    while(1){
        printf("Thread: %s Yield!\n", os_thread_self()->name);
        os_thread_yield();
    }
}

void two_yield_thread_startup(void){
    os_thread_init(&yield_thread1, "yield_1", yield_thread_entry, 0, yield_thread1_stack, STACK_SIZE, 20, 10, 0, 0);
    os_thread_init(&yield_thread2, "yield_2", yield_thread_entry, 0, yield_thread2_stack, STACK_SIZE, 20, 10, 0, 0);
    os_thread_startup(&yield_thread1);
    os_thread_startup(&yield_thread2);
}