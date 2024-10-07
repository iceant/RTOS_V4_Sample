#include <main.h>
#include <iap.h>
#include <os_kernel.h>
#include <board.h>
#include "printf.h"

int main(void){
    
    nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x00000000);
    
    Board_Init();
    printf("/* -------------------- */\n");
    printf("/* Bootloader V1.0 */\n");
    printf("/* -------------------- */\n\n");
    IAP_JumpToApplication(0x08010000);
    
    while(1);
}