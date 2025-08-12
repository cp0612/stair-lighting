#include <RadioLib.h>
SX1276 radio = new Module(18, 26, 14, 25);  // ESP32-C3 SPI引脚

// 继电器控制引脚
const int relayPins[3] = {12, 13, 15}; 

void setup() {
  // 初始化继电器GPIO
  for(int i=0; i<3; i++) pinMode(relayPins[i], OUTPUT);
  
  // 初始化LoRa
  radio.begin(868.0, 125.0, 9, 7, 0x34, 20); // 频率868MHz, SF=9
}

void loop() {
  // 1. 检测本地开关（示例：按键1按下）
  if(digitalRead(4) == HIGH) {
    digitalWrite(relayPins[0], !digitalRead(relayPins[0])); // 切换灯1状态
    delay(200); // 防抖
  }
  
  // 2. 监听LoRa全局指令
  String recvMsg;
  if(radio.receive(recvMsg) == RADIOLIB_ERR_NONE) {
    if(recvMsg.startsWith("GLOBAL:")) {
      handleGlobalCommand(recvMsg);
    }
  }
}

// 处理全局指令
void handleGlobalCommand(String cmd) {
  if(cmd == "GLOBAL:ALL_ON") {
    for(int i=0; i<3; i++) digitalWrite(relayPins[i], HIGH);
  } 
  else if(cmd == "GLOBAL:ALL_OFF") {
    for(int i=0; i<3; i++) digitalWrite(relayPins[i], LOW);
  }
}