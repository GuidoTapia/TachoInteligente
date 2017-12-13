


void setup()
{
  Serial.begin(9600);
}

void loop()
{
  String sms ("{ \"US0\":\"12\", \"US1\":\"12\", \"US2\":\"15\",\"US3\":\"14 \" }") ;
  //Serial.println("SMS:");
  //Serial.println(sms);
  String val(66);
  String resMedicion("{");///
  for(int i=0;i<4;i++){
    resMedicion.concat("\"US");
      resMedicion.concat(i);
      resMedicion.concat("\":\"");
      resMedicion.concat(val);
      if(i<3)resMedicion.concat("\",");
  }
  resMedicion.concat("}");
  Serial.println("SMS:");
  Serial.println(resMedicion);
  delay(3000);
}
