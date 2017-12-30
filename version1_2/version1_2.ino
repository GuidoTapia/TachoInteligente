#include <EEPROM.h>
#include "EEPROMAnything.h"
#include "Arduino.h"
#include "Tacho.h"
#include <SoftwareSerial.h>
SoftwareSerial GSMSerial(11,10); 


const int lightSensorPin = A0;
const int buttonStartPin = A3;
const int potPin = A1;
const int ledPin = A2;

int buttonStartState = 0;      

boolean  flagfirstMedicion = false;
boolean flagSmsSend = false;
boolean flagTakeAction = false;

Tacho r2d2=Tacho();
boolean isOpen = false;
int contadorIsOpen = 0;
int alertIsOpen = 1; //minutos 
int indexEE;
int eepromValue = 0;
int potValue;
int lightSensorValue;
const int limitsPot[5]={0,255,512,767,1023}; // 1,2,3,4

int potEstados(int input)
{
  int res =0;
  for(int i = 1 ;i<5;i++)
  {
      if(input >= limitsPot[i-1] && input <= limitsPot[i])
      {
        res = i*4;
      }
  }
  return res;
}

/* typeOfMessage could be ...
0: regular
1: alert
2: firstTime
*/
void mandar_SMS(String mensaje, int typeOfMessage){
  Serial.println("Enviando SMS...");
  GSMSerial.print("AT+CMGF=1\r"); //Comando AT para mandar un SMS
  delay(1000);
  GSMSerial.println("AT+CMGS=\"+51992547553\"\r"); //Numero al que vamos a enviar el mensaje
  //guido:992547553
  delay(1000);
  mensaje.concat(" (");
  mensaje.concat(typeOfMessage);
  mensaje.concat(")");
  GSMSerial.println(mensaje);
  delay(100);
  GSMSerial.println((char)26);//Comando de finalizacion ^Z
  delay(100);
  GSMSerial.println();
  delay(5000); // Esperamos un tiempo para que envíe el SMS
  Serial.println("SMS enviado");
}

void blinkLedPin(int times, int duration){
  for(int i = 0;i < times; i++){
     digitalWrite(ledPin, HIGH);
     delay(duration); 
     digitalWrite(ledPin, LOW);
     delay(duration); 
     
  }
}

void ledFade(boolean isInicializated){
  if(!isInicializated){
    blinkLedPin(3, 500);
  }
  else
  {
    digitalWrite(ledPin,HIGH);
  }
  return;
}

void llamada(){
delay(5000);
  //GSMSerial.print("ATD+51943415889;\r");
  GSMSerial.print("ATD+51992547553;\r");
Serial.println("llamando");
delay(20000);
Serial.println("finalizar llamada");
GSMSerial.print("ATH\r");
}



void setup()
{   
 Serial.begin(9600); 
 GSMSerial.begin(9600);
 delay(5000);
 pinMode(buttonStartPin,INPUT);     
 pinMode(potPin,INPUT); 
 pinMode(lightSensorPin,INPUT);     
 pinMode(ledPin, OUTPUT);
 Serial.println("Start r2d2 program");
 delay(5000);
}

void loop(){  
  indexEE = 0;      //Lee los valores de la EEPROM 
  indexEE += EEPROM_readAnything(indexEE, eepromValue);  
  indexEE += EEPROM_readAnything(indexEE, flagfirstMedicion); 
  Serial.println("flagfirstMedicion: ");
  Serial.println(flagfirstMedicion); 
  potValue = analogRead(potPin);
  Serial.print("potValue: ");
  Serial.println(potValue);  
  potValue = potEstados(potValue);
  potValue = 0;
  Serial.print("potEstados: ");
  Serial.println(potValue);  
  if (eepromValue < potValue)
    {
      indexEE = 0; 
      eepromValue++;
      Serial.print("valor a escribir en EE ");
      Serial.println(eepromValue);
      blinkLedPin(eepromValue,500);
      indexEE +=EEPROM_writeAnything(indexEE, eepromValue);  //Guarda los valores en la EEPROM
      // solo para dejarlo seteado antes de poner en uso
      //flagfirstMedicion=0;
      indexEE +=EEPROM_writeAnything(indexEE, flagfirstMedicion);
    }
  else
    {
      Serial.println(" >> enter r2d2code"); 
      delay(3000);      
      Serial.print("out");
      while(!flagTakeAction)
      {
        Serial.print ("in ");
        Serial.println(buttonStartState);
        ledFade(flagfirstMedicion);
        buttonStartState = digitalRead(buttonStartPin); 
        if (buttonStartState == HIGH && flagfirstMedicion == false)
        {//buttonStartState is pressed (primera vez)
         String FisrtMedicion = r2d2.medir();
         mandar_SMS(FisrtMedicion,2);
         Serial.println("FisrtMedicion: (2) ");//debugging
         Serial.println(FisrtMedicion); //debugging
         flagfirstMedicion = true;
         flagTakeAction = true;
        }
        else
        { //funcionamiento normal
        if(flagfirstMedicion){
          lightSensorValue = analogRead(lightSensorPin);
          lightSensorValue= 50;
         Serial.print("lightSensorValue ");
         Serial.println(lightSensorValue);
         //lightSensorValue = 150;
         if(lightSensorValue<100)
          { //el tacho esta cerrado
           if(flagSmsSend == false)
            {
             String resMedicion = r2d2.medir();
             //mandar_SMS(resMedicion,0);
             llamada();
             Serial.println("resMedicion: (0) ");//debugging
             Serial.println(resMedicion);//debugging
             flagSmsSend = true;
             blinkLedPin(3,200);
             flagTakeAction = true;
            }  
           contadorIsOpen = 0;
          }
         else
          { //el tacho esta abierto
            Serial.println(contadorIsOpen); //debugging
            //el tacho esta abierto por mas del tiempo permitido
            if(contadorIsOpen == (alertIsOpen*60))
             { 
              String SAlertIsOpen="El tacho ha estdo abierto por mas de";
              SAlertIsOpen.concat(alertIsOpen);
              SAlertIsOpen.concat(" min");
              mandar_SMS(SAlertIsOpen,1);
              Serial.println(SAlertIsOpen);
              contadorIsOpen = 0;
              flagTakeAction = true;
             }
            delay(1000); // 1seg -- tiempo de espera para volver a revisar si ya cerraron el tacho
            contadorIsOpen++;           
          }
         } 
        }
      }
      eepromValue=0;
      indexEE = 0;
      Serial.println("Reiniciar a 0");
      indexEE += EEPROM_writeAnything(indexEE, eepromValue);      
      indexEE += EEPROM_writeAnything(indexEE, flagfirstMedicion);      
    } 
    blinkLedPin(7,250);
}
  
