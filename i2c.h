#include <xc.h>

//PIC12F1822 Configuration Bit Settings
#define I2C_SLAVE 0x0A  // address without R/W bit, bit shifted left to make 14h or 15h
#define _XTAL_FREQ 4000000 // frequency for __delay_ms

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = ON          // Flash Program Memory Code Protection (Program memory code protection is enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
//#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)


#define I2C_WRITE 0
#define I2C_READ 1

// Initialize MSSP - Master Synchronous Serial Port. (12F1822 - other devices may differ)
void i2c_Init(void){
    //OSCCON = 0b01101010;                // set internal osc to 4MHz
    //TRISA = 0b000000;                   // set ports to outputs
    //LATA = 0b000000;
    //PORTA = 0b000000;
    
   // Initialize I2C MSSP
   SSP1CON1 = 0b00101000;   // I2C enabled, Master mode
   //SSP1CON2bits.SEN = 0;
   SSP1CON2 = 0x00;
   SSP1ADD = I2C_SLAVE; // F_clock of SCL = F_osc/(4*(SSP1ADD + 1)) = 90.9kHz < 100kHz
   SSP1STAT = 0x00;   // Slew rate disabled
   //SSP1CON3 = 0b11111000;
   
   // SCL needs to be 100KHz or less for OMRON 44L-D6T   
   TRISAbits.TRISA1 = 1;    // set SCL and SDA pins as inputs
   TRISAbits.TRISA2 = 1;
    
          
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
    i2c_Wait();
    
    //while ( ( SSP1CON2 & 0x1F ) || ( SSP1STAT & 0x04 ) ) {
    SSP1CON2bits.SEN = 1;  //Indicates that start bit has been detected last
    //while(SSP1CON2bits.SEN);
    //}
    //SSP1CON3 = 0b11101000;
        
    return;
}

// i2c_Restart - Re-Start I2C communication
void i2c_Restart(void){
    i2c_Wait();
    
    SSP1CON2bits.RSEN = 1; // Initiates repeated start condition
    //while(SSP1CON2bits.RSEN);
    //SSP1CON3 = 0b11111000;
    
    return;
}

// i2c_Stop - Stop I2C communication
void i2c_Stop(void)
{
    i2c_Wait();
    
    SSP1CON2bits.PEN=1;  //Initiate stop condition on SDA and SCL pins, auto cleared by hardware
    //while(SSP1CON2bits.PEN);
    
    return;
}

// i2c_Write - Sends one byte of data
void i2c_Write(unsigned char data)
{
    i2c_Wait();
    
    SSP1BUF = data;  //MSSP1 Receive Buffer/Transmit Register
    
    //i2c_Wait();
    //SSP1CON2bits.ACKDT = 0;
    //SSP1CON2bits.ACKDT = 0;
    //SSP1CON2bits.ACKEN = 1;                    // send acknowledge sequence
    //while(SSP1CON2bits.ACKEN);
    
    return;
}

// i2c_Address - Sends Slave Address and Read/Write mode
// mode is either I2C_WRITE or I2C_READ
void i2c_Address(unsigned char address, unsigned char mode)
{
    i2c_Wait();
    
    unsigned char l_address;

    l_address=address<<1;       // Shifts slave address (0x0A)
    l_address+=mode;            // Adds R/W bit to Address to make 14h or 15h
    SSP1BUF = l_address;
    
    //SSP1CON2bits.ACKDT = 0;    
    //SSP1CON2bits.ACKEN = 1;     // send acknowledge sequence
        
    //while(SSP1CON2bits.ACKEN);
    
    return;
}

// i2c_Read - Reads a byte from Slave device
char i2c_Read(unsigned short ack)
{
    // ack should be 1 if there is going to be more data read
    // ack should be 0 if this is the last byte of data read
    
    char i2cReadData;
    
    i2c_Wait();

    SSP1CON2bits.RCEN=1;
    i2c_Wait();
    
    i2cReadData = SSP1BUF;
    i2c_Wait();
    
    if(ack == 1) {
        SSP1CON2bits.ACKDT = 0;         // Ack
    } else {
        SSP1CON2bits.ACKDT = 1;
    }
    SSP1CON2bits.ACKEN = 1;             // send acknowledge sequence
    
    return i2cReadData;
}