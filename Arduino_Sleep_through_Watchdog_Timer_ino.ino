/*Document to be referred to for pages numbers 
Datasheet of ATMEGA 8 Microcontroller with 4/8/16K Flash Memory */

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#define LED_PIN (13)

void setup()
{
  Serial.begin(9600);
  pinMode(LED_PIN,OUTPUT);


 // ---------------- Setting up Watchdog Timer ----------------
  
  
  MCUSR &= ~(1<<WDRF);  // Clears the reset flag
  
  WDTCSR |= (1<<WDCE) | (1<<WDE); // WDCE enables Watchdog change, WDE enables Watchdog system reset (Pg.54)

  WDTCSR = 1<<WDP1 | 1<<WDP2; // Set Prescalar Value - 1 Second (Pg.55)
  
  WDTCSR |= 1<<WDIE; //Set WDIE to Interrupt (Pg.53 & 54)
  
 // ----------------------------------------------------------- 
  
}

void loop()
{
  digitalWrite(LED_PIN, HIGH);
  delay(5000);
  enterSleep();
}

void enterSleep(void)
{  
  digitalWrite(LED_PIN,LOW); //Switch off LED when asleep

  // -------------- Put to sleep ------------
     set_sleep_mode(SLEEP_MODE_PWR_DOWN);   
     sleep_enable();
     sleep_mode();
  //-----------------------------------------

  // ----- After Wake Up -------------
     sleep_disable(); // Disable sleep after wake up
     power_all_enable();
}

