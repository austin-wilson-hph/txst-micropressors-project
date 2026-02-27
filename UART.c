//Spring 2026
//Austin Wilson
//UART.c
//02/19/2026
//Implementation of the UART1 serial communication for the Blackboard hardware using memory-mapped I/O.

#include <stdint.h> // so I can use the integers I want
#include <stdio.h> // so I can use sprintf
#include "sleep.h"
#define UART1_CR *((uint32_t *) 0xE0001000)// UART1 control register
#define UART1_MR *((uint32_t *) 0xE0001004)// UART1 Mode register
#define UART1_BAUDGEN *((uint32_t *) 0xE0001018)
#define UART1_BAUDDIV *((uint32_t *) 0xE0001034)
#define UART1_SR *((uint32_t *) 0xE000102C) //UART1 status reg
#define UART1_DATA *((uint32_t *) 0xE0001030) //UART1 TX/RX FIFO DATA
#define BaudGen115200 0x7c
#define BaudDiv115200 6

int isPrime(int userNum) {
    for (int i = 2; i < userNum; i++) {
        if (userNum % i == 0) {
            return 0; //not prime
        }
    }
    return 1; //prime
}

int uart1_tx_full()
{
    return (UART1_SR & 16)!=0; //Check SR register bit4, return 1 if fifo full
}
int UART1_RXEmpty() {
    uint32_t UARTData; // general variable to hold data  to/from UART
    UARTData = UART1_SR;
    if ((UARTData & 0x02) == 2){
        return 1;
    }else {
        return 0;
    }
}
void ResetUART1() {
    UART1_CR = 3;  // assert the reset of the UART
    while ((UART1_CR & 0x2)  == 2){} // wait till xmit reset clears
    while ((UART1_CR & 0x1)  == 1){} // wait till rcv reset cleared by the UART
    }
    
void Configure_UART1() {

    uint32_t UARTData;// general variable to hold data  to/from UART
    //configure the mode register
    //normal mode bits [9:8] = 2b00
    // mode = auto echo bits [9:8] =01
    //one stop bit, bits [7:6] = 2b00
    //8 data bits, bits [2:1] = 2b0x
    //use reference clock, bit0 = 0
    //disable parity, bits[5:3] = 3b1xx
    //6b100000 == 0x20 // no echoe
    // 9b100000 == 0x120 // auto echo

    UART1_MR = 0x20;
    // enable the transmit and recieve  OR 0x14 to CR register
    UARTData = UART1_CR; //get the current setting and OR in the
    UARTData |= 0x14;    // Enable TX and RX
    UART1_CR = UARTData;
}
void SetBaudrate1() {
    // set to 115200 baudrate
    UART1_BAUDGEN = BaudGen115200;
    UART1_BAUDDIV = BaudDiv115200;
}
void initUart1() {
    //Set to 115200 8,n,1
    ResetUART1(); // reset the uart to start
    Configure_UART1(); // configure to 8,n,1
    SetBaudrate1(); // set baudrate to 115200
}

void uartPutchar(char c) {
    while(uart1_tx_full()) {}
    UART1_DATA = c;
}

void uartPutString(char *s) {
    int i = 0;
    while(s[i] != '\0') {
        uartPutchar(s[i]);
        i++;
    }
}

int main(void) {
    initUart1();
    while(1) {
        int index = 0;
        uartPutString("Enter a number >= 2 then press Enter: \r\n");
        char userInput[20];
    
        while(1) {
            while(UART1_RXEmpty());
            char c = UART1_DATA;
            uartPutchar(c);
            if(c == '\r') { break; }
            userInput[index] = c;
            index++;
        }
        userInput[index] = '\0';
        int userNum = atoi(userInput);
        
        int userOGNum = userNum;
        
        if (userNum < 2) {
            uartPutString("Invalid input, must be >= 2\r\n");
            continue;
        }
        while(isPrime(userNum) == 0) {
            userNum--;
        }

        char result[20];
        sprintf(result, "Your number was %d\r\n", userOGNum);
        uartPutString(result);
        sprintf(result, "The largest prime is %d\r\n", userNum);
        uartPutString(result);
    }

}
