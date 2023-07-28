// 从六个模拟引脚读取灰度传感器返回的值

int gray2, gray3, gray4, gray5, gray6, gray7; // 灰度传感器2号到7号传感器的测量结果
float k1, k2, k3;                             // 传感器权重
void setAnalog()
{
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
}
void readGrayscale() // 读数越高，画面越黑。 读数最高处为黑线位置
{
    gray2 = analogRead(A0);
    gray3 = analogRead(A1);
    gray4 = analogRead(A2);
    gray5 = analogRead(A3);
    gray6 = analogRead(A4);
    gray7 = analogRead(A5);
}

float gray2bias() // 由灰度传感器的读数得到一个线条的偏移量 返回值为负数表示向左偏，为正数表示向右偏
{
    // 黑线和白色灰度值需实测
    // 黑线：620
    // 白色：200
    int standard = 0; // 待调  standard代表白色  传感器读数减去standard
    k1 = 0.8;
    k2 = 0.4;
    k3 = 0.2;
    float factors[6] = {-k1, -k2, -k3, k3, k2, k1}; // 待调 六个传感器读数的权重  越靠边的传感器权重越大
    // 计算偏移量
    int gray_bias = factors[0] * (gray2 - standard) + factors[1] * (gray3 - standard) + factors[2] * (gray4 - standard) + factors[3] * (gray5 - standard) + factors[4] * (gray6 - standard) + factors[5] * (gray7 - standard);
    return gray_bias; //   setAnalog();
    //   readGrayscale();
    //   Serial.println(gray2);
    //   delay(10);
}

float read_bias_from_gray()
{
    float a;
    readGrayscale();
    a = gray2bias();
    return a;
}