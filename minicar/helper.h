// #include <MsTimer2.h>

uint8_t head[] = {0X53, 0x5A, 0X48, 0X59};
// int channel = 0x01;
uint8_t length[] = {0X00, 0X00, 0X00, 0X00};
// int flag = 0X01;//PID对应的功能码
uint8_t index1[] = {0x00, 0x00, 0x00, 0x00};
uint8_t index2[] = {0x00, 0x00, 0x00, 0x00};
uint8_t index3[] = {0x00, 0x00, 0x00, 0x00};
uint8_t sum;
uint8_t buffer[23];




void helper(uint8_t channel, uint8_t l, uint8_t flag, uint8_t i1, uint8_t i2, uint8_t i3) {
  // put your setup code here, to run once:

  length[0] = l;
  index1[0] = i1 & 0xFF;
  index1[1] = i1 & 0xFF00;
  index1[2] = i1 & 0xFF0000;
  index1[3] = i1 & 0xFF000000;
  index2[0] = i2 & 0xFF;
  index2[1] = i2 & 0xFF00;
  index2[2] = i2 & 0xFF0000;
  index2[3] = i2 & 0xFF000000;
  index3[0] = i3 & 0xFF;
  index3[1] = i3 & 0xFF00;
  index3[2] = i3 & 0xFF0000;
  index3[3] = i3 & 0xFF000000;
  uint8_t chan[] = {channel};
  uint8_t fl[] = {flag};
  sum = channel + flag;
  for(int k = 0; k < 4; k++){
    sum = sum + head[k] + length[k] + index1[k] + index2[k] + index3[k];
  }
  sum = sum&0xFF;
  uint8_t su[] = {sum};
  // if(isr == 1){
    // isr = 0;
    // Serial.write(head, sizeof(head));
    // Serial.write(chan, sizeof(chan));
    // Serial.write(length, sizeof(length));
    // Serial.write(fl, sizeof(fl));
  for(int k = 0; k < 4; k++){
    buffer[k] = head[k];
  }
  buffer[4] = chan[0];
  for(int k = 0; k < 4; k++){
    buffer[k + 5] = length[k];
  }
  buffer[9] = fl[0];
  if(l == 0x0f || l == 0x17){
    // Serial.write(index1, sizeof(index1));
    for(int k = 0; k < 4; k++){
    buffer[k + 10] = index1[k];
    }
  }
  if(l == 0x17){
    // Serial.write(index2, sizeof(index2));
    // Serial.write(index3, sizeof(index3));
    for(int k = 14; k < 4; k++){
    buffer[k + 5] = index2[k];
  }
    for(int k = 18; k < 4; k++){
    buffer[k + 5] = index3[k];
  }
  }
  // Serial.write(su, sizeof(su));
  buffer[l-1] = su[0];
  

}

// void helper_loop(int channel, int l, int flag, int i1, int i2, int i3) {
//   // put your main code here, to run repeatedly:
//   // Serial.println(num);
//   // num += 1;
//   helper_setup(channel, l, flag, i1, i2, i3);
//   Serial.write(package,l);
// }
