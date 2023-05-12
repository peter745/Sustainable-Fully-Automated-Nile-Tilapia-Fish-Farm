//#include <OneWire. h>
#include <DallasTemperature.h>
#define oneWireBus 7
#define heater 8
#define aerator 9
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define sensorOut 6
const int pumb1 = 10;
const int pumb2 = 11;
int t1=0,t2=0,t3=0,t4=0,t5=0,t6=0;
int frequency = 0;
void setup() {
  Serial.begin(9600);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(pumb1, OUTPUT);
  pinMode(pumb2, OUTPUT);
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  digitalWrite(pumb1,HIGH);
  digitalWrite(pumb2,HIGH);
  sensors.begin();
  pinMode(heater, OUTPUT);
  pinMode(aerator, OUTPUT);
  digitalWrite(heater, HIGH);
  digitalWrite(aerator, HIGH);
}

void loop() {
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  frequency = pulseIn(sensorOut, LOW);
  int red = map(frequency, 94, 171, 255, 0);
  Serial.print("R= ");
  Serial.print(red);
  Serial.print("  ");
  delay(100);
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  frequency = pulseIn(sensorOut, LOW);
  int green = map(frequency, 130, 214, 255, 0);
  Serial.print("G= ");
  Serial.print(green);
  Serial.print("  ");
  delay(100);
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  frequency = pulseIn(sensorOut, LOW);
  int blue = map(frequency, 117, 187, 255, 0);
  Serial.print("B= ");
  Serial.print(blue);
  Serial.println("  ");
  delay(100);
  if(green-red>50&&green-blue>50){
    digitalWrite(aerator, LOW);
    Serial.print("Green detected!");
    t1 = (t1==0? millis(): t1);
    int c = analogRead(A0);
    while(c >= 400){
      digitalWrite(pumb1, LOW);
      delay(5000);
      digitalWrite(pumb1, HIGH);
      delay(5000);
      c = analogRead(A0);
    }
    c = analogRead(A0);
    while(c <= 600){
      digitalWrite(pumb2, LOW);
      delay(5000);
      digitalWrite(pumb2, HIGH);
      delay(5000);
      c = analogRead(A0);
    }
  } else {
    digitalWrite(aerator, HIGH);
    if(t1 != 0){
      t2=millis();
      Serial.print("time taken for the green color to be removed: "); Serial.println(t2-t1);
      t1=0,t2=0;  
    }
  }
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print("Temperature in C: ");
  Serial.print(temperatureC);
  float DO_22 = 8.9;
  float DO_final = DO_22+((22-temperatureC)*1.5/5.0);
  Serial.print(" Dissolved oxygen in mg/L: ");
  Serial.println(DO_final); 
  if(temperatureC < 22){
    digitalWrite(heater,LOW);
    t3 = (t3==0? millis(): t3);
    Serial.println("Low tempertaure detected: ");
  } else {
    digitalWrite(heater, HIGH);
    if(t3!=0){
      t4=millis();
      Serial.print("time taken for the water to be Heated: "); Serial.println(t4-t3);
      t3=0,t4=0;  
    }
  }
  if(temperatureC > 28){
   digitalWrite(aerator,LOW);
   t5 = (t5==0? millis(): t5);
   Serial.println("High tempertaure detected! ");
  } else {
   digitalWrite(aerator,HIGH);   
   if(t5!=0){
      t6=millis();
      Serial.print("time taken for the water to be : "); Serial.println(t6-t5);
      t5=0,t6=0;  
    }
  }
  delay(1000);
}
