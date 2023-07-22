#include "move.h"
#include "speed.h"
#include "mpu6050.h"
#include "helper.h"
#include "pid.h"
int pwm_a = 100, pwm_b = 100;
PID speedPID;
float reference_speed = 40;
void setup() {
  // put your setup code here, to run once:

  move_setup();
  speed_setup(10);  //设置定时器时长（需要与后面的保持一致）
  Serial.begin(115200);
  float kp, ki, kd, maxI, maxOut;
  kp = 5;
  ki = 0;
  kd = 0;
  maxI = 10;
  maxOut = 255;
  setupPID(&speedPID, kp, ki, kd, maxI, maxOut);  //void setupPID(PID *pid, float p, float i, float d, float maxI, float maxOut) {
  helper(0x01, 0x0b, 0x04, 0x00, 0x00, 0x00);  //pid调试助手启动
  
  //helper(0x02,0x0b,0x04,0x00,0x00,0x00);
}

void loop() {
  // put your main code here, to run repeatedly:
  move_loop(1, pwm_a, pwm_b);
  PID_CalcIn(&speedPID, reference_speed, v_a);  // PID_CalcIn(PID *pid, float reference, float feedback)
                                                // pwm_a=int(speedPID.output);
  PID_CalcIn(&speedPID, reference_speed, v_b);
  //pwm_b=int(speedPID.output);
  // vi = speed_loop();
  // Serial.println(vi);
  // delay(10);
  //

  //Serial.print(v_a);
  //Serial.println(v_b);
  helper(0x01, 0x0f, 0x02, uint8_t(v_a), 0x00, 0x00);  //void helper(uint8_t channel, uint8_t l, uint8_t flag, uint8_t i1, uint8_t i2, uint8_t i3)
  //helper(0x02, 0x0f, 0x02, uint8_t(v_b), 0x00, 0x00);

  delay(10);
}
