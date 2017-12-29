class Ultrasonic{
  public:
    Ultrasonic(int EP, int TP);
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

