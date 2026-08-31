/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PSOC Control C1 MCU: CAN Transmitter example
*              for ModusToolbox. The CAN node 1 transmits the button status
*              periodically over the CAN bus. Successful message transmission is
*              indicated by toggling the USER LED.
*
* Related Document: See README.md
*
*******************************************************************************
* (c) 2026, Infineon Technologies AG, or an affiliate of Infineon
* Technologies AG. All rights reserved.
* This software, associated documentation and materials ("Software") is
* owned by Infineon Technologies AG or one of its affiliates ("Infineon")
* and is protected by and subject to worldwide patent protection, worldwide
* copyright laws, and international treaty provisions. Therefore, you may use
* this Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software. If no license
* agreement applies, then any use, reproduction, modification, translation, or
* compilation of this Software is prohibited without the express written
* permission of Infineon.
*
* Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
* IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
* THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
* SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
* Infineon reserves the right to make changes to the Software without notice.
* You are responsible for properly designing, programming, and testing the
* functionality and safety of your intended application of the Software, as
* well as complying with any legal requirements related to its use. Infineon
* does not guarantee that the Software will be free from intrusion, data theft
* or loss, or other breaches ("Security Breaches"), and Infineon shall have
* no liability arising out of any Security Breaches. Unless otherwise
* explicitly approved by Infineon, the Software may not be used in any
* application where a failure of the Product or any consequences of the use
* thereof can reasonably be expected to result in personal injury.
*******************************************************************************/

#include "cybsp.h"
#include "cy_utils.h"

/*****************************************************************************
* Defines
******************************************************************************/
/* Declarations for System timer timing */
#define TICKS_PER_SECOND                    10           /* Ticks per Second */


/*******************************************************************************
* Variables
*******************************************************************************/
/* Variable for keeping track of Interrupt */
static volatile bool timer_interrupt_flag = false;
/* Runtime transmit message object copy (generated object is const). */
static Cy_CAN_MO_t can_tx_msg;

/*******************************************************************************
* Function Name: SysTick_Handler
********************************************************************************
* Summary:
* Function called by system timer every 100 milliseconds. The button state is
* sampled and sent over the CAN bus to the receiver. The User LED is toggled to
* indicate successful transmission.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void SysTick_Handler(void)
{
    /* Update CAN message. Sample and send button state. */
    can_tx_msg.can_data_byte[0] = (uint8_t) Cy_GPIO_GetInput(CYBSP_USER_BTN_PORT, CYBSP_USER_BTN_PIN);

    /* Configure data to be transmitted and data length code */
    Cy_CAN_MO_UpdateData(&can_tx_msg);

    /* Send data in CAN_NODE_LMO_0 */
    Cy_CAN_MO_Transmit(&can_tx_msg);

    /* Toggle USER LED1 to indicate that the message is transmitted */
    Cy_GPIO_ToggleOutput(CYBSP_USER_LED_PORT, CYBSP_USER_LED_PIN);

    /* Timer interrupt occurs, set the flag to true  */
    timer_interrupt_flag = true;
}


/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function. This function performs
*  - initial setup of device
*  - initialize CAN transmit message object and node
*  - transmits a CAN frame periodically
*
* Parameters:
*  none
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Use a mutable runtime copy for payload updates before transmit. */
    can_tx_msg = CAN_NODE_LMO_0;

    /* Enable system timer */
    SysTick_Config(SystemCoreClock / TICKS_PER_SECOND);

    for (;;)
    {
    }
}

/* [] END OF FILE */
