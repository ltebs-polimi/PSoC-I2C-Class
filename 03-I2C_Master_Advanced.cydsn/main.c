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

// Set this to 1 to send byte data for the Bridge Control Panel
// Otherwise set it to 0 to send temperature data as int16_t
#define USE_BRIDGECONTROLPANEL  0

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    I2C_Peripheral_Start();
    UART_1_Start();
    
    CyDelay(5); //"The boot procedure is complete about 5 ms after device power-up."
    
    UART_1_PutString("*** Project 3 - I2C Master Advanced\r\n");
    
    // Check if LIS3DH is connected
    uint32_t rval = I2C_Master_MasterSendStart(LIS3DH_DEVICE_ADDRESS, I2C_Master_WRITE_XFER_MODE);
    if( rval == I2C_Master_MSTR_NO_ERROR ) {
        UART_1_PutString("LIS3DH found @ address 0x18\r\n");
    }
    I2C_Master_MasterSendStop();
    
    // String to print out messages over UART
    char message[50] = {'\0'};
    
    /*******************************************/
    /*   I2C Temperature Reading from LIS3DH   */
    /*******************************************/
    
    // I2C error return code
    ErrorCode error;
    // Register variables
    uint8_t tmp_cfg_reg, ctrl_reg4;
    
    // Only for debugging: reboot memory content
    I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS, 0x24, 0x01);
    CyDelay(10);
    I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS, 0x24, 0x00);
    CyDelay(10);
    
    // Your time to code!
    
    // Read LIS3DH_TEMP_CFG_REG
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_TEMP_CFG_REG,
                                        &tmp_cfg_reg);
    
    if (error == NO_ERROR) {
        sprintf(message, "LIS3DH_TEMP_CFG_REG value: 0x%02X\r\n", tmp_cfg_reg);
        UART_1_PutString(message); 
    }
    else {
        UART_1_PutString("I2C error while reading LIS3DH_TEMP_CFG_REG\r\n");   
    }
    
    // Set it to the appropriate value
    tmp_cfg_reg |= LIS3DH_TEMP_CFG_REG_ACTIVE;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_TEMP_CFG_REG,
                                         tmp_cfg_reg);
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_TEMP_CFG_REG,
                                        &tmp_cfg_reg);
    if( error == NO_ERROR ) {
        sprintf(message, "LIS3DH_TEMP_CFG_REG successfully written as: 0x%02X\r\n", tmp_cfg_reg);
        UART_1_PutString(message); 
    }
    else {
        UART_1_PutString("I2C error while writing LIS3DH_TEMP_CFG_REG\r\n");   
    }
    
    
    // Read LIS3DH_CTRL_REG4
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        &ctrl_reg4);
    
    if (error == NO_ERROR) {
        sprintf(message, "LIS3DH_CTRL_REG4 value: 0x%02X\r\n", ctrl_reg4);
        UART_1_PutString(message); 
    }
    else {
        UART_1_PutString("I2C error while reading LIS3DH_CTRL_REG4\r\n");   
    }
    
    // Set it to the appropriate value
    ctrl_reg4 = LIS3DH_CTRL_REG4_BDU_ACTIVE;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG4,
                                         ctrl_reg4);
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        &ctrl_reg4);
    if( error == NO_ERROR ) {
        sprintf(message, "LIS3DH_CTRL_REG4 successfully written as: 0x%02X\r\n", ctrl_reg4);
        UART_1_PutString(message); 
    }
    else {
        UART_1_PutString("I2C error while writing LIS3DH_CTRL_REG4\r\n");   
    }
    
    // Make sure we set also CTRL_REG1 : write 0x47 to activate the device
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG1,
                                         LIS3DH_NORMAL_MODE_CTRL_REG1);
    uint8_t ctrl_reg1;
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        &ctrl_reg1);
    if( error != NO_ERROR || ctrl_reg1 != LIS3DH_NORMAL_MODE_CTRL_REG1 )
        UART_1_PutString("I2C error activating the device (CTRL_REG1)\r\n");
    
    /**   Temperature reading and log over UART   **/
    
    // Init the data BUFFER
    const uint8_t header = 0xA0;
    const uint8_t footer = 0xC0;
    uint8_t dataArray[4] = {0};
    dataArray[0] = header;
    dataArray[3] = footer;
    
    int16_t dirtyTrick = 1000;
    
    // Raw temperature data buffer
    uint8_t raw_temp[2];
    // Output temperature data
    int16_t out_temp;
    

    for(;;)
    {
        
        CyDelay(50); // Sample at approx. 20 Hz
        
        // Get raw temperature data from the LIS3DH
        error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                                 LIS3DH_OUT_ADC_3L,
                                                 2,
                                                 raw_temp);
        
        // Convert the value
        out_temp = ((int16_t)((raw_temp[0] | (raw_temp[1] << 8))) >> 6) & 0x03FF;
        
        
        #if USE_BRIDGECONTROLPANEL
            // Send Raw Data Bytes
            dataArray[1] = (uint8_t) (out_temp & 0xFF);
            dataArray[2] = (uint8_t) (out_temp >> 8);
            UART_1_PutArray(dataArray, 4);
        #else
            // Send the uint16_t value
            // Apply the dirty trick 
            out_temp *= dirtyTrick;
            sprintf(message, "0x%02x 0x%02x | %d\r\n", raw_temp[0], raw_temp[1], out_temp);
            UART_1_PutString(message);
        #endif
         
        
    }
    
    return 0;
    
}

/* [] END OF FILE */
