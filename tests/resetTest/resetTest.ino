#include "Arduino.h"
#include "LowPow.h"


const int buttonStartPin = A3;
const int ledPin = A2;
int buttonStartState = 0;      
boolean flagTakeAction = false;

void setup()
{   
 Serial.begin(9600); 
 pinMode(ledPin, OUTPUT);
 pinMode(buttonStartPin,INPUT);
 pinMode(A5, OUTPUT);
 digitalWrite(A5,HIGH);
 Serial.println("Start r2d2 program");
 delay(5000);
}

void loop(){
    Serial.println(">r2d2 code");
    
    while(!flagTakeAction){
      buttonStartState = digitalRead(buttonStartPin);
      
      if(buttonStartState == HIGH){
        flagTakeAction = true;
      }
      else
      {
        Serial.print("-");
      }    
    }   
    delay(5000);
    digitalWrite(ledPin, LOW);
    //for (int i = 0 ;  i  <  60 ; i++) // 1 hora - 60 min
      //for (int i = 0 ;  i  <  15 ; i++) // 1 minuto - 60 seg
        LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
    
    digitalWrite(ledPin, HIGH);
    delay(500); 
    digitalWrite(ledPin, LOW);
    delay(500); 
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(A5, LOW);
}
