#include <test_flash.h>
#include <board.h>
#include <string.h>
#include "printf.h"
#include <sdk_hex.h>

#define TEST_DATA_SIZE  (6*1024)
#define TEST_START_ADDR (0x0807F800)

static uint8_t WriteData[TEST_DATA_SIZE];
static uint8_t ReadData[TEST_DATA_SIZE];

void Test_Flash_Run(void){
    
    for(int i=0; i<TEST_DATA_SIZE; i++){
        WriteData[i] = i & 0xFF;
    }
    
    bsp_flash_write_uint8(TEST_START_ADDR, WriteData, TEST_DATA_SIZE);
    bsp_flash_read_uint8(TEST_START_ADDR, ReadData, TEST_DATA_SIZE);
    
    if(memcmp(ReadData, WriteData, TEST_DATA_SIZE)!=0){
        printf("Failed!\n");
//        sdk_hex_dump("WRITE", WriteData, TEST_DATA_SIZE, printf);
//        sdk_hex_dump("READ", ReadData, TEST_DATA_SIZE, printf);
        
    }else{
        printf("Success!\n");
    }
    
}

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#define FMC_PAGE_SIZE           ((uint16_t)(2*1024))
#define FMC_WRITE_START_ADDR    ((uint32_t)0x0807F800U)
#define FMC_WRITE_END_ADDR      ((uint32_t)0x08080000U)
uint32_t *ptrd;
uint32_t address = 0x00000000U;
uint32_t data0   = 0x01234567U;

/* calculate the number of page to be programmed/erased */
uint32_t PageNum = (FMC_WRITE_END_ADDR - FMC_WRITE_START_ADDR) / FMC_PAGE_SIZE;
/* calculate the number of page to be programmed/erased */
uint32_t WordNum = ((FMC_WRITE_END_ADDR - FMC_WRITE_START_ADDR) >> 2);

void fmc_erase_pages(void)
{
    uint32_t EraseCounter;
    
    /* unlock the flash program/erase controller */
    fmc_unlock();
    
    /* clear all pending flags */
    fmc_flag_clear(FMC_FLAG_BANK0_END);
    fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
    fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
    
    /* erase the flash pages */
    for(EraseCounter = 0; EraseCounter < PageNum; EraseCounter++){
        fmc_page_erase(FMC_WRITE_START_ADDR + (FMC_PAGE_SIZE * EraseCounter));
        fmc_flag_clear(FMC_FLAG_BANK0_END);
        fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
        fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
    }
    
    /* lock the main FMC after the erase operation */
    fmc_lock();
}

void fmc_program(void)
{
    /* unlock the flash program/erase controller */
    fmc_unlock();
    
    address = FMC_WRITE_START_ADDR;
    
    /* program flash */
    while(address < FMC_WRITE_END_ADDR){
        fmc_word_program(address, data0);
        address += 4;
        fmc_flag_clear(FMC_FLAG_BANK0_END);
        fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
        fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
    }
    
    /* lock the main FMC after the program operation */
    fmc_lock();
}


void fmc_erase_pages_check(void)
{
    uint32_t i;
    
    ptrd = (uint32_t *)FMC_WRITE_START_ADDR;
    
    /* check flash whether has been erased */
    for(i = 0; i < WordNum; i++){
        if(0xFFFFFFFF != (*ptrd)){
            printf("Erase ERROR!!!\n");
            break;
        }else{
            ptrd++;
        }
    }
    if(i==WordNum){
        printf("Erase Success!!!\n");
    }
}

void fmc_program_check(void)
{
    uint32_t i;
    
    ptrd = (uint32_t *)FMC_WRITE_START_ADDR;
    
    /* check flash whether has been programmed */
    for(i = 0; i < WordNum; i++){
        if((*ptrd) != data0){
            printf("PROGRAM ERROR!!!!\n");
            break;
        }else{
            ptrd++;
        }
    }
    if(i==WordNum){
        printf("Program Success!!!\n");
    }
}

void Test_Flash2_Run(void){
    /* step1: erase pages and check if it is successful. If not, light the LED2. */
    fmc_erase_pages();
    fmc_erase_pages_check();
    
    /* step2: program and check if it is successful. If not, light the LED3. */
    fmc_program();
    fmc_program_check();
}