
#include <SPI.h>                                  //Add 'SPI' library
#include <RFID.h>                                 //Add 'RFID' library
#include <Servo.h>                                //Add 'Servo' library
#include "pitches.h"                              //Add 'pitches' header file
#include<LiquidCrystal.h>                         //Add 'LiquidCrystal' library
#include <SoftwareSerial.h>
SoftwareSerial nodemcu(0,1);



LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);                   // 'lcd' is declared with the pin configuration
RFID rfid(10,9);                                            // RFID definition
constexpr uint8_t greenLed = 4;
constexpr uint8_t redLed = 6;
long duration,distance,slot1,slot2;
String sensor1;
String sensor2;
String cdata =""; // complete data, consisting of sensors values
#define TRIGPIN1 7
#define ECHOPIN1 8

#define TRIGPIN2 2
#define ECHOPIN2 3

#define buzzerPin 5

                                                  
byte USER[4][5] = {                               // Enter the 'USER' variable with Serial No of RFID tags 
  {84,127,79,211,183},
  {192,33,134,37,66},
  {142,40,84,197,55},
  {181,9,162,67,93}
}; 
                                                  
byte data[5];                                     // Put here the another allowed cards
boolean USER_card[4]={false,false,false,false};   // To check the state of RFID cards
unsigned long timer1[4];                          // stores the time at the entry time
unsigned long timer2[4];                          // stores the time at the exit time
unsigned long tTime=0;                            // set 'tTime' to 0
int price=0;                                      // variable to display the price
Servo myServo;                                    // variable for servo motor

void setup(){ 

  Serial.begin(9600);                             // Initialize Serial Communication at 9600 baudrate
  nodemcu.begin(9600);
  lcd.begin(16,2);                                // Number of Rows, Columns in the LCD Screen
  lcd.print("Please Tag");                        // Displays 'Please Tag'on the screen
  //myServo.attach(8);                              // Assign pin 9 to servo
  
pinMode(redLed, OUTPUT);
pinMode(greenLed, OUTPUT);
pinMode(buzzerPin, OUTPUT);
pinMode(TRIGPIN1, OUTPUT);
pinMode(ECHOPIN1, INPUT);
pinMode(TRIGPIN2, OUTPUT);
pinMode(ECHOPIN2, INPUT);
  SPI.begin();                                    // SPI communication initialization
  rfid.init();                                   // RFID module initialization  
}
  void ultra(int trig, int echo){
    digitalWrite(trig,LOW);
    delayMicroseconds(2);
    digitalWrite(trig,HIGH);
    delay(10);
    digitalWrite(trig,LOW);
    duration=pulseIn(echo,HIGH);
    distance = (duration/2) / 29.1;
  }

void loop(){
                                                  // Here we create a variable for each user
  if (rfid.isCard()){                             // valid card found
    if (rfid.readCardSerial()){                   // reads the card 
      data[0] = rfid.serNum[0];                   // stores the serial number
      data[1] = rfid.serNum[1];
      data[2] = rfid.serNum[2];
      data[3] = rfid.serNum[3];
      data[4] = rfid.serNum[4];
    }
                                          
    for(int i=0; i<5; i++){                       // the for loop prints the tagged RFID card's Serial No.
      Serial.print(data[i]);
      Serial.print(",");
    }
    
    Serial.println();
                                                  // Loop to identify the parking spot by reading the RFID tag
    for(int i=0; i<4;i++){                        // Checks each row 
      for(int j=0; j<5;j++){                      // Checks each element                                                
        if(data[j]==USER[i][j]){                  // Checks if tagged card is there in predefined list
          if(USER_card[i]){                       // Check if the user has already entered the parking space
            timer2[i]=millis();                   // set 'timer2' with the current time
            tTime=timer2[i]-timer1[i];            // total time is calculated
            tTime=tTime/1000;                     // time is then converted to seconds
            price=tTime*0.5;                      // price calculated at $ 0.5 per second
            
            
                                                  // Display on lcd.
            lcd.clear();                          // clears the LCD screen and sets cursor to (0,0)
            lcd.print("Time: ");                  // displays 'Time:' on the LCD
            lcd.print(tTime);                     // displays the total time on LCD
            lcd.setCursor(0,1);                   // sets cursor to column 0 and row 1
            lcd.print("Price:$");                // displays 'Price:$' on the LCD
            lcd.print(price);                     //  displays the price on the LCD
            delay(2000);                          // The text will be shown on LCD for 2 seconds
            lcd.clear();                          // clears the lcd and sets cursor to (0,0)
            lcd.print("Thanks!");                 // displays 'Thanks!' on the LCD
            lcd.setCursor(0,1);                   // sets cursor on (0,1)
            lcd.print("Visit Us Again!");         // displays 'Visit Us Again!' on LCD
            USER_card[i]=false;                   // set user entry as false
            myServo.write(90);                    // rotate servo to 90 degrees
            delay(1000);                          // wait for 1 second
            myServo.write(0);                     // rotates servo to 0 degree
            delay(1000);                          // wait for 1 second
            break;                                // breaks from the for loop
          }
          else{                                   // when user enters the parking space
            digitalWrite(greenLed, HIGH);
            tone(buzzerPin, 500);
            delay(300);
            digitalWrite(greenLed, LOW);
            noTone(buzzerPin);
            delay(100);
            timer1[i]=millis();                   // set timer1 with the current time
            lcd.clear();                          // clears lcd and sets the cursor to (0,0)
            lcd.print("Welcome!");                // prints "Welcome!" on LCD
            lcd.setCursor(0,1);                   // sets cursor to column on 0 and row 1
            lcd.print("Proceed to Slot");            // displays 'Proceed to P' on LCD
            lcd.print(i+1);                       // displays the parking location
            myServo.write(90);                    //  rotates the servo to 90 degrees
            delay(1000);                          // waits for 1 second
            myServo.write(0);                     // rotates servo to 0 degree
            delay(1000);                          // waits for 1 second
            USER_card[i]=true;                    // sets user entry as true
            break;                                // breaks from for loop
          }
        }
      }
    }
  lcd.clear();                                    // clears lcd
  lcd.print("Please Tag");                        // display 'Please Tag'
  delay(1000);                                    // wait for 1 second 
  }
  pSlot1();
  pSlot2();
  delay(1000);                                    // wait for 1 second 
  cdata = cdata + sensor1 +"," + sensor2 + ","; // comma will be used a delimeter
   Serial.println(cdata); 
   nodemcu.println(cdata);
   delay(6000); // 100 milli seconds
   cdata = ""; 
  ultra(TRIGPIN1,ECHOPIN1);
  slot1=distance;
  ultra(TRIGPIN2,ECHOPIN2);
  slot2=distance;
}
  void pSlot1() // parkng  slot1
{
  if( slot1 <= 15){
  sensor1 = "255";
 delay(200);
 //Serial.println("slot1 if booked");
 }
else{
  sensor1 = "0"; 
 delay(200); 
 //Serial.println("slot1 is free");
}
}
void pSlot2() // parking slot2
{
  if( slot2 <= 15){
  sensor2 = "255";
 //Serial.println("slot2 is booked");
 delay(200);
  }else{
  sensor2 = "0"; 
  //Serial.println("slot2 is free");
 delay(100);
  } 
}
