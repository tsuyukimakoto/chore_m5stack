#define M5STACK_MPU6886 

#include <M5Stack.h>

float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

float maxAccX = 0.0F;
float maxAccY = 0.0F;
float maxAccZ = 0.0F;

float minAccX = 0.0F;
float minAccY = 0.0F;
float minAccZ = 0.0F;

float maxAccKMX = 0.0F;
float maxAccKMY = 0.0F;
float maxAccKMZ = 0.0F;

float minAccKMX = 0.0F;
float minAccKMY = 0.0F;
float minAccKMZ = 0.0F;

float oneGSec = 9.80665F; // 1g = 9.80665m/s^2
float km = oneGSec * 60.0F * 60.0F / 1000.0F;

void setup(){

  M5.begin();
  M5.Power.begin();
    
  M5.IMU.Init();

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(GREEN , BLACK);
  M5.Lcd.setTextSize(2);
}

void _reset() {
  maxAccX = 0.0F;
  maxAccY = 0.0F;
  maxAccZ = 0.0F;
  minAccX = 0.0F;
  minAccY = 0.0F;
  minAccZ = 0.0F;

  maxAccKMX = 0.0F;
  maxAccKMY = 0.0F;
  maxAccKMZ = 0.0F;
  minAccKMX = 0.0F;
  minAccKMY = 0.0F;
  minAccKMZ = 0.0F;
}

void loop() {
  M5.IMU.getAccelData(&accX,&accY,&accZ);

  accZ = accZ - 1; // Z axis gets 1g in nature.

  M5.update();
  if (M5.BtnB.wasReleased()) {
    _reset();
    delay(300); // avoid button press vibration
  }

  if(maxAccX < accX) {
    maxAccX = accX;
    maxAccKMX = maxAccX * km;
  }
  if(maxAccY < accY) {
    maxAccY = accY;
    maxAccKMY = maxAccY * km;
  }
  if(maxAccZ < accZ) {
    maxAccZ = accZ;
    maxAccKMZ = maxAccZ * km;
  }

  if(minAccX > accX) {
    minAccX = accX;
    minAccKMX = minAccX * km;
  }
  if(minAccY > accY) {
    minAccY = accY;
    minAccKMY = minAccY * km;
  }
  if(minAccZ > accZ) {
    minAccZ = accZ;
    minAccKMZ = minAccZ * km;
  }

  M5.Lcd.setCursor(0, 20);
  M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", accX, accY, accZ);
  M5.Lcd.setCursor(220, 42);
  M5.Lcd.print(" G");

  M5.Lcd.setCursor(0, 65);
  M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", maxAccX, maxAccY, maxAccZ);
  M5.Lcd.setCursor(220, 87);
  M5.Lcd.print(" G(Max)");

  M5.Lcd.setCursor(0, 110);
  M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", minAccX, minAccY, minAccZ);
  M5.Lcd.setCursor(220, 132);
  M5.Lcd.print(" G(Min)");

  M5.Lcd.setCursor(0, 155);
  M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", maxAccKMX, maxAccKMY, maxAccKMZ);
  M5.Lcd.setCursor(220, 177);
  M5.Lcd.print(" KM/h");

  M5.Lcd.setCursor(0, 200);
  M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", minAccKMX, minAccKMY, minAccKMZ);
  M5.Lcd.setCursor(220, 222);
  M5.Lcd.print(" KM/h");

  delay(1);
}