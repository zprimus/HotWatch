#include <xc.h>
#include "i2c.h"

//PIC12F1822 Configuration Bit Settings

#define _XTAL_FREQ 4000000  // Fosc
#define I2C_SLAVE 0x0A  // address without R/W bit, bit shifted left to make 14h or 15h

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

// Initialize Reading Sequence from OMRON 44L-D6T
void i2c_read_command(unsigned char address)
{
    i2c_Start();                        // send Start bit
    i2c_Address(I2C_SLAVE, I2C_WRITE);  // Write to slave address
    i2c_Write(0x4C);                    // Send slave command (0x4C)
    i2c_Restart();                      // Restart bit
    i2c_Address(I2C_SLAVE, I2C_READ);   // Read from slave address
    
    return;
}

void main(void) {

    OSCCON = 0b01101010;                // set internal osc to 8MHz
    TRISA = 0b000000;                   // set ports to outputs
    LATA = 0b000000;
    PORTA = 0b000000;

    i2c_Init();                         // Start I2C as Master 100KHz

    while(1)
    {
        unsigned char read_buff[35];
        
        __delay_ms(10);
        i2c_read_command(I2C_SLAVE);
        read_buff[0] = i2c_Read(0);            // Read one byte
        
        
        //read_buff[0] = i2c_Read(1);            // Read one byte
        //read_buff[1] = i2c_Read(1);            // Read one byte
        //read_buff[2] = i2c_Read(0);            // Read one byte
        
        
        
        /*
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
        read_buff[34] = i2c_Read(0); //last byte
        */
        
        i2c_Stop();     // send Stop
    }
    
    
    return; //never reaches this point
}