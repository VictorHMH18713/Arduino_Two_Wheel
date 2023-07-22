#define AIN1 12
#define AIN2 13
#define PWMA 10
#define BIN1 7
#define BIN2 6
#define PWMB 9

void move_setup(){
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  
}

void move_loop(int flag,int pwm_a,int pwm_b) {
  // put your main code here, to run repeatedly:
  switch(flag)
  {
    case 0://前进/后退
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMA, pwm_a);
    analogWrite(PWMB, pwm_b);
    break;
    case 1://前进/后退
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMA, pwm_a);
    analogWrite(PWMB, pwm_b);
    break;
    case 2://左右转
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMA, pwm_a);
    analogWrite(PWMB, pwm_b);
    break;
    case 3://左右转
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMA, pwm_a);
    analogWrite(PWMB, pwm_b);
    break;
  }
  

}
