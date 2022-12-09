arduino library 
stepper motor
lcd
clock
dht11 sensor
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <DS3231.h>
#include <dht_nonblocking.h>


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
void adc_init()
{
  // setup the A register
  *my_ADCSRA |= 0b10000000; // set bit   7 to 1 to enable the ADC
  *my_ADCSRA &= 0b11011111; // clear bit 5 to 0 to disable the ADC trigger mode
  *my_ADCSRA &= 0b11011111; // clear bit 5 to 0 to disable the ADC interrupt
  *my_ADCSRA &= 0b11000000; // clear bit 5 to 0 to set prescaler selection to slow reading
  // setup the B register
  *my_ADCSRB &= 0b11110111; // clear bit 3 to 0 to reset the channel and gain bits
  *my_ADCSRB &= 0b11111000; // clear bit 2-0 to 0 to set free running mode
  // setup the MUX Register
  *my_ADMUX  &= 0b01111111; // clear bit 7 to 0 for AVCC analog reference
  *my_ADMUX  |= 0b01000000; // set bit   6 to 1 for AVCC analog reference
  *my_ADMUX  &= 0b11011111; // clear bit 5 to 0 for right adjust result
  *my_ADMUX  &= 0b11100000; // clear bit 4-0 to 0 to reset the channel and gain bits
}
unsigned int adc_read(unsigned char adc_channel_num)
{
  // clear the channel selection bits (MUX 4:0)
  *my_ADMUX  &= 0b11100000;
  // clear the channel selection bits (MUX 5)
  *my_ADCSRB &= 0b11011111;
  // set the channel number
  if(adc_channel_num > 7)
  {
    // set the channel selection bits, but remove the most significant bit (bit 3)
    adc_channel_num -= 8;
    // set MUX bit 5
    *my_ADCSRB |= 0b00100000;
  }
  // set the channel selection bits
  *my_ADMUX  += adc_channel_num;
  // set bit 6 of ADCSRA to 1 to start a conversion
  *my_ADCSRA |= 0x40;
  // wait for the conversion to complete
  while((*my_ADCSRA & 0x40) != 0);
  // return the result in the ADC data register
  return *my_ADC_DATA;
  
 void disabled()
 {
  //•DISABLED
//– YELLOW LED should be ON
//– No monitoring of temperature or water should be performed
   //– Start button should be monitored using an ISR
}
 void idle()
 {
   //•IDLE
//– Exact time stamp (using real time clock) should record transition times
//– Water level should be continuously monitored and state changed to error if level is too low
//– GREEN LED should be ON
 }
  
 void error()
 {
   
   //•ERROR
//– Motor should be off and not start regardless of temperature
//– A reset button should trigger a change to the IDLE stage if the water level is above the threshold
//– Error message should be displayed on LCD
//– RED LED should be turned on (all other LEDs turned off)
 }
  
  void running()
  {
    //•RUNNING
//– Fan motor should be on
//– System should transition to IDLE as soon as temperature drops below threshold
//– System should transition to ERROR state if water becomes too low
//– BLUE LED should be turned on (all other LEDs turned off)
  }
  
void waterLevel()
{
  
}

void checkRunning()
{
  
}
void LCDandDHT()
{
  
}
void fan()
  {
    
  }
void stepper()
{
  
}

}
//•All States
//– The realtime clock must be used to report (via the Serial port) the time of each state transition,
//and any changes to the stepper motor position for the vent.

//•All states except DISABLED
//– Humidity and temperature should be continuously monitored and reported on the LDC screen.
//Updates should occur once per minute.
//– System should respond to changes in vent position control
//– Stop button should turn fan motor off (if on) and system should go to DISABLED state


