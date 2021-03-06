# -*- coding:utf-8 -*-
"""
   @file speechRecognition.ino
   @brief 语音识别，有三种模式.
   @n 1.循环模式: 会一直循环地识别目标词条，直到成功识别
   @n 2.按钮模式: 按一下按钮才会开始识别，再按一次按钮结束识别
   @n 3.指令模式：第一个词条为指令，传感器只有识别到第一个词条才会正常开始工作，
   @n             若无输入，传感器会在十秒内进入空闲模式，需要再次输入指令

   @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
   @licence     The MIT License (MIT)
   @author [fengli](li.feng@dfrobot.com)
   @version  V1.0
   @date  2020-08-17
   @get from https://www.dfrobot.com
   @https://github.com/DFRobot/DFRobot_ASR
"""
import sys
import time
sys.path.append("../..")
from DFRobot_ASR import *

IIC_MODE         = 0x01            # default use IIC1
IIC_ADDRESS      = 0x4F           # default i2c device address
'''
   # The first  parameter is to select iic0 or iic1
   # The second parameter is the iic device address
'''
asr = DFRobot_ASR(IIC_MODE ,IIC_ADDRESS)
"""
     @brief 初始化函数
     @param 语音识别模式1.LOOP(循环模式)2.PASSWORD(指令模式)3.BUTTON(按钮模式)
     @return 返回0表示初始化成功，返回其他值表示初始化失败，返回错误码
"""

asr.begin(asr.LOOP,asr.MIC)
"""
   * 只能识别汉字，将要识别的汉字转换成拼音字母，每个汉字之间空格隔开，比如：开始 --> kai shi
   * 最多添加50个词条，每个词条最长为79个字符，每个词条最多10个汉字
   * 每个词条都对应一个识别号（1~255随意设置）不同的语音词条可以对应同一个识别号，
"""
asr.addCommand("kai deng",0)     #开灯

asr.addCommand("guan deng",1)             #关灯
'''在指令模式下，第一个词组为开始指令'''
asr.addCommand("bei jing",2)              #北京
asr.addCommand("shang hai",3)       #上海
asr.addCommand("xiang gang",4)             #香港
#开始识别
asr.start()
print("start\r\n")


while(1):
    result = asr.read()
    if result != -1 :
           print("ASR result is:%d"%result)
           #print(result);
          # print("\r\n");