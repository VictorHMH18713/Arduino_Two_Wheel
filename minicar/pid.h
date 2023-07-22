//该头文件功能为两级PID，也可以只使用一级，即PID_CalcIn
struct PID{
  float kp, ki, kd;
  float error, lastError;
  float integral, maxIntegral;
  float output, maxOutput;
};

// 串级PID的结构体，包含两个单级PID
typedef struct {
  PID balancePID;  //内环
  PID speedPID;    //外环
  float output;    //串级输出，等于inner.output
} CascadePID;

void setupPID(PID *pid, float p, float i, float d, float maxI, float maxOut) {
  pid->kp = p;
  pid->ki = i;
  pid->kd = d;
  pid->maxIntegral = maxI;
  pid->maxOutput = maxOut;
}
//feedback是待调轮子的速度，reference是另一个轮子的速度（参考值）
void PID_CalcIn(PID *pid, float reference, float feedback) {
  //更新数据
  pid->lastError = pid->error;         //将旧error存起来
  pid->error = reference - feedback;  //计算新error
  //计算微分
  float dout=(pid->error-pid->lastError)*pid->kd;
  //float dout = feedback2 * pid->kd;
  //计算比例
  float pout = pid->error * pid->kp;
  //计算积分
  pid->integral += pid->error * pid->ki;
  //积分限幅
  if (pid->integral > pid->maxIntegral) pid->integral = pid->maxIntegral;
  else if (pid->integral < -pid->maxIntegral) pid->integral = -pid->maxIntegral;
  //计算输出
  pid->output = pout + dout + pid->integral;
  //输出限幅
  if (pid->output > pid->maxOutput) pid->output = pid->maxOutput;
  else if (pid->output < -pid->maxOutput) pid->output = -pid->maxOutput;
}

void PID_CalcOut(PID *pid, float reference, float feedback) {
  //更新数据
  pid->lastError = pid->error;        //将旧error存起来
  pid->error = reference - feedback;  //计算新error
  //计算微分
  float dout = (pid->error - pid->lastError) * pid->kd;
  // float dout = feedback2*pid->kd;
  //计算比例
  float pout = pid->error * pid->kp;
  //计算积分
  pid->integral += pid->error * pid->ki;
  //积分限幅
  if (pid->integral > pid->maxIntegral) pid->integral = pid->maxIntegral;
  else if (pid->integral < -pid->maxIntegral) pid->integral = -pid->maxIntegral;
  //计算输出
  pid->output = pout + dout + pid->integral;
  //输出限幅
  if (pid->output > pid->maxOutput) pid->output = pid->maxOutput;
  else if (pid->output < -pid->maxOutput) pid->output = -pid->maxOutput;
}

//串级PID的计算函数
//参数(PID结构体,外环目标值,外环反馈值,内环反馈值)
void PID_CascadeCalc(CascadePID *pid, float outerRef, float outerFdb, float innerFdb1, float innerFdb2) {
  PID_CalcOut(&pid->speedPID, outerRef, outerFdb);                           //计算外环
  PID_CalcIn(&pid->balancePID, pid->speedPID.output, innerFdb1);  //计算内环
  pid->output = pid->balancePID.output;                                      //内环输出就是串级PID的输出
}

CascadePID mypid;  //创建串级PID结构体变量