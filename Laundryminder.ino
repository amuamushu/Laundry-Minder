#include <Blynk.h>

#include <BlynkSimpleEsp8266.h>
#include <Wire.h> // I2C library for GY-521

char auth[] = "3DJG2r8eiFA2VECT62Jrqutxbx9dEXvd";
char ssid[] = "ATT33vtm76";
char pass[] = "4z3xx5436qij";
BlynkTimer timer;

const int MPU_addr=0x68; // GY-521 I2C address
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ; // GY-521
int minVal=265; // GY-521
int maxVal=300; // GY-521

int x = 360; // GY-521
int xx = 360; // GY-521 test if movement stopped
int xMax = 0;
int xMin = 360;

long previousMillis = 0;  // will store last time data was updated
long interval = 10000; // wait 10 secs before sending push
//long interval = 300000; // wait 5 mins before sending push

void sendUptime()
{
  Blynk.virtualWrite(10, xx); //virtual pin
  Blynk.virtualWrite(11, xMax); //virtual pin
  Blynk.virtualWrite(12, xMin); //virtual pin
}

void setup()
{
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendUptime);
}

void loop()
{
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
    int xAng = map(AcX,minVal,maxVal,-90,90);
    int yAng = map(AcY,minVal,maxVal,-90,90);
    int zAng = map(AcZ,minVal,maxVal,-90,90);

       xx = x;
       x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
       //y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
       //z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
/*     
  Serial.print("AngleX= ");
  Serial.println(x);
  Serial.print("VariableXX= "); // test variable for movement
  Serial.println(xx);
  Serial.println("-----------------------------------------");
*/
  if (x > xMax) {xMax = x;}
  if (x < xMin) {xMin = x;}
  
  Blynk.run();
  timer.run();
  
 unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
  if (xMax-xMin <= 2) // the min value of movement at washing machine at rest
  {
  //Serial.println("Washing is Done!");
  Blynk.notify("Washing is done!");
  xx=x;
  }
  xMax = 0;
  xMin = 360;
  }
} 
 
