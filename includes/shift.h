#include <avr/io.h>
#include <util/delay.h>

#include "io.h"

#define DATA 2		//Data pin location
#define SHIFT 3	//Shift Clock pin location
#define STORE 4	//Store Clock pin location


unsigned char Set_Bit(unsigned char output, unsigned char in){
    return (output | (1 << in));   
}
unsigned char Clear_Bit(unsigned char output, unsigned char in){
    return (output & ~(1 << in));
} 

void Shift_Init() {
    DDRC = Set_Bit(DDRC, DATA);
    DDRC = Set_Bit(DDRC, SHIFT);
    DDRC = Set_Bit(DDRC, STORE);
}

//Set Data line high
void Shift_Data_High() {
    PORTC = Set_Bit(PORTC, DATA);
}

//Set Data line low
void Shift_Data_Low() {
    PORTC = Clear_Bit(PORTC, DATA);
}

//Pulse Shift Clock
void Shift_Pulse() {
    PORTC = Set_Bit(PORTC, SHIFT);
    PORTC = Clear_Bit(PORTC, SHIFT);
}

//Pulse Store Clock
void Shift_Store() {
    PORTC = Set_Bit(PORTC, STORE);
    //SET_BIT(SHIFT_PORT,STORE_LINE);
    _delay_loop_2(1);

    //CLR_BIT(SHIFT_PORT,STORE_LINE);
    PORTC = Clear_Bit(PORTC, STORE);
    _delay_loop_2(1);
}


void Shift_transmit_data(unsigned char data) {
    for(unsigned char i=0;i<8;i++) {
        if(data & 0x80) {
            Shift_Data_High();
            } else {
            Shift_Data_Low();
        }

        Shift_Pulse();
        data=data << 1;
    }

    Shift_Store();
}