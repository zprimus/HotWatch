#define I2C_WRITE 0
#define I2C_READ 1

// Initialize MSSP - Master Synchronous Serial Port. (12F1822 - other devices may differ)
void i2c_Init(void){

   // Initialize I2C MSSP
   // SCL needs to be 100KHz or less for OMRON 44L-D6T
   TRISAbits.TRISA1 = 1;    // set SCL and SDA pins as inputs
   TRISAbits.TRISA2 = 1;

   SSP1CON1 = 0b00101000;   // I2C enabled, Master mode
   SSP1CON2bits.SEN = 0;
   SSP1ADD = 0x0A;          // 100Khz @ 4Mhz Fosc
   SSP1STAT = 0b11000000;   // Slew rate disabled
      
   return;
}

// i2c_Wait - wait for I2C transfer to finish
void i2c_Wait(void)
{
    while ( ( SSP1CON2 & 0x1F ) || ( SSP1STAT & 0x04 ) );
    
    return;
}

// i2c_Start - Start I2C communication
void i2c_Start(void)
{
    SSP1CON2bits.SEN = 1;  //Indicates that start bit has been detected last
    //while(SSP1CON2bits.SEN);
    
    SSP1CON3 = 0b11101000;
        
    return;
}

// i2c_Restart - Re-Start I2C communication
void i2c_Restart(void){
    SSP1CON2bits.RSEN = 1; // Initiates repeated start condition
    //while(SSP1CON2bits.RSEN);
    
    return;
}

// i2c_Stop - Stop I2C communication
void i2c_Stop(void)
{
    SSP1CON2bits.PEN=1;  //Initiate stop condition on SDA and SCL pins, auto cleared by hardware
    //while(SSP1CON2bits.PEN);
    
    return;
}

// i2c_Write - Sends one byte of data
void i2c_Write(unsigned char data)
{
    SSP1BUF = data;  //MSSP1 Receive Buffer/Transmit Register
    while(SSP1STATbits.BF);       // wait till complete data is sent from buffer 
    //i2c_Wait();
    //SSP1CON2bits.ACKDT = 0;
    SSP1CON2bits.ACKDT = 0;
    SSP1CON2bits.ACKEN = 1;                    // send acknowledge sequence
    //while(SSP1CON2bits.ACKEN);
    
    return;
}

// i2c_Address - Sends Slave Address and Read/Write mode
// mode is either I2C_WRITE or I2C_READ
void i2c_Address(unsigned char address, unsigned char mode)
{
    unsigned char l_address;

    l_address=address<<1;       // Shifts slave address (0x0A)
    l_address+=mode;            // Adds R/W bit to Address to make 14h or 15h
    SSP1BUF = l_address;
    while(SSP1STATbits.BF);     // wait till complete data is sent from buffer
    
    SSP1CON2bits.ACKDT = 0;
    SSP1CON2bits.ACKEN = 1;     // send acknowledge sequence
    //while(SSP1CON2bits.ACKEN);
    
    return;
}

// i2c_Read - Reads a byte from Slave device
unsigned char i2c_Read(unsigned char ack)
{
    // ack should be 1 if there is going to be more data read
    // ack should be 0 if this is the last byte of data read
    
    unsigned char i2cReadData;
    
    i2c_Wait();

    SSP1CON2bits.RCEN=1;
    while(!(SSP1STATbits.BF));      // wait for receive complete
    i2cReadData = SSP1BUF;
    if ( ack ) {
        SSP1CON2bits.ACKDT=0;         // Ack
    } else {
        SSP1CON2bits.ACKDT=1;         // NAck
    }
    SSP1CON2bits.ACKEN=1;             // send acknowledge sequence

    return( i2cReadData );
}