#include<string.h>
#include <stdio.h>

// float stringToFloat(const char *str) {
//     float result;
//     sscanf(str, "%f", &result);
//     return result;
// }

String data_str;
float data;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    data_str=Serial.readString();
    // Serial.println(data_str);
    data=data_str.toFloat();
    data=data*10;
    Serial.println(data);
  }
  delay(10);
}

