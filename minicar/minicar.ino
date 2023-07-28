#include "move.h"
#include "speed.h"
#include "mpu6050.h"
#include "helper.h"
#include "pid.h"
#include "grayscale.h"
// #include "protocol.h"

int pwm_a = 100, pwm_b = 100; // pwm_a控制左边电机占空比，pwm_b控制右边电机占空比

int numdata = 0;
PID speedPIDL;
PID speedPIDR;
PID V_diff_PID; // 差速 pid

float reference_speedl = 6.3; // 6.3
float reference_speedr = 8;   // 右边测速是左边的4/3     8

float bias; // 由摄像头得到的方向偏移量
float data_str;
int speed_diff; // 差速值（占空比）
String str;
int status; // 由读到的字符判断当前状态
int flag_status_change = 0;
int minus;
int data1, data2;
int delta1;
int delta2;
int flag = 0;
void setup()
{
  // put your setup code here, to run once:
  move_setup();
  speed_setup(12); // 设置定时器时长（需要与后面的保持一致）
  Serial.begin(115200);
  setAnalog();                       // 开启模拟输入GPIO
  float kpl, kil, kdl, maxI, maxOut; // 左轮速度pid系数
  kpl = 15;
  kil = 0.15;
  kdl = 3;
  maxI = 10;
  maxOut = 255;
  float kpr, kir, kdr; // 右轮速度pid系数
  kpr = 15;
  kir = 0.08;
  kdr = 2;
  float kp_vd, ki_vd, kd_vd, maxI_vd, maxOut_vd; // 差速pid系数 待调
  kp_vd = 2;
  ki_vd = 0.05;
  kd_vd = 5;
  maxI_vd = 0;
  maxOut_vd = 10;
  // maxOut_vd = 50;
  setupPID(&speedPIDR, kpr, kir, kdr, maxI, maxOut); // void setupPID(PID *pid, float p, float i, float d, float maxI, float maxOut) {
  setupPID(&speedPIDL, kpl, kil, kdl, maxI, maxOut);
  setupPID(&V_diff_PID, kp_vd, ki_vd, kd_vd, maxI_vd, maxOut_vd);
  init_ctr();

  // helper(0x01, 0x0b, 0x04, 0x00, 0x00, 0x00);  //pid调试助手启动
  // Serial.write(buffer, sizeof(uint8_t)*0x0b);
  // helper(0x02,0x0b,0x04,0x00,0x00,0x00);
  // Serial.write(buffer, sizeof(uint8_t)*0x0b);
}

void loop()
{
  // put your main code here, to run repeatedly :

  //     if (Serial.available() > 0)
  // {
  //   delay(10);
  //   data1 = Serial.read();
  //   data2 = Serial.read();
  //   for (int i = 0; i < 10; i++)
  //   {
  //     if (buffer1[i] == 0xAB && buffer1[i + 1] == 0xBA)
  //     {
  //       flag = i + 1;
  //       Serial.println(flag);
  //     }

  //     // 从M2D摄像头获取指令及偏移量
  //     if (data1 == 0xAB && data2 == 0xBA)
  //     {
  //       status = Serial.read();
  //       minus = Serial.read();
  //       delta1 = Serial.read();
  //       delta2 = Serial.read();
  //       if (minus == 0x00)
  //       {
  //         bias = delta1 * 256 + delta2;
  //       }
  //       if (minus == 0x01)
  //       {
  //         bias = -(delta1 * 256 + delta2);
  //       }
  //       Serial.print(status);
  //       Serial.println(bias);
  //     }
  //   }

  // 改进：由灰度传感器获得偏移量
  bias = read_bias_from_gray();
  // Serial.println(bias);
  car_control(1);
  Serial.println(speed_diff);
  Serial.println(gray2);
  Serial.println(gray3);
  Serial.println(gray4);
  Serial.println(gray5);
  Serial.println(gray6);
  Serial.println(gray7);

  delay(10);
}

// void loop()
// {
//   setAnalog();
//   readGrayscale();
//   Serial.println(gray2);
//   delay(10);
// }

