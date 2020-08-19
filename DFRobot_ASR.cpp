/*!
   @file DFRobot_ASR.h
   @brief DFRobot_ASR 类的基础结构，基础方法的实现
   @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
   @licence     The MIT License (MIT)
   @author [fengli](li.feng@dfrobot.com)
   @version  V1.0
   @date  2020-08-17
   @get from https://www.dfrobot.com
   @https://github.com/DFRobot/DFRobot_ASR
*/
#include "DFRobot_ASR.h"
DFRobot_ASR::DFRobot_ASR(TwoWire *pWire, uint8_t address){

  _deviceAddr = address;
  _pWire = pWire;

}
int DFRobot_ASR::begin(eMode_t mode)
{
  _mode = mode;
  _pWire->begin();
  _pWire->beginTransmission(_deviceAddr);
  if(_pWire->endTransmission() != 0){
    DBG("bus data access error");
    return ERR_DATA_BUS;
   }
  writeReg(ASR_BEGIN);
  delay(50);
  return ERR_OK;
}
void DFRobot_ASR::start()
{
  writeReg(ASR_START);
  delay(50);
}
bool DFRobot_ASR::addCommend( char *words, char idNum)
{
   _pWire->beginTransmission(I2C_ADDR);
   _pWire->write(ASR_ADDCOMMAND);
   _pWire->write(idNum);
   _pWire->write(strlen(words));
   _pWire->write((uint8_t *)words,strlen(words));
   _pWire->write(ASR_ADDCOMMAND_END);
   if( _pWire->endTransmission() != 0 ) {
       delay(10);
       return false;
   }
   delay(20);
   return true;
}

int DFRobot_ASR::read(){

  int result = 0;
  switch (_mode)
  {

     case      BUTTON: readReg(ASR_BUTTON,&result,1);
                       break;   
     case       LOOP: readReg(ASR_LOOP,&result,1);
               break;
     case      PASSWORD: readReg(ASR_PASSWORD,&result,1);
               break;          
     default: break;
  }
  if(_mode == PASSWORD){
    idle++;
    if(idle >= 500){
       writeReg(ASR_IDLE);
       idle = 0;
    }
  }
  if(result == 0xff){
    return -1;
  }
  else {
    idle = 0;
    return result;
  }
}

void DFRobot_ASR::writeReg(uint8_t reg, uint8_t data)
{
  _pWire->beginTransmission(_deviceAddr);
  _pWire->write(reg);
  _pWire->write(data);
  _pWire->endTransmission();
}
uint8_t DFRobot_ASR::writeReg(uint8_t reg)
{

    _pWire->beginTransmission(_deviceAddr);
    _pWire->write(reg);
    if( _pWire->endTransmission() != 0 ) {
        return false;
    }
    return true;
}



uint8_t DFRobot_ASR::readReg(uint8_t reg, void* pBuf, size_t size)
{
  if(pBuf == NULL){
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
    pBuf[0] = 0xff;
    unsigned char i = 0;  
    /* Indicate which register we want to read from */
    if (!writeReg(reg)) {
        return -1;
    }
    delay(20);
   _pWire->requestFrom(_deviceAddr, size);
   while (_pWire->available()) {
        if (i >= size) {
            return -1;
        }
        _pBuf[i] = _pWire->read();
        i++;
    }
    /* Read block data */    
    return i;

}