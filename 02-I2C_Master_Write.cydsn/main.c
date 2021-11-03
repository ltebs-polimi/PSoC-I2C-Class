/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <stdio.h>
#include "project.h"
#include "../src_shared/I2C_Interface.h"
#include "../src_shared/LIS3DH.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    I2C_Peripheral_Start();
    UART_1_Start();
    
    CyDelay(5); //"The boot procedure is complete about 5 ms after device power-up."
    
    // Check if LIS3DH is connected
    uint32_t rval = I2C_Master_MasterSendStart(LIS3DH_DEVICE_ADDRESS, I2C_Master_WRITE_XFER_MODE);
    if( rval == I2C_Master_MSTR_NO_ERROR ) {
        UART_1_PutString("LIS3DH found @ address 0x18\r\n");
    }
    I2C_Master_MasterSendStop();
    
    // String to print out messages over UART
    char message[50] = {'\0'};
    
    /******************************************/
    /*            I2C Writing                 */
    /******************************************/
    
    uint8_t ctrl_reg1 = 0x00;
    ErrorCode error;
    
    // Your time to code!
    
    // Write CTRL_REG1 and...
    if ( ctrl_reg1 != 0x47 ) {
        UART_1_PutString("\r\nWriting to CTRL_REG1...\r\n");
        
        //ctrl_reg1 = LIS3DH_NORMAL_MODE_CTRL_REG1;
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             LIS3DH_NORMAL_MODE_CTRL_REG1);
    
        if( error == NO_ERROR ) {
            sprintf(message, "CTRL_REG1 successfully written as: 0x%02X\r\n", LIS3DH_NORMAL_MODE_CTRL_REG1);
            UART_1_PutString(message); 
        }
        else
        {
            UART_1_PutString("I2C error while writing CTRL_REG1\r\n");   
        }
    }
    
    
    // ... read the value of CTRL_REG1 and make sure
    // you get the value written before
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        &ctrl_reg1);
    
    if( error == NO_ERROR ) {
        sprintf(message, "CTRL_REG1 after (over-)write operation: 0x%02X\r\n", ctrl_reg1);
        UART_1_PutString(message); 
    }
    else {
        UART_1_PutString("I2C error while reading CTRL_REG1\r\n");   
    }
    
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
