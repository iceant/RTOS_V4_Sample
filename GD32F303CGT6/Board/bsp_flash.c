#include <bsp_flash.h>
#include <gd32f30x.h>
#include <string.h>
#include "printf.h"

/* -------------------------------------------------------------------------------------------------------------- */
/*  */
#define PAGE(T, SZ) (((T) + (SZ) - 1)/(SZ))
#define ALIGN_DOWN(x, a) ((x)&~(a - 1))
#define HW32_VAL(A) (*((volatile uint32_t*)(A)))

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

static int bsp_flash__get_page_size(uint32_t address){
    if(address<0x08080000){
        return 2048;
    }else if(address<=0x082FFFFF){
        return 4096;
    }else{
        return -1;
    }
}

static void bsp_flash__erase_uint32(uint32_t address, size_t size){
    uint32_t start_address = ALIGN_DOWN(address, bsp_flash__get_page_size(address));
    uint32_t end_address = address + size*4;
    uint32_t addr_p;
    fmc_state_enum fmc_state = FMC_READY;
    
//    fmc_unlock();
    
    fmc_flag_clear(FMC_FLAG_BANK0_END);
    fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
    fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
    
    for(addr_p = start_address; addr_p<end_address;){
        if(0xFFFFFFFFU!=HW32_VAL(addr_p)){
            int page_size = bsp_flash__get_page_size(addr_p);
            printf("[BSP_FLASH] Erase 0x%08x size: %d\n", addr_p, page_size);
            fmc_state = fmc_page_erase(addr_p);

            fmc_flag_clear(FMC_FLAG_BANK0_END);
            fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
            fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
        }
        addr_p += bsp_flash__get_page_size(addr_p);
    }
//    fmc_lock();
    
    /* check */
    if(0xFFFFFFFFU==HW32_VAL(start_address)){
        printf(" SUCCESS!\n");
    }else{
        printf(" FAILED!\n");
    }
    
}
/* -------------------------------------------------------------------------------------------------------------- */
/*  */

int bsp_flash_write_uint32(uint32_t address, uint32_t* data, size_t data_size){
    uint32_t end_address = address + data_size*4;
    uint32_t x_address = address;
    
    printf("[BSP_FLASH] Program 0x%08x to 0x%08x, size:%d\n", address, end_address, data_size*4);
    
    /* prepare flash for program */
    fmc_unlock();
    
    bsp_flash__erase_uint32(address, data_size);
    
    while(x_address < end_address){
        
        fmc_word_program(x_address, *data++);
        x_address+=4;
        
        fmc_flag_clear(FMC_FLAG_BANK0_END);
        fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
        fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
    }
    
    fmc_lock();
    
    return 0;
}

int bsp_flash_read_uint32(uint32_t address, uint32_t* data, size_t data_size){
    memcpy(data, (uint8_t*)address, data_size*4);
//    size_t i;
//    for(i=0; i<data_size; i++){
//        *data++ = HW32_VAL(address);
//        address+=4;
//    }
//    return i;
    return data_size;
}

int bsp_flash_write_uint8(uint32_t address, uint8_t* data, size_t data_size){
    return bsp_flash_write_uint32(address, (uint32_t*)data, PAGE(data_size, 4));
}

int bsp_flash_read_uint8(uint32_t address, uint8_t* data, size_t data_size){
    return bsp_flash_read_uint32(address, (uint32_t*)data, PAGE(data_size, 4));
}

int bsp_flash_write_uint16(uint32_t address, uint16_t* data, size_t data_size){
    uint32_t end_address = address + data_size*2;
    uint32_t x_address = address;
    
    printf("[BSP_FLASH] Program 0x%08x to 0x%08x, size:%d\n", address, end_address, data_size);
    
    fmc_unlock();
    
    /* prepare flash for program */
    bsp_flash__erase_uint32(address, data_size*2);
    
    while(x_address < end_address){
        uint16_t d = *data++;
        fmc_halfword_program(x_address, d);
        printf("program %x to %x \n", d, x_address);
        x_address+=2u;
        
        fmc_flag_clear(FMC_FLAG_BANK0_END|FMC_FLAG_BANK0_WPERR|FMC_FLAG_BANK0_PGERR);
    }
    
    fmc_lock();
    
    return 0;
}
