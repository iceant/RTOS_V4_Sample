#include <bsp_usart0.h>
#include <os_kernel.h>
/* -------------------------------------------------------------------------------------------------------------- */
/* DEFINITIONS */
#define USARTX                  USART0

#define USART_CLOCK_CMD         rcu_periph_clock_enable
#define USART_CLOCK             RCU_USART0

#define TX_GPIO_CLOCK_CMD       rcu_periph_clock_enable
#define TX_GPIO_CLOCK           RCU_GPIOA
#define TX_GPIO                 GPIOA
#define TX_GPIO_PIN             GPIO_PIN_9

#define RX_GPIO_CLOCK_CMD       rcu_periph_clock_enable
#define RX_GPIO_CLOCK           RCU_GPIOA
#define RX_GPIO                 GPIOA
#define RX_GPIO_PIN             GPIO_PIN_10

#define DMAX                    DMA0
#define DMA_CLOCK_CMD           rcu_periph_clock_enable
#define DMA_CLOCK               RCU_DMA0

#define DMA_TX_CHN              DMA_CH3
#define DMA_RX_CHN              DMA_CH4

#define DMA_RX_CHN_IRQn         DMA0_Channel4_IRQn

#define USARTX_DATA_ADDRESS    ((uint32_t)&USART_DATA(USARTX))

#define USARTX_IRQn             USART0_IRQn
#define USARTX_IRQHandler       USART0_IRQHandler

/* -------------------------------------------------------------------------------------------------------------- */
/* STATIC */
static BSP_USART0_RxHandler_t BSP_USART0__RxHandler;
static void* BSP_USART0__RxHandler_Userdata;
static uint8_t BSP_USART0__RxBuffer[BSP_USART0_RX_BUFFER_SIZE];
static size_t  BSP_USART0__RxSize;
/* -------------------------------------------------------------------------------------------------------------- */
/* METHODS */

void BSP_USART0_Init(void){
    BSP_USART0__RxHandler = 0;
    BSP_USART0__RxHandler_Userdata = 0;
    BSP_USART0__RxSize = 0;
    
    TX_GPIO_CLOCK_CMD(TX_GPIO_CLOCK);
    RX_GPIO_CLOCK_CMD(RX_GPIO_CLOCK);
    USART_CLOCK_CMD(USART_CLOCK);
    /* enable DMA0 */
    DMA_CLOCK_CMD(DMA_CLOCK);
    
/* connect port to USARTx_Tx */
    gpio_init(TX_GPIO, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, TX_GPIO_PIN);
    
    /* connect port to USARTx_Rx */
    gpio_init(RX_GPIO, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, RX_GPIO_PIN);
    
    nvic_irq_enable(USARTX_IRQn, 0, 3);
    
    /* USART configure */
    usart_deinit(USARTX);
    usart_baudrate_set(USARTX, 115200U);
    usart_receive_config(USARTX, USART_RECEIVE_ENABLE);
    usart_transmit_config(USARTX, USART_TRANSMIT_ENABLE);
    usart_enable(USARTX);
    
    BSP_USART0_DMARx(BSP_USART0__RxBuffer, BSP_USART0_RX_BUFFER_SIZE);
    
    usart_interrupt_enable(USARTX, USART_INT_IDLE);
}

void BSP_USART0_DMATx(uint8_t* buffer, size_t size){
    dma_parameter_struct dma_init_struct;

    /* deinitialize DMA channel3(USART0 tx) */
    dma_deinit(DMAX, DMA_TX_CHN);
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = (uint32_t)buffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = size;
    dma_init_struct.periph_addr = USARTX_DATA_ADDRESS;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMAX, DMA_TX_CHN, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMAX, DMA_TX_CHN);
    /* enable DMA channel3 */
    dma_channel_enable(DMAX, DMA_TX_CHN);
    
    /* enable USART DMA for transmission */
    usart_dma_transmit_config(USARTX, USART_TRANSMIT_DMA_ENABLE);
    
    /* wait DMA channel transfer complete */
    while(RESET == dma_flag_get(DMAX, DMA_TX_CHN, DMA_INTF_FTFIF)){}
}

void BSP_USART0_DMARx(uint8_t* buffer, size_t size){
    dma_parameter_struct dma_init_struct;
    
    dma_deinit(DMAX, DMA_RX_CHN);
    usart_flag_clear(USARTX, USART_FLAG_RBNE);
    usart_dma_receive_config(USARTX, USART_RECEIVE_DMA_ENABLE);
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)buffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = size;
    dma_init_struct.periph_addr = USARTX_DATA_ADDRESS;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMAX, DMA_RX_CHN, &dma_init_struct);
    /* configure DMA mode */
//    dma_circulation_disable(DMAX, DMA_RX_CHN);
    dma_circulation_enable(DMAX, DMA_RX_CHN);
    dma_memory_to_memory_disable(DMAX, DMA_RX_CHN);
    
//    nvic_irq_enable(DMA_RX_CHN_IRQn, 0, 3);
    
    /* enable DMA0 channel4 transfer complete interrupt */
    //dma_interrupt_enable(DMAX, DMA_RX_CHN, DMA_INT_FTF);
    
    /* enable DMA channel4 */
    dma_channel_enable(DMAX, DMA_RX_CHN);
    
}

void BSP_USART0_DeInit(void){
    BSP_USART0__RxHandler = 0;
    BSP_USART0__RxHandler_Userdata = 0;
}

void BSP_USART0_SetRxHandler(BSP_USART0_RxHandler_t handler, void* ud)
{
    BSP_USART0__RxHandler = handler;
    BSP_USART0__RxHandler_Userdata = ud;
}

void BSP_USART0_Send(uint8_t* bytes, size_t size){
    for(size_t i=0; i<size; i++){
        usart_data_transmit(USARTX, (uint8_t)*bytes++);
        while(RESET == usart_flag_get(USARTX, USART_FLAG_TBE));
    }
}

//void DMA0_Channel4_IRQHandler(void)
//{
//    if(dma_interrupt_flag_get(DMA0, DMA_CH4, DMA_INT_FLAG_FTF)) {
//        dma_interrupt_flag_clear(DMA0, DMA_CH4, DMA_INT_FLAG_G);
//    }
//}

void USARTX_IRQHandler(void){
    os_interrupt_enter();
    
    if(RESET != usart_interrupt_flag_get(USARTX, USART_INT_FLAG_IDLE)){
        /* clear IDLE flag */
        usart_data_receive(USARTX);
        
        /* number of data received */
        BSP_USART0__RxSize = BSP_USART0_RX_BUFFER_SIZE - (dma_transfer_number_get(DMAX, DMA_RX_CHN));
        /* disable DMA and reconfigure */
        dma_channel_disable(DMAX, DMA_RX_CHN);
        if(BSP_USART0__RxHandler){
            BSP_USART0__RxHandler(BSP_USART0__RxBuffer, BSP_USART0__RxSize, BSP_USART0__RxHandler_Userdata);
        }
        dma_transfer_number_config(DMAX, DMA_RX_CHN, BSP_USART0_RX_BUFFER_SIZE);
        dma_channel_enable(DMAX, DMA_RX_CHN);
    }
    
    os_interrupt_leave();
}


