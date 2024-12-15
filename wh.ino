#define BLYNK_TEMPLATE_ID "TMPL36bW8HpeF"
#define BLYNK_TEMPLATE_NAME "wheel chair priya"
#define BLYNK_AUTH_TOKEN "lg3hMoV5g4JXAzkuytXr30sUTwoDTJgz"

#include "I2Cdev.h"
#include "MPU6050.h"
#include<SoftwareSerial.h>
#define in1 14
#define in2 27
#define in3 26
#define in4 25
SoftwareSerial gpss(5,18);
#include <TinyGPS++.h>
TinyGPSPlus gps;
/* MPU6050 default I2C address is 0x68*/
MPU6050 mpu;
#include <BlynkSimpleEsp32.h>
// Replace with your network credentials
const char* ssid     = "iotdata";
const char* password = "12345678";
#define OUTPUT_READABLE_ACCELGYRO
//#define OUTPUT_BINARY_ACCELGYRO
double lati,lngi;
String loca;
int16_t ax, ay, az;
int16_t gx, gy, gz;
bool blinkState;
int ff,but,bf,force;
BLYNK_WRITE(V0){
  int f=param.asInt();
  if(f==1){
    Serial.println("f");
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    digitalWrite(in3,HIGH);
    digitalWrite(in4,LOW);
    }
    else{
      digitalWrite(in1,LOW);
      digitalWrite(in2,LOW);
      digitalWrite(in3,LOW);
      digitalWrite(in4,LOW);
     }
  }
BLYNK_WRITE(V1){
  int b=param.asInt();
  if(b==1){
    Serial.println("b");
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
    }
    else{
      digitalWrite(in1,LOW);
      digitalWrite(in2,LOW);
      digitalWrite(in3,LOW);
      digitalWrite(in4,LOW);
     }
  }
BLYNK_WRITE(V2){
  int l=param.asInt();
  
  if(l==1){
    Serial.println("l");
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
    }
    else{
      digitalWrite(in1,LOW);
      digitalWrite(in2,LOW);
      digitalWrite(in3,LOW);
      digitalWrite(in4,LOW);
     }
     }
BLYNK_WRITE(V3){
  int r=param.asInt();
  
  if(r==1){
    Serial.println("r");
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    digitalWrite(in3,HIGH);
    digitalWrite(in4,LOW);
    }
    else{
      digitalWrite(in1,LOW);
      digitalWrite(in2,LOW);
      digitalWrite(in3,LOW);
      digitalWrite(in4,LOW);
     }}
  
void setup() {
  
  Serial.begin(9600);
  Serial2.begin(9600);
  gpss.begin(9600);
  pinMode(4,INPUT_PULLUP);
  pinMode(33,INPUT);
  pinMode(14,OUTPUT);
  pinMode(27,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(25,OUTPUT);
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
  
  Blynk.begin(BLYNK_AUTH_TOKEN,ssid,password);
   #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin(); 
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif
  mpu.initialize();
  Serial.println("Updating internal sensor offsets...\n");
  mpu.setXAccelOffset(0); //Set your accelerometer offset for axis X
  mpu.setYAccelOffset(0); //Set your accelerometer offset for axis Y
  mpu.setZAccelOffset(0); //Set your accelerometer offset for axis Z
  mpu.setXGyroOffset(0);  //Set your gyro offset for axis X
  mpu.setYGyroOffset(0);  //Set your gyro offset for axis Y
  mpu.setZGyroOffset(0);  //Set your gyro offset for axis Z
}

void loop() {
  Blynk.run();
  while (gpss.available() > 0)
    if (gps.encode(gpss.read()))
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while (true);
  }
  Serial.print("LAT = ");
  Serial.println(gps.location.lat(),6);
  Serial.print("Long = ");
  Serial.println(gps.location.lng(),6);
  lati=gps.location.lat(),6;
  lngi=gps.location.lng(),6;
  loca=String(lati)+","+String(lngi);
  Serial.print("Loca = ");
  Serial.println(loca);
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  but=digitalRead(4);
  Serial.print("BUT = ");
  Serial.println(but);
  force=analogRead(33);
  Serial.print("Force = ");
  Serial.println(force);
  if(force>=2500){
    Blynk.virtualWrite(V5,1);
    }
    else{
     Blynk.virtualWrite(V5,0);
     }
  if((but==0)&&(bf==0)){
    Blynk.logEvent("emerg","Emergency Alert");
    Serial.println("Emergency Alert");
    bf=1;
    }
   if((but==1)&&(bf==1)){
    bf=0;
    }
  if(((ax<28000)||(ay<-12000)||(ay>1000))&&(ff==0)){
    Blynk.logEvent("flip","wheel chair met with accident");
    Serial.println("Flip alert");
    ff=1;
    }
   if(((ax>28000)||(ay>-12000)||(ay<1000))&&(ff==1)){
    ff=0;
    }
    Serial.print("a/g:\t");
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.println("\t");
    Blynk.virtualWrite(V4,loca);
}