void car_control(int status_input)
{                        // 控制小车运动， 如停止、前进、掉头等
  if (status_input == 0) // 停止
  {
    move_loop(0, 0, 0);
  }
  if (status_input == 1) // 直行
  {
    go_straight();
  }
  // 先前进 再左转
  if (status_input == 2)
  {
    // Serial.println(status);
    init_ctr();
    while (!distance_enough(200))
    {
      move_loop(0, 75, 75);
      // delay(10);
      Serial.println(counter_pulse_l);
    }
    move_loop(0, 0, 0);
    delay(1000);
    init_ctr();
    // delay(100);
    while (!angle_enough(200))
    {
      move_loop(2, 75, 75);
      // delay(5);
      Serial.println(counter_pulse_l);
    }
    move_loop(0, 0, 0);
    status = 0;
    Serial.println(status);
    delay(1000);
  }
  // 先前进 再右转
  if (status_input == 3)
  {
    // Serial.println(status);
    init_ctr();
    while (!distance_enough(200))
    {
      move_loop(0, 75, 75);
      // delay(10);
      Serial.println(counter_pulse_l);
    }
    move_loop(0, 0, 0);
    delay(1000);
    init_ctr();
    // delay(100);
    while (!angle_enough(200))
    {
      move_loop(3, 75, 75);
      // delay(5);
      Serial.println(counter_pulse_l);
    }
    move_loop(0, 0, 0);
    status = 0;
    delay(1000);
  }
  // 掉头180°
  if (status_input == 4)
  {
    init_ctr();
    while (!angle_enough(400))
    { // 调头
      move_loop(2, 75, 75);
    }
    move_loop(0, 0, 0); // 反方向
  }

  // 查看路口两边的数字
  if (status_input == 5)
  {
    // move_loop(2, 75, 0); // 右轮不动，左轮后退
    init_ctr();
    while (!angle_enough(200)) // 左轮后退
    {
      move_loop(2, 75, 0);
      Serial.println(counter_pulse_l);
    }
    move_loop(0, 0, 0);
    delay(500);
    init_ctr();
    while (!angle_enough(200)) // 左轮上前，恢复路口原始姿态
    {
      move_loop(3, 75, 0);
      Serial.println(counter_pulse_l);
    }
  }
}

void go_straight() // 含有pid的直线行走
{
  // if (Serial.available() > 0 && flag_status_change == 1) // 状态改变了，需要读缓冲区中的用字符串发送的数字
  // {
  // str = Serial.readString();
  // bias = str.toFloat();

  // }
  // else if (flag_status_change == 0) // 状态未改变，将当前接收到的字符串转换为浮点数
  // {
  //     bias = str.toFloat();
  // }
  move_loop(0, pwm_a, pwm_b);
  // Serial.println(bias);
  PID_CalcVDiff(&V_diff_PID, 0, bias);
  speed_diff = int(V_diff_PID.output); // 由PID计算差速
  // if(reference_speedr+speed_diff>255)
  //   reference_speedr=255;
  // if(reference_speedr+speed_diff<0);
  //   refenrence_speedr=0;
  // else
  //   ren
  PID_CalcIn(&speedPIDL, reference_speedl - 0.08 * 0.75 * speed_diff, v_a); // PID_CalcIn(PID *pid, float reference, float feedback)
  PID_CalcIn(&speedPIDR, reference_speedr + 0.08 * speed_diff, v_b);        // 左右加上或减去差速

  // pwm_b=int(speedPID.output);
  //  vi = speed_loop();

  // Serial.println(vi);
  // delay(10);

  // 串口输出速度
  // Serial.print(v_a);
  // Serial.println(v_b);
  // pid调试助手 发送速度数据
  // v_a = int(v_a);
  // helper(0x01, 0x0f, 0x02, uint8_t(v_a), 0x00, 0x00);  //void helper(uint8_t channel, uint8_t l, uint8_t flag, uint8_t i1, uint8_t i2, uint8_t i3)
  // Serial.write(buffer, sizeof(uint8_t)*0x0f);
  // helper(0x02, 0x0f, 0x02, uint8_t(v_b), 0x00, 0x00);
  // Serial.write(buffer, sizeof(uint8_t)*0x0f);
  // pid更新占空比
  pwm_a = int(speedPIDL.output);
  pwm_b = int(speedPIDR.output);
  // Serial.print(pwm_a);
  // Serial.println(pwm_b);
  delay(12);
}
