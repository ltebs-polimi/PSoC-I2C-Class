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
    
    /*******************************************/
    /*   I2C Temperature Reading from LIS3DH   */
    /*******************************************/
    
    // I2C error return code
    ErrorCode error;
    // Register variables
    uint8_t tmp_ctrl_reg, ctrl_reg_4;
    
    // Read LIS3DH_TEMP_CFG_REG
    
    // Set it to the appropriate value
    
    
    // Read LIS3DH_CTRL_REG4
    
    // Set it to the appropriate value
    
    
    
    /*    Temperature reading and log over UART    */
    
    // Init the data BUFFER
    const uint8_t header = 0xA0;
    const uint8_t footer = 0xC0;
    uint8_t dataArray[4];
    dataArray[0] = header;
    dataArray[3] = footer;
    
    int16_t dirtyTrick = 1000;
    
    // Raw temperature data buffer
    uint8_t temp[2];
    // Output temperature data
    int16_t outTemp;
    

    for(;;)
    {
        
        // Get raw temperature data from the LIS3DH
        
        // Apply the dirty trick 
        
        // Fill the data BUFFER
        
        // Log: put the array over UART 
        
    }
}

/* [] END OF FILE */
