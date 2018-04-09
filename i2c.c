#include "i2c.h"


// Initialize Reading Sequence from OMRON 44L-D6T
void i2c_read_command() {
    i2c_Start();                        // send Start bit
    i2c_Address(I2C_SLAVE, I2C_WRITE);  // Write to slave address
    i2c_Write(0x4C);                    // Send slave command (0x4C)
    i2c_Restart();                      // Restart bit
    i2c_Address(I2C_SLAVE, I2C_READ);   // Read from slave address
    
    return;
}

void main(void) {
    ADCON1 = 0xFF;
    TRISAbits.TRISA5 = 0;
    i2c_Init();                         // Configure i2c protocol
    //__delay_ms(1000);
    while(1)
    {
        char read_buff[35]; // buffer for data reception
        int tPTAT, tP[16], tPEC; // variables for storing temperature calculations
                
        ///////////////////////////////////////////////
        ///////////// I2C PROTOCOL /////////////
        i2c_read_command();
                
        // return 35 bytes of data from Slave device
        read_buff[0] = i2c_Read(1);
        read_buff[1] = i2c_Read(1);
        read_buff[2] = i2c_Read(1);
        read_buff[3] = i2c_Read(1);
        read_buff[4] = i2c_Read(1);
        read_buff[5] = i2c_Read(1);
        read_buff[6] = i2c_Read(1);
        read_buff[7] = i2c_Read(1);
        read_buff[8] = i2c_Read(1);
        read_buff[9] = i2c_Read(1);
        read_buff[10] = i2c_Read(1);
        read_buff[11] = i2c_Read(1);
        read_buff[12] = i2c_Read(1);
        read_buff[13] = i2c_Read(1);
        read_buff[14] = i2c_Read(1);
        read_buff[15] = i2c_Read(1);
        read_buff[16] = i2c_Read(1);
        read_buff[17] = i2c_Read(1);
        read_buff[18] = i2c_Read(1);
        read_buff[19] = i2c_Read(1);
        read_buff[20] = i2c_Read(1);
        read_buff[21] = i2c_Read(1);
        read_buff[22] = i2c_Read(1);
        read_buff[23] = i2c_Read(1);
        read_buff[24] = i2c_Read(1);
        read_buff[25] = i2c_Read(1);
        read_buff[26] = i2c_Read(1);
        read_buff[27] = i2c_Read(1);
        read_buff[28] = i2c_Read(1);
        read_buff[29] = i2c_Read(1);
        read_buff[30] = i2c_Read(1);
        read_buff[31] = i2c_Read(1);
        read_buff[32] = i2c_Read(1);
        read_buff[33] = i2c_Read(1);
        read_buff[34] = i2c_Read(0); // last byte to be received
                    
        i2c_Stop();     // send Stop
        
        ///////////////////////////////////////////////
        ///////////// Compute Temp Values /////////////
        tPTAT = 256*read_buff[1] + read_buff[0];
        tP[0] = 256*read_buff[3] + read_buff[2];
        tP[1] = 256*read_buff[5] + read_buff[4];
        tP[2] = 256*read_buff[7] + read_buff[6];
        tP[3] = 256*read_buff[9] + read_buff[8];
        tP[4] = 256*read_buff[11] + read_buff[10];
        tP[5] = 256*read_buff[13] + read_buff[12];
        tP[6] = 256*read_buff[15] + read_buff[14];
        tP[7] = 256*read_buff[17] + read_buff[16];
        tP[8] = 256*read_buff[19] + read_buff[18];
        tP[9] = 256*read_buff[21] + read_buff[20];
        tP[10] = 256*read_buff[23] + read_buff[22];
        tP[11] = 256*read_buff[25] + read_buff[24];
        tP[12] = 256*read_buff[27] + read_buff[26];
        tP[13] = 256*read_buff[29] + read_buff[28];
        tP[14] = 256*read_buff[31] + read_buff[30];
        tP[15] = 256*read_buff[33] + read_buff[32];
        tPEC = read_buff[34];      
         
        ////////////////////////////////////////////////
        //////////////// Toggle I/O Pin ////////////////
        
        RA5 = 1;
         __delay_ms(100);
        
        if(tP[0] >= 0 || tP[6] >= 0) {
            RA5 = 0;
            __delay_ms(100);
        }      
            
        __delay_ms(200);
    }    
    
    
    
    return; //never reaches this point
}