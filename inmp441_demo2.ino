/**
 * @file inmp441_demo.ino
 * @author Wu Mingyang (783172832@qq.com)
 * @brief ESP32 S3 inmp441 i2s测试, 基于乐鑫官方库ESP32 3.07版本
 * @version 0.1
 * @date 2024-11-27
 */

#include <ESP_I2S.h>

/* I2S引脚配置 */
const uint8_t I2S_MCLK = -1;  // 给外部提供时钟, INMP441不需要
const uint8_t I2S_SCK = 17;    // Audio data bit clock
const uint8_t I2S_WS = 16;    // Audio data left and right clock
const uint8_t I2S_SDOUT = -1; // ESP32 audio data output (to speakers), INMP441不需要
const uint8_t I2S_SDIN = 7;  // ESP32 audio data input (from microphone)

I2SClass i2s;

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 S3 INMP441 I2S Test.");

  /* 1. 配置I2S的引脚 */
  i2s.setPins(I2S_SCK, I2S_WS, I2S_SDOUT, I2S_SDIN, I2S_MCLK);

  /* 2. 初始化I2S, 标准模式, 44100采样率, 16位, 单声道, 左声道 */
  if (!i2s.begin(I2S_MODE_STD, 44100, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO, I2S_STD_SLOT_LEFT)) {
    Serial.println("Failed to initialize I2S bus!");
    return;
  }  

}

void loop() {
  size_t bytes_read = 0; // 用于保存实际读到的字节数
  int16_t read_buffer[64];  // 定义缓存数组, 总共应该有2*64=128个字节

  bytes_read = i2s.readBytes((char *)read_buffer, 128); // 读取128个字节
  if(bytes_read == 128)
  {
    int samples_read = bytes_read / 8;  // 8个字节为一个声音样本, 前4个字节为左声道, 后4个字节为右声道
    
    if (samples_read > 0)
    {
      float mean = 0;
      for(int i = 0; i < samples_read; i++)
      {
        mean += read_buffer[i];
      }
      mean /= samples_read;
      Serial.println(mean);
    }
  }
  else
  {
    Serial.println("INMP441 record fail!");
    delay(100);
  }

}
