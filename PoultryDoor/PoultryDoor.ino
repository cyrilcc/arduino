#include <RTClib.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <LCDKeypad.h>


#define PIN_BACKLIGHT 10

#define PIN_RELAY1 12
#define PIN_RELAY2 13

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5


#define STATE_IDLE 0
#define STATE_HOME 1
#define STATE_MENU 2
#define STATE_MANUAL 3
#define STATE_CLOSE_TIME 4
#define STATE_OPEN_TIME 5
#define STATE_OPENING 11
#define STATE_CLOSING 12



#define IDLE_TIMEOUT 30000

int idle_timeout = IDLE_TIMEOUT;

int lcd_key     = 0;
int adc_key_in  = 0;
 


LCDKeypad lcd;
RTC_DS1307 rtc;
int buttonState = 0;
boolean open = true;
int currentState = STATE_HOME;
int currentMenuItem = 0;

void display_date() {

    lcd.setCursor(5,0);
    DateTime now = rtc.now(); 

    if ( now.day() < 10 ) {
      lcd.print("0");
    }
    lcd.print(now.day(), DEC);
    lcd.print('/');
    if ( now.month() < 10 ) {
      lcd.print("0");
    }
    lcd.print(now.month(), DEC);
    lcd.print(' ');
    if ( now.hour() < 10 ) {
      lcd.print("0");
    }
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    if ( now.minute() < 10 ) {
      lcd.print("0");
    }
    lcd.print(now.minute(), DEC);
}


int read_LCD_buttons(){               // read the buttons
    adc_key_in = analogRead(0);       // read the value from the sensor 
 
    // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
    // we add approx 50 to those values and check to see if we are close
    // We make this the 1st option for speed reasons since it will be the most likely result
 
    if (adc_key_in > 1000) return btnNONE; 
 
    // For V1.1 us this threshold
    if (adc_key_in < 50)   return btnRIGHT;  
    if (adc_key_in < 250)  return btnUP; 
    if (adc_key_in < 450)  return btnDOWN; 
    if (adc_key_in < 650)  return btnLEFT; 
    if (adc_key_in < 850)  return btnSELECT;  
 
   // For V1.0 comment the other threshold and use the one below:
   /*
     if (adc_key_in < 50)   return btnRIGHT;  
     if (adc_key_in < 195)  return btnUP; 
     if (adc_key_in < 380)  return btnDOWN; 
     if (adc_key_in < 555)  return btnLEFT; 
     if (adc_key_in < 790)  return btnSELECT;   
   */
 
    return btnNONE;                // when all others fail, return this.
}

void setup () {

  lcd.begin(16, 2);
  lcd.clear();

  pinMode(PIN_BACKLIGHT, OUTPUT);
  pinMode(PIN_RELAY1, OUTPUT);       
  pinMode(PIN_RELAY2, OUTPUT);
  
  digitalWrite(PIN_RELAY1,HIGH);
  digitalWrite(PIN_RELAY2,HIGH);
  digitalWrite(PIN_BACKLIGHT,HIGH);

  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }

}

void idle_state() {

  digitalWrite(PIN_BACKLIGHT,LOW);
  delay(1000);
  lcd_key = read_LCD_buttons();
  if ( lcd_key != btnNONE ) {
    idle_timeout = IDLE_TIMEOUT;
    currentState = STATE_HOME;
    lcd.clear();
  } 

}


void home_state() {

  digitalWrite(PIN_BACKLIGHT,HIGH);
  display_date();
 // lcd.setCursor(0,1);
 // lcd.print(idle_timeout/1000, DEC);
  delay(500);
  lcd_key = read_LCD_buttons();
  if ( lcd_key != btnNONE ) {
    idle_timeout = IDLE_TIMEOUT;
    currentState = STATE_MENU;
    currentMenuItem = 0;
    lcd.clear();
  } else {
    idle_timeout -= 500;
    if ( idle_timeout <= 0 ) {
      currentState = STATE_IDLE;
      lcd.clear();
    }
  }

}

void display_menu() {
  
    lcd.setCursor(0,0);
    lcd.print("MENU");
    lcd.setCursor(0,1);

    switch ( currentMenuItem )  {

     case 0 :
        lcd.print("> REGLER L'HORLOGE");
        break;

     case 1 :
        lcd.print("> HEURE OUVERTURE");
        break;   

     case 2 :
        lcd.print("> HEURE FERMETURE");
        break;   

     case 3 :
        lcd.print("> MODE MANUEL");
        break;   
    }

    
}

void menu_state() {

  digitalWrite(PIN_BACKLIGHT,HIGH);
  display_menu();
  
  delay(250);
  lcd_key = read_LCD_buttons();



  
  if ( lcd_key != btnNONE ) {

Serial.print("menu item ");
Serial.println(currentMenuItem);

Serial.print("key ");
Serial.println(lcd_key);

    
    idle_timeout = IDLE_TIMEOUT;
    currentState = STATE_MENU;

     if ( lcd_key == btnUP ) {
        if ( currentMenuItem > 0 ) { currentMenuItem--; } 
     } else if ( lcd_key == btnDOWN ) {
        if ( currentMenuItem < 3 ) { currentMenuItem++; } 
     } else if ( lcd_key == btnSELECT ) {
      
     }  
     lcd.clear();
    
  } else {
    idle_timeout -= 250;
    if ( idle_timeout <= 0 ) {
      idle_timeout = IDLE_TIMEOUT;
      currentState = STATE_HOME;
      lcd.clear();
    }
  }

}



void loop () {

  switch ( currentState ) {

  case STATE_IDLE :
    idle_state();
    break;

  case STATE_HOME :
    home_state();
    break;

  case STATE_MENU :
    menu_state();
    break;

  default:
    break;  
  }

  
}
