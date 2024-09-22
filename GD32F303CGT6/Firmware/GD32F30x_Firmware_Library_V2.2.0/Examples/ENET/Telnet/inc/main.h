/*!
    \file    main.h
    \brief   the header file of main 

    \version 2017-02-10, V1.0.0, firmware for GD32F30x
    \version 2018-10-10, V1.1.0, firmware for GD32F30x
    \version 2018-12-25, V2.0.0, firmware for GD32F30x
    \version 2020-09-30, V2.1.0, firmware for GD32F30x
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

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

#ifndef __MAIN_H
#define __MAIN_H

#include "gd32f30x.h"
#include "stdint.h"
#include "gd32f30x_enet_eval.h"

//#define USE_DHCP       /* enable DHCP, if disabled static address is used */

//#define USE_ENET_INTERRUPT
//#define TIMEOUT_CHECK_USE_LWIP

/* MAC address: BORAD_MAC_ADDR0:BORAD_MAC_ADDR1:BORAD_MAC_ADDR2:BORAD_MAC_ADDR3:BORAD_MAC_ADDR4:BORAD_MAC_ADDR5 */
#define BORAD_MAC_ADDR0   2
#define BORAD_MAC_ADDR1   0xA
#define BORAD_MAC_ADDR2   0xF
#define BORAD_MAC_ADDR3   0xE
#define BORAD_MAC_ADDR4   0xD
#define BORAD_MAC_ADDR5   6
 
/* static IP address: BORAD_IP_ADDR0.BORAD_IP_ADDR1.BORAD_IP_ADDR2.BORAD_IP_ADDR3 */
#define BORAD_IP_ADDR0   10
#define BORAD_IP_ADDR1   50
#define BORAD_IP_ADDR2   3
#define BORAD_IP_ADDR3   210
   
/* net mask */
#define BORAD_NETMASK_ADDR0   255
#define BORAD_NETMASK_ADDR1   255
#define BORAD_NETMASK_ADDR2   255
#define BORAD_NETMASK_ADDR3   0

/* gateway address */
#define BORAD_GW_ADDR0   10
#define BORAD_GW_ADDR1   50
#define BORAD_GW_ADDR2   3
#define BORAD_GW_ADDR3   1

/* MII and RMII mode selection */
#define RMII_MODE  // user have to provide the 50 MHz clock by soldering a 50 MHz oscillator
//#define MII_MODE

/* clock the PHY from external 25MHz crystal (only for MII mode) */
#ifdef  MII_MODE
#define PHY_CLOCK_MCO
#endif

/* function declarations */
/* updates the system local time */
void time_update(void);
/* insert a delay time */
void delay_10ms(uint32_t ncount);

#endif /* MAIN_H */