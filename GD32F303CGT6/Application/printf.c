#include <printf.h>
#include <board.h>
#include <stdio.h>

#define PRINTF_BUFFER_SIZE 1024

static char printf__buffer[PRINTF_BUFFER_SIZE];
static volatile int printf__lock = 0;

int my_printf(const char* fmt, ...)
{
    int size;
    va_list ap;

   
    if(printf__lock==1){
        return 0;
    }
    printf__lock = 1;
    
    va_start(ap, fmt);
    size = vsnprintf(printf__buffer, PRINTF_BUFFER_SIZE, fmt, ap);
    va_end(ap);

    if(size>PRINTF_BUFFER_SIZE){
        size = PRINTF_BUFFER_SIZE-1;
    }
    printf__buffer[size]='\0';
    
    BSP_USART0_DMATx((uint8_t*)printf__buffer, size);
    
    printf__lock = 0;
    
    return size;
}
