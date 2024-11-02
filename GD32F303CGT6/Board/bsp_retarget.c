#include <bsp_retarget.h>
#include <gd32f30x.h>

#define USARTX USART0

#ifdef __GNUC__
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
    #define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
    struct __FILE{int handle;};
    typedef struct __FILE FILE;

    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
    #define GETCHAR_PROTOTYPE int fgetc(FILE* f)
#endif /* __GNUC__ */



PUTCHAR_PROTOTYPE{
    
    usart_data_transmit(USARTX, (uint8_t)ch);
    while(RESET == usart_flag_get(USARTX, USART_FLAG_TBE));
    
    return ch;
}

GETCHAR_PROTOTYPE{
    uint8_t ch;
    ch = usart_data_receive(USARTX);
    return ch;
}



