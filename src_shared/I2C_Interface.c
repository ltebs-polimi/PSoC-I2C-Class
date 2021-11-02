/*
* This file includes all the required source code to interface
* the I2C peripheral.
*
* This file needs to be completed.
*/

/**
*   \brief Value returned if device present on I2C bus.
*/
#ifndef DEVICE_CONNECTED
    #define DEVICE_CONNECTED 1
#endif

/**
*   \brief Value returned if device not present on I2C bus.
*/
#ifndef DEVICE_UNCONNECTED
    #define DEVICE_UNCONNECTED 0
#endif

#include "I2C_Interface.h" 
#include "I2C_Master.h"

ErrorCode I2C_Peripheral_Start(void) 
{
    // Start I2C peripheral
    I2C_Master_Start();  
    
    // Return no error since start function does not return any error
    return NO_ERROR;
}

// --------------------------------------------------------------------------------- //

ErrorCode I2C_Peripheral_Stop(void)
{
    // Stop I2C peripheral
    I2C_Master_Stop();
    // Return no error since stop function does not return any error
    return NO_ERROR;
}

// --------------------------------------------------------------------------------- //

ErrorCode I2C_Peripheral_ReadRegister(uint8_t device_address, 
                                      uint8_t register_address,
                                      uint8_t* data) {
    
    // TODO
    
}
                                    
// --------------------------------------------------------------------------------- //

ErrorCode I2C_Peripheral_ReadRegisterMulti(uint8_t device_address,
                                           uint8_t register_address,
                                           uint8_t register_count,
                                           uint8_t* data) {
    // TODO
}
                                        
// --------------------------------------------------------------------------------- //

ErrorCode I2C_Peripheral_WriteRegister(uint8_t device_address,
                                       uint8_t register_address,
                                       uint8_t data) {
    // TODO
}
                                    
// --------------------------------------------------------------------------------- //

ErrorCode I2C_Peripheral_WriteRegisterMulti(uint8_t device_address,
                                            uint8_t register_address,
                                            uint8_t register_count,
                                            uint8_t* data) {
    // TODO
}
                                            
// --------------------------------------------------------------------------------- //

uint8_t I2C_Peripheral_IsDeviceConnected(uint8_t device_address) {
    
    // Send a start condition followed by a stop condition
    uint8_t error = I2C_Master_MasterSendStart(device_address, I2C_Master_WRITE_XFER_MODE);
    I2C_Master_MasterSendStop();
    // If no error generated during stop, device is connected
    return (error == I2C_Master_MSTR_NO_ERROR ? DEVICE_CONNECTED : DEVICE_UNCONNECTED);
    
}

/* [] END OF FILE */
