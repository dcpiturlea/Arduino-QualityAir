

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 
byte ledControl = 6;
byte ledAnomalie = 7;
byte senzorTemperatura = 9;
byte pinSenzorAer = 0;
DHT dht(senzorTemperatura, DHT22);

void setup() {
//alegem parametrii pentru led, ledControl este pinul de pe placa, iar OUTPUT spune ca prin acest pin iese ceva(curent)):
  pinMode(ledControl, OUTPUT);
  pinMode(ledAnomalie, OUTPUT);
  
//setam senzorul de temperatura, alegem pinul, apoi modelul senzorului, ii spunem sa porneasca:
  dht.begin();
  Serial.begin(115200);

//setam parametrii pentru ecran
  lcd.begin(16,2);
  lcd.backlight();
}
int senzorAer;
float umiditate;
float temperatura;
String umid;
String temp;
String nivelAer;\
String calitateAer;
  
void loop() {
  CitireParametri();
  
//Daca parametri sunt depasiti sau sub nivel afisam anumite erori pe ecran si aprindem/stingem ledurile
  if(temperatura >=28.0!=umiditate>=75.0!=senzorAer>=160){
    while(temperatura >=28.0!=umiditate>=75.0!=senzorAer>=160){
     AfisareParametriiDepasiti();
     ParametriiLedAndSoundError();
     AfisareParametri();
     CitireParametri();
    
  }
      
  }
  else if(temperatura <17.0!=umiditate<30.0){
    while(temperatura <17.0!=umiditate<30.0){
     AfisareParametriiSubNivel();
     ParametriiLedAndSoundError();
     AfisareParametri();
     CitireParametri();
     
  } 
  }
  
  else{
 digitalWrite(ledAnomalie, LOW);
 digitalWrite(ledControl, HIGH); // aceasta metoda cu doi parametri ii spune variabilei ledControl,//care este un pin de pe microcontroler sa fie alimentat cu curent prin cuvantul HIGH
 noTone(8);
 CitireParametri();
 AfisareParametri();
 
    }

}

void CitireParametri(){
//Creem variabilele pentru senzor:
  senzorAer=analogRead(pinSenzorAer); //citim valorea senzorului detector de aer si o stocam inbtr-o variabila int
  umiditate = dht.readHumidity();  // aceasta variabila float stocheaza, dupa citire, umiditatea
  temperatura = dht.readTemperature(); // aceasta variabila float stocheaza, dupa citire, temperatura
  
   //transformam rezultatele variabilelor umiditate si temperatura, care sunt float, in string, pentru a putea fi citite mai apoi
  umid = String(umiditate);
  temp = String(temperatura);
  nivelAer = String(senzorAer);
}

void AfisareParametri(){
  
  //in functie de punctajul obtinut de senzorul de aer stocam un literal diferit intr-un String:
calitateAer = "Eror";
if(senzorAer>0&&senzorAer<50){
   calitateAer = "V. Good Quality";
}
if(senzorAer>=50&&senzorAer<100){
   calitateAer = "Good Quality";
}
if(senzorAer>=100&&senzorAer<150){
   calitateAer = "Limited Quality";
}
if(senzorAer>=150&&senzorAer<200){
   calitateAer = "Poor Quality";
   
}
if(senzorAer>200){
   calitateAer = "V. Poor Quality";
   
}
  lcd.clear();//resetam ecranul, sa nu mai apara nimic pe el
  lcd.setCursor(0,0); //setam cursorul pe prima linia a ecranului
  lcd.print("Umid: "+umid+" %");
  lcd.setCursor(0,1);//setam cursorul pe a doua linie a ecranului
  lcd.print("Temp: " +temp+" C");
  delay(2500);
  lcd.clear();//resetam ecranul, sa nu mai apara nimic pe el
  lcd.setCursor(0,0);
  lcd.print("Air Quality: " +nivelAer);
  lcd.setCursor(0,1);//setam cursorul pe a doua linie a ecranului
  lcd.print(calitateAer);
  delay(2500);
}

void ParametriiLedAndSoundError(){
  digitalWrite(ledControl, LOW);
  digitalWrite(ledAnomalie, HIGH);
  for(int i=700;i<800;i++){
  tone(8,i);
  delay(15);
  }
  for(int i=800;i>700;i--){
  tone(8,i);
  delay(15);
  }
}

void AfisareParametriiDepasiti(){
  lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("    Exceed");
     lcd.setCursor(0,1);
     lcd.print("  Parameters!");
     delay(2000);
}

void AfisareParametriiSubNivel(){
  digitalWrite(ledControl, LOW);
     digitalWrite(ledAnomalie, HIGH);
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("      LOW");
     lcd.setCursor(0,1);
     lcd.print("  Parameters");
     delay(2000);
}
