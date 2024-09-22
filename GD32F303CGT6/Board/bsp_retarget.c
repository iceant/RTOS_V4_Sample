#include <bsp_retarget.h>
#include <gd32f30x.h>

#define USARTX USART0

struct __FILE{int handle;};
typedef struct __FILE FILE;

int fputc(int ch, FILE* fp){
    
    usart_data_transmit(USARTX, (uint8_t)ch);
    while(RESET == usart_flag_get(USARTX, USART_FLAG_TBE));
    
    return ch;
}

