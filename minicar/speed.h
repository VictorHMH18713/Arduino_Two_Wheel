#include <MsTimer2.h>
#include <PinChangeInterrupt.h>

#define A0_v 2
// #define B0 5
#define A1_v 4
// #define B1 8

// 两个轮子的速度
float v_a, v_b;
float v;
volatile long counter_val0 = 0;
volatile long counter_val1 = 0; // 该变量用于存储编码器的值，所以用类型修饰符volatile；
long counter_pulse_l = 0;       // 记录转过的角度  待测：90°相当于多少脉冲？
long counter_pulse_r = 0;
int j = 0; // 定时器标志；
volatile int flag0 = 0;
volatile int flag1 = 0;

void counter0()
{
  counter_val0++; // 每一个中断加一
  counter_pulse_l++;
  // flag0 = digitalRead(B0);
  // flag0 = 1-2*flag0;
}

void counter1()
{
  counter_val1++; // 每一个中断加一
  counter_pulse_r++;
  // Serial.println("okn");
  // flag1 = digitalRead(B1);
  // flag1 = 1-2*flag1;
}

// 定时器中断处理函数
void timerIsr()
{
  j = 1; // 定时时间达到标志
  v_a = counter_val0;
  v_b = counter_val1;
  counter_val0 = 0;
  counter_val1 = 0; // 清空该时间段内的脉冲数
}
void speed_setup(int t)
{
  pinMode(A0_v, INPUT_PULLUP);
  // pinMode(B0, INPUT_PULLUP);
  pinMode(A1_v, INPUT_PULLUP);
  // pinMode(B1, INPUT);
  // Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(A0_v), counter0, RISING);      // 设置编码器B相位上升沿中断
  attachPinChangeInterrupt(digitalPinToPCINT(A1_v), counter1, RISING); // 4号针脚属于INT0中断，这里填0（这个规则真逆天）
  MsTimer2::set(t, timerIsr);                                          // 定时单位ms
  MsTimer2::start();
}
float speed_loop()
{
  if (j == 1) // 判断是否发生定时器中断，这里是50ms发生一次
  {
    j = 0; // 清除标记
    //  Serial.println(v);
    v = counter_val0;
    return v;
  }
}
bool angle_enough(int destined_pulse) // 判断是否转够角度了
{
  if (counter_pulse_l >= destined_pulse || counter_pulse_r >= destined_pulse)
  {
    return 1;
  }
  else
    return 0;
}
bool distance_enough(int destined_distance)
{
  if (counter_pulse_l >= destined_distance || counter_pulse_r >= destined_distance)
  {
    return 1;
  }
  else
    return 0;
}
void init_ctr()
{
  counter_pulse_l = counter_pulse_r = 0;
}

// 外部中断处理函数
