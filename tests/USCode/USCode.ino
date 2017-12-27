#include "Arduino.h"

class Ultrasonic{
  public:
    Ultrasonic(int TP, int EP);
    Ultrasonic();
    long Timing();
    long Ranging();
    void swap( int * e1, int *  e2 );
    void sort( int arr[], int n);
    int Promedio();
    
  private:
    int Trig_pin;
    int Echo_pin;
    long Time_out;
    long duration,distance_cm,distance_inc;
};

Ultrasonic::Ultrasonic(){
   Trig_pin=2;
   Echo_pin=3;
   Time_out=6000;
}

Ultrasonic::Ultrasonic(int TP, int EP){
   pinMode(TP,OUTPUT);
   pinMode(EP,INPUT);
   Trig_pin=TP;
   Echo_pin=EP;
   Time_out=6000;  // 3000 µs = 50cm // 30000 µs = 5 m 
}

long Ultrasonic::Timing(){
  digitalWrite(Trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig_pin, LOW);
  duration = pulseIn(Echo_pin,HIGH,Time_out);
  if ( duration == 0 ) {
  duration = Time_out; }
  return duration;
}

long Ultrasonic::Ranging()
{
  Timing();
  distance_cm = duration /29 / 2 ;
  return distance_cm;
}

void Ultrasonic:: swap( int * e1, int *  e2 )
{
	int temp = *e1;
	*e1 = *e2;
	*e2 = temp;
}

void Ultrasonic:: sort(int arr[], int n) // de ser relevante optimizar
{
   int i, j;
   for (i = 0; i < n-1; i++)      
       for (j = 0; j < n-i-1; j++) 
           if (arr[j] > arr[j+1])
              swap(&arr[j], &arr[j+1]);
}

int Ultrasonic::Promedio()
{
  int i=0;
  int ifall=0;
  int aux;
  int promedio=0;
  int valores[10];
  while(i<10){
    //solo medir en caso de estar cerrada la tapa
    aux=Ranging();
    delay(5);
    if (aux!=103){ // valor de rango maximo
      valores[i]=aux;
      i++;
      ifall--;
    }else{
      if(ifall>10){
        return 0;
      }
      ifall++;
    }
  }
  aux=0;
  
  //ordenar y eliminar los menores y los mayores
  sort(valores,10);
  for(i=2;i<8;i++){  
    aux+=valores[i];
  }
  promedio = aux/6;
  
  return promedio;
}



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
    ultrasonic[4]=Ultrasonic(13,12);
  }
  void medir()
  {
    int contador=0;
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
    }
    Serial.println();
  }
  int medicion()
  {
    int Resultado=0;
    int cont=0;
    for(int mR=0;mR<2;mR++){
      medir();
      for(int i=0;i<5;i++){
        if(valores[i]){
          Resultado+=valores[i];
          cont++;
        }
      }
    }
    return Resultado/cont;
  }
};

//Ultrasonic ultrasonic(2,3); // (Trig PIN,Echo PIN)
Tacho r2d2=Tacho();

void setup() {
  Serial.begin(9600); 
}

void loop(){
  //Serial.println(ultrasonic.Promedio());
  Serial.println("Resultado de la medicion:");
  Serial.println(r2d2.medicion());
  delay(100);
}
