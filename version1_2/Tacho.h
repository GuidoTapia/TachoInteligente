#include "US.h"
class Tacho{
  Ultrasonic ultrasonic[5];
  int valores[5];
public:
  Tacho()
  {
    ultrasonic[0]=Ultrasonic(3,2);
    ultrasonic[1]=Ultrasonic(5,4);
    ultrasonic[2]=Ultrasonic(7,6);
    ultrasonic[3]=Ultrasonic(9,8);
    ultrasonic[4]=Ultrasonic(12,13);
  }
  String medir() ///
  {
    String resMedicion("{");///
    int contador=0;
    Serial.println();
    for(int i=0;i<5;i++){
      valores[i]=0;
      int ival=0;
      int ifal=0;
      while(ival<3 && ifal<7){
        int aux=ultrasonic[i].Promedio();
        if(aux){
          valores[i]+=aux;
          ival++;
        }else{
          ifal++;
        }
      }
      if(ifal>=7){
        valores[i]=0;
      }else{
        valores[i]/=3;
      }
      //Serial.print(i+1); //only for debugging
      Serial.print(" - "); //only for debugging
      Serial.print(valores[i]); //only for debugging
    
      String val(valores[i]);
      resMedicion.concat("\"US");
      resMedicion.concat(i);
      resMedicion.concat("\":\"");
      resMedicion.concat(val);
      if(i<4)resMedicion.concat("\",");
      if(i==4)resMedicion.concat("\"");
    }
    //{ "US0":"12", "US1":"12", "US2":"15","US3":"14" } 
    Serial.println();
    resMedicion.concat("}");
    return resMedicion;///
  }
};

