 #include <SoftwareSerial.h>
/*
Conexion
UNO --> SIM800l
D7 --> TX
D8 --> RX
Reset --> RST
GND --> GND
*/
SoftwareSerial serialSIM800(11,10);// (RX,TX)

void setup() {
 Serial.begin(9600);
 serialSIM800.begin(9600);
 delay(3000);
 Serial.println("Inisialize SIM800L module");
 delay(1000);
 }

void loop() {
  if(serialSIM800.available()){
    Serial.write(serialSIM800.read());
  }
  
  if(Serial.available()){
    while(Serial.available()){
      serialSIM800.write(Serial.read());
    }
    serialSIM800.println();
  }
}


