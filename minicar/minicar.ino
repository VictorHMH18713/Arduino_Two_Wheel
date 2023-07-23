#include "move.h"
#include "speed.h"
#include "mpu6050.h"
#include "helper.h"
#include "pid.h"
int pwm_a = 100, pwm_b = 100;

PID speedPIDR;
PID speedPIDL;
PID V_diff_PID;  //差速 pid

float reference_speedr = 15;
float reference_speedl = 20;  //左边测速是右边的4/3

float bias;//由摄像头得到的方向偏移量
int speed_diff;//差速值（占空比）
String bias_str;
void setup() {
  // put your setup code here, to run once:
  move_setup();
  speed_setup(12);  //设置定时器时长（需要与后面的保持一致）
  Serial.begin(115200);
  float kpl, kil, kdl, maxI, maxOut;  //左轮速度pid系数
  kpl = 15;
  kil = 0.08;
  kdl = 2;
  maxI = 10;
  maxOut = 255;
  float kpr, kir, kdr;  //右轮速度pid系数
  kpr = 15;
  kir = 0.08;
  kdr = 2;
  float kp_vd, ki_vd, kd_vd, maxI_vd, maxOut_vd;  //差速pid系数
  kp_vd = 0;
  ki_vd = 0;
  kd_vd = 0;
  maxI_vd = 0;
  maxOut_vd = 0;
  setupPID(&speedPIDR, kpr, kir, kdr, maxI, maxOut);  //void setupPID(PID *pid, float p, float i, float d, float maxI, float maxOut) {
  setupPID(&speedPIDL, kpl, kil, kdl, maxI, maxOut);
  setupPID(&V_diff_PID, kp_vd, ki_vd, kd_vd, maxI_vd, maxOut_vd);
  // helper(0x01, 0x0b, 0x04, 0x00, 0x00, 0x00);  //pid调试助手启动
  // Serial.write(buffer, sizeof(uint8_t)*0x0b);
  // helper(0x02,0x0b,0x04,0x00,0x00,0x00);
  // Serial.write(buffer, sizeof(uint8_t)*0x0b);
}

void loop() {
  // put your main code here, to run repeatedly:
  move_loop(1, pwm_a, pwm_b);
  bias_str=Serial.readString();
  bias=bias_str.toFloat();
  PID_CalcVDiff(&V_diff_PID,0,bias);
  speed_diff=int(V_diff_PID.output);//由PID计算差速
  // if(reference_speedr+speed_diff>255) 
  //   reference_speedr=255;
  // if(reference_speedr+speed_diff<0);
  //   refenrence_speedr=0;
  // else
  //   ren
  PID_CalcIn(&speedPIDR, reference_speedr+speed_diff, v_a);  // PID_CalcIn(PID *pid, float reference, float feedback)
  PID_CalcIn(&speedPIDL, reference_speedl-speed_diff, v_b);//左右加上或减去差速

  //pwm_b=int(speedPID.output);
  // vi = speed_loop();

  // Serial.println(vi);
  // delay(10);

  //串口输出速度
  //Serial.print(v_a);
  //Serial.println(v_b);
  //pid调试助手 发送速度数据

  // helper(0x01, 0x0f, 0x02, uint8_t(v_a), 0x00, 0x00);  //void helper(uint8_t channel, uint8_t l, uint8_t flag, uint8_t i1, uint8_t i2, uint8_t i3)
  // Serial.write(buffer, sizeof(uint8_t)*0x0b);
  // helper(0x02, 0x0f, 0x02, uint8_t(v_b), 0x00, 0x00);
  // Serial.write(buffer, sizeof(uint8_t)*0x0f);
  //pid更新占空比
  pwm_a = int(speedPIDR.output);
  pwm_b = int(speedPIDL.output);
  // Serial.print(pwm_a);
  // Serial.println(pwm_b);
  delay(12);
}
