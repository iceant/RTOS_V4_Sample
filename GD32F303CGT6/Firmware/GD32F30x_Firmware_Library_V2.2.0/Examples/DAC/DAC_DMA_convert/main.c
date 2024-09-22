/*!
    \file    main.c
    \brief   DAC DMA convert demo
    
    \version 2017-02-10, V1.0.0, firmware for GD32F30x
    \version 2018-10-10, V1.1.0, firmware for GD32F30x
    \version 2018-12-25, V2.0.0, firmware for GD32F30x
    \version 2020-09-30, V2.1.0, firmware for GD32F30x
*/

/*
    Copyright (c) 2023, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f30x.h"

#define CONVERT_NUM          (10)
#define DAC0_R8DH_ADDRESS    (0x40007410)
const uint8_t convertarr[CONVERT_NUM] = {0x00, 0x33, 0x66, 0x99, 0xCC, 0xFF, 0xCC, 0x99, 0x66, 0x33};

/* configure RCU peripheral */
void rcu_config(void);
/* configure GPIO peripheral */
void gpio_config(void);
/* configure DMA peripheral */
void dma_config(void);
/* configure DAC peripheral */
void dac_config(void);
/* configure TIMER peripheral */
void timer5_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* configure RCU peripheral */
    rcu_config();
    /* configure GPIO peripheral */
    gpio_config();
    /* configure DMA peripheral */
    dma_config();
    /* configure DAC peripheral */
    dac_config();
    /* configure TIMER peripheral */
    timer5_config();

    while(1) {
    }
}

/*!
    \brief      configure RCU peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* enable GPIOA clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA1);
    /* enable DAC clock */
    rcu_periph_clock_enable(RCU_DAC);
    /* enable TIMER clock */
    rcu_periph_clock_enable(RCU_TIMER5);
}

/*!
    \brief      configure GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* configure PA4 as DAC output */
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
}

/*!
    \brief      configure DMA peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config(void)
{
    dma_parameter_struct dma_struct;
    /* clear all the interrupt flags */
    dma_flag_clear(DMA1, DMA_CH2, DMA_INTF_GIF);
    dma_flag_clear(DMA1, DMA_CH2, DMA_INTF_FTFIF);
    dma_flag_clear(DMA1, DMA_CH2, DMA_INTF_HTFIF);
    dma_flag_clear(DMA1, DMA_CH2, DMA_INTF_ERRIF);
    
    /* configure the DMA1 channel 2 */
    dma_struct.periph_addr  = DAC0_R8DH_ADDRESS;
    dma_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_struct.memory_addr  = (uint32_t)convertarr;
    dma_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_struct.number       = CONVERT_NUM;
    dma_struct.priority     = DMA_PRIORITY_ULTRA_HIGH;
    dma_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_struct.direction    = DMA_MEMORY_TO_PERIPHERAL;
    dma_init(DMA1, DMA_CH2, &dma_struct);

    dma_circulation_enable(DMA1, DMA_CH2);
    dma_channel_enable(DMA1, DMA_CH2);
}

/*!
    \brief      configure DAC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_config(void)
{
    /* initialize DAC */
    dac_deinit(DAC0);
    /* DAC trigger config */
    dac_trigger_source_config(DAC0, DAC_OUT0, DAC_TRIGGER_T5_TRGO);
    /* DAC trigger enable */
    dac_trigger_enable(DAC0, DAC_OUT0);
    /* DAC wave mode config */
    dac_wave_mode_config(DAC0, DAC_OUT0, DAC_WAVE_DISABLE);

    /* DAC enable */
    dac_enable(DAC0, DAC_OUT0);
    /* DAC DMA function enable */
    dac_dma_enable(DAC0, DAC_OUT0);
}

/*!
    \brief      configure TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void timer5_config(void)
{
    timer_parameter_struct timer_initpara;

    /* TIMER deinitialize */
    timer_deinit(TIMER5);

    /* TIMER configuration */
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler         = 119;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;

    /* initialize TIMER init parameter struct */
    timer_init(TIMER5, &timer_initpara);

    /* TIMER master mode output trigger source: Update event */
    timer_master_output_trigger_source_select(TIMER5, TIMER_TRI_OUT_SRC_UPDATE);

    /* enable TIMER */
    timer_enable(TIMER5);
}
