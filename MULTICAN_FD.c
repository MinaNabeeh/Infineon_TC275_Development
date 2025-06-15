#include "Ifx_Types.h"
#include "IfxPort.h"
#include "IfxMultican_Can.h"

void initTransceiver(void)
{
    // Configure P20.6 as a push-pull output
    IfxPort_setPinModeOutput(&MODULE_P20, 6, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    // Drive P20.6 LOW to enable normal operation
    IfxPort_setPinLow(&MODULE_P20, 6);
}


IfxMultican_Can g_canModule;
IfxMultican_Can_Node g_canNode;
IfxMultican_Can_MsgObj g_canTxMsgObj;
IfxMultican_Can_MsgObj g_canRxMsgObj;

void initCan(void)
{
    // Step 1: Initialize the CAN module
    IfxMultican_Can_Config canConfig;
    IfxMultican_Can_initModuleConfig(&canConfig, &MODULE_CAN);
    IfxMultican_Can_initModule(&g_canModule, &canConfig);

    // Step 2: Configure the CAN node
    IfxMultican_Can_NodeConfig canNodeConfig;
    IfxMultican_Can_Node_initConfig(&canNodeConfig, &g_canModule);
    canNodeConfig.nodeId = IfxMultican_NodeId_0; // CAN Node 0
    canNodeConfig.rxPin = &IfxMultican_RXD0B_P20_7_IN; // RX Pin: P20.7
    canNodeConfig.txPin = &IfxMultican_TXD0_P20_8_OUT; // TX Pin: P20.8
    canNodeConfig.loopBackMode = FALSE; // Normal mode
    IfxMultican_Can_Node_init(&g_canNode, &canNodeConfig);

    // Step 3: Configure the TX message object
    IfxMultican_Can_MsgObjConfig canMsgObjConfig;
    IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &g_canNode);
    canMsgObjConfig.msgObjId = 0; // Message Object ID 0 for TX
    canMsgObjConfig.messageId = 0x100; // CAN Message ID
    canMsgObjConfig.frame = IfxMultican_Frame_transmit;
    canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
    IfxMultican_Can_MsgObj_init(&g_canTxMsgObj, &canMsgObjConfig);

    // Step 4: Configure the RX message object
    canMsgObjConfig.msgObjId = 1; // Message Object ID 1 for RX
    canMsgObjConfig.messageId = 0x100; // Same Message ID for RX
    canMsgObjConfig.frame = IfxMultican_Frame_receive;
    IfxMultican_Can_MsgObj_init(&g_canRxMsgObj, &canMsgObjConfig);
}
void transmitCanMessage(void)
{
    IfxMultican_Message txMsg;
    IfxMultican_Message_init(&txMsg, 0x100, 0xDEADBEEF, 0xCAFEBABE, IfxMultican_DataLengthCode_8);

    IfxMultican_Status status = IfxMultican_Can_MsgObj_sendMessage(&g_canTxMsgObj, &txMsg);

    if (status == IfxMultican_Status_ok)
    {
        IfxPort_togglePin(&MODULE_P00, 5); // Toggle LED1 on success
    }
    else
    {
        IfxPort_togglePin(&MODULE_P00, 6); // Toggle LED2 on failure
    }
}

void receiveCanMessage(void)
{
    IfxMultican_Message rxMsg;
    IfxMultican_Message_init(&rxMsg, 0x100, 0, 0, IfxMultican_DataLengthCode_8);

    if (IfxMultican_Can_MsgObj_readMessage(&g_canRxMsgObj, &rxMsg) == IfxMultican_Status_ok)
    {
        IfxPort_togglePin(&MODULE_P00, 6); // Toggle LED2 on successful reception
    }
}
