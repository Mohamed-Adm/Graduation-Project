
#define BLYNK_PRINT Serial

#define TRIGGER 4
#define ECHO    5

#define TRIGPIN2 13
#define ECHOPIN2 15

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include <SimpleTimer.h>

char auth[] = " pKsfpdQNmictHTx45_PhHx_ISfTNiVb-";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "doctor nourdine";
char pass[] = "mhandani269";

const int output14 = 14;
const int output12 = 12;
SimpleTimer timer;

String myString; // complete message from arduino, which consistors of snesors data
char rdata; // received charactors

int led1,led2;
// This function sends Arduino's up time every second to Virtual Pin (1).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // we can send any value at any time.
  // we don't send more that 10 values per second.
  Blynk.virtualWrite(V1, millis() / 1000);
 
}
void setup()
{
  // Debug console
  Serial.begin(9600);
 Blynk.begin(auth, ssid, pass);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(TRIGPIN2, OUTPUT);
  pinMode(ECHOPIN2, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);

   // Initialize the output variables as outputs
  pinMode(output14, OUTPUT);
  pinMode(output12, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output14, LOW);
  digitalWrite(output12, LOW);

    timer.setInterval(1000L,sensorvalue1);
    timer.setInterval(1000L,sensorvalue2);

}
void loop()
{

   if (Serial.available() == 0 )
   {
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
   }
  
  if (Serial.available() > 0 )
  {
    rdata = Serial.read();
    myString = myString+ rdata;
   //Serial.print(rdata);
    if( rdata == '\n')
    {
     Serial.println(myString);
  // Serial.println("fahad");
// new code
String l = getValue(myString, ',', 0);
String m = getValue(myString, ',', 1);

// these leds represents the leds used in blynk application
led1 = l.toInt();
led2 = m.toInt();

  myString = "";
// end new code
    }
  }
//led_control();
}

void sensorvalue1()
{
int sdata = led1;
  // we can send any value at any time.
  // we don't send more that 10 values per second.
  Blynk.virtualWrite(V10, sdata);
  slot1();
  
}
void sensorvalue2()
{
int sdata = led2;
  // we can send any value at any time.
  // we don't send more that 10 values per second.
  Blynk.virtualWrite(V11, sdata);
  slot2();

}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
       return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void slot1() {
  
  long duration, distance;
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1; 
  switch(distance){
    case 15:
    Blynk.virtualWrite(V2, 255);
    digitalWrite(output14, HIGH);
    delay(300);
    break;
    case 14:
    Blynk.virtualWrite(V2, 255);
    digitalWrite(output14, HIGH);
    delay(300);
    break;
    case 13:
    Blynk.virtualWrite(V2, 255);
    digitalWrite(output14, HIGH);
    delay(300);
    break;
    case 12:
    Blynk.virtualWrite(V2, 255);
    digitalWrite(output14, HIGH);
    delay(300);
    break;
    case 11:
    Blynk.virtualWrite(V2, 255);
    digitalWrite(output14, HIGH);
    delay(300);
    break;
    default:
    Blynk.virtualWrite(V2, 0);
    digitalWrite(output14, LOW);
    delay(100);
    
  }
   if (distance <= 10) {
    Blynk.virtualWrite(V5, 255);
}
  else {
    Blynk.virtualWrite(V5, 0);
  }
   //Serial.print(distance);
  //Serial.println("Centimeter:");
  Blynk.virtualWrite(V6, distance);
  delay(200);
  Blynk.run();
}
void slot2() {
  
  long duration, distance;
  digitalWrite(TRIGPIN2, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(TRIGPIN2, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGPIN2, LOW);
  duration = pulseIn(ECHOPIN2, HIGH);
  distance = (duration/2) / 29.1;
  switch(distance){
    case 15:
    Blynk.virtualWrite(V3, 255);
    digitalWrite(output12, HIGH);
    delay(300);
    break;
    case 14:
    Blynk.virtualWrite(V3, 255);
    digitalWrite(output12, HIGH);
    delay(300);
    break;
    case 13:
    Blynk.virtualWrite(V3, 255);
    digitalWrite(output12, HIGH);
    delay(300);
    break;
    case 12:
    Blynk.virtualWrite(V3, 255);
    digitalWrite(output12, HIGH);
    delay(300);
    break;
    case 11:
    Blynk.virtualWrite(V3, 255);
    digitalWrite(output12, HIGH);
    delay(300);
    break;
    default:
    Blynk.virtualWrite(V3, 0);
    digitalWrite(output12, LOW);
    delay(100);
    
  }
   if (distance <= 10) {
    Blynk.virtualWrite(V4, 255);
}
  else {
    Blynk.virtualWrite(V4, 0);
  }
  Blynk.virtualWrite(V7, distance);
  delay(200);
  Blynk.run();
}
