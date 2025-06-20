/**********************************************************************************************************************
 * \file Cpu0_Main.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 * 
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of 
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 * 
 * Boost Software License - Version 1.0 - August 17th, 2003
 * 
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and 
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all 
 * derivative works of the Software, unless such copies or derivative works are solely in the form of 
 * machine-executable object code generated by a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE 
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN 
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 *********************************************************************************************************************/
 /*\title MULTICAN FD data transmission
 * \abstract MULTICAN in Flexible Data-Rate mode is used to exchange data between two nodes, implemented in the same device using Loop-Back mode.
 * \description The CAN messages are sent from CAN node 0 to CAN node 1 using Loop-Back mode.
 *              Both CAN nodes are set to CAN Flexible Data-rate mode (CAN FD). After each CAN message
 *              transmission and successful reception, an interrupt is generated. Inside the interrupt service routine,
 *              the content of the received CAN message is read. In case of the successful read operation, the
 *              received data is compared to the transmitted data. If all messages are received without any error
 *              detected, the LED is turned on to confirm successful message reception.
 *
 * \name MULTICAN_FD_1_KIT_TC275_LK
 * \version V1.0.0
 * \board AURIX TC275 lite Kit, KIT_AURIX_TC275_LITE, TC27xTP_D-Step
 * \keywords AURIX, CAN, FD, MULTICAN, MULTICAN_FD_1
 * \documents https://www.infineon.com/aurix-expert-training/Infineon-AURIX_MULTICAN_FD_1_KIT_TC275_LK-TR-v01_00_00-EN.pdf
 * \documents https://www.infineon.com/aurix-expert-training/TC27D_iLLD_UM_1_0_1_12_0.chm
 * \lastUpdated 2021-06-29
 *********************************************************************************************************************/
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "MULTICAN_FD.h"

uint8 g_currentCanFdUseCase = 0;
IfxCpu_syncEvent g_cpuSyncEvent = 0;

int core0_main(void)
{

    initTransceiver(); // Initialize the transceiver
        initCan();         // Initialize CAN module

        while (1)
        {
            transmitCanMessage(); // Send a CAN message
            for (volatile int i = 0; i < 1000000; i++); // Delay to prevent flooding
            receiveCanMessage();  // Check for received messages
        }
    return (1);
}
