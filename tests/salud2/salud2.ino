#include <SoftwareSerial.h>
SoftwareSerial GSMSerial(11,10); 
int contador = 0;


/* typeOfMessage could be ...
0: regular
1: alert
2: firstTime
*/
void mandar_SMS(String mensaje, int typeOfMessage){
  Serial.println("Enviando SMS...");
  GSMSerial.print("AT+CMGF=1\r"); //Comando AT para mandar un SMS
  delay(1000);
  GSMSerial.println("AT+CMGS=\"+51943415889\"\r"); //Numero al que vamos a enviar el mensaje
  delay(1000);
  mensaje.concat('(');
  mensaje.concat(typeOfMessage);
  mensaje.concat(')');
  GSMSerial.println(mensaje);
  delay(100);
  GSMSerial.println((char)26);//Comando de finalizacion ^Z
  delay(100);
  GSMSerial.println();
  delay(5000); // Esperamos un tiempo para que envíe el SMS
  Serial.println("SMS enviado");
}



void setup(){
Serial.begin(9600);
   GSMSerial.begin(9600);
   delay(5000);
   Serial.println("Start");
}

void loop(){
if(contador == 0){
  //{ \"US0\":\"12", "US1":"12", "US2":"15","US3":"14" }
  String stringOne = "SMS(HELLO WORLD)-->"; 
  mandar_SMS(stringOne,2);
  contador = 1;
   }
}
