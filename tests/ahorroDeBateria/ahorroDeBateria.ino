#include <EEPROM.h>
#include "EEPROMAnything.h"

int indexEE;
int valueEeprom = 0;
const int potPin = A1;
int valuePot;
int led = 13;
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

void setup()
{
 pinMode(potPin,INPUT);     
 Serial.begin(9600);
}
void loop(){

  indexEE = 0;      //Lee los valores de la EEPROM 
  indexEE += EEPROM_readAnything(indexEE, valueEeprom);  
  valuePot = analogRead(potPin);
  //valuePot = 156;
  Serial.print("potValue: ");
  Serial.println(valuePot);  
  valuePot = potEstados(valuePot);
  Serial.print("potEstados: ");
  Serial.println(valuePot);  
  if (valueEeprom < valuePot)
    {
      indexEE = 0; 
      valueEeprom++;
      Serial.print("valor a escribir en EE ");
      Serial.println(valueEeprom);
      indexEE +=EEPROM_writeAnything(indexEE, valueEeprom);  //Guarda los valores en la EEPROM
      //mandar señal de apagar
    }
  else
    {
      //mandar señal de prender
      Serial.println(" > enter r2d2code");       
      valueEeprom=0;
      indexEE = 0;
      Serial.println("Reiniciar a 0");
      indexEE += EEPROM_writeAnything(indexEE, valueEeprom);      
    } 
    delay(3000);
}

