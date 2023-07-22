#include <Adafruit_MPU6050.h>
#include <Adafruit_sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
float* b;
float c[2];

void mpu_setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);

  // initialize mpu
  if (!mpu.begin()) {
    Serial.println("Failed to start MPU6050");
  } 
  else {
    Serial.println("Start MPU6050");
  }

  // display MPU6050 settings
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

//c语言基础知识实在是不过关……help
// float* mpu_loop() {
//   // get new sensor events with the readings
//   sensors_event_t a, g, temp;
//   mpu.getEvent(&a, &g, &temp);
//   // c = {a.acceleration.y, g.gyro.x};
//   // b = &c[0];
//   // return b;
// }

