
#include <xc.h>
#include <stdint.h>
#include "config.h"
#define _XTAL_FREQ 4000000

void EEPROM_Write(uint8_t address , uint8_t data){
    uint8_t GIE_STATE = GIE; //save the current state of the global interrupt
    while(EECON1bits.WR); // Waits Until Last Attempt To Write Is Finished
    EEADR = address; // Writes The Addres To Which We'll Wite Our Data
    EEDATA = data; // Write The Data To Be Saved
    EECON1bits.EEPGD = 0; // Cleared To Point To EEPROM Not The Program Memory
    EECON1bits.WREN = 1; // Enable The Operation !
    GIE = 0; // Disable All Interrupts Untill Writting Data Is Done
    EECON2 = 0x55; // Part Of Writing Mechanism..
    EECON2 = 0xAA; // Part Of Writing Mechanism..
    EECON1bits.WR = 1; // Part Of Writing Mechanism..
    GIE = GIE_STATE; // Re-Enable Interrupts to it's state before
    EECON1bits.WREN = 0; // Disable The Operation
    EECON1bits.WR = 0; // Ready For Next Writting Operation
}
uint8_t EEPROM_Read(uint8_t address){
    uint8_t data = 0; 
    EEADR = address; // Write The Address From Which We Wonna Get Data
    EECON1bits.EEPGD = 0; // Cleared To Point To EEPROM Not The Program Memory
    EECON1bits.RD = 1; // Start The Read Operation
    data = EEDATA; // Read The Data
    return data;
}
void main(void) {
    uint8_t address = 0;
    TRISB = 0x0F; //input pins for push buttons
    TRISD = 0x00; //output for LEDS
    PORTD = 0x00; //intially off
    while(1){
        if(RB1){
            EEPROM_Write(address++ , 3);
            __delay_ms(500);
        }
        if(RB2){
            EEPROM_Write(address++ , 5);
            __delay_ms(500);
        }
        if(RB3){
            EEPROM_Write(address++ , 7);
            __delay_ms(500);
        }
        if(address == 3) address = 0;
        if(RB0){
            for(uint8_t i = 0; i < 3 ; i++){
                PORTD = EEPROM_Read(i);
                __delay_ms(1000);
            }
            PORTD = 0x00;
        }
    }
    return;
}
