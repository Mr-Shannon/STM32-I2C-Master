#include "tools.h"
/*******************************************************************************
* Function Name  : tool_calculator_crc16
* Description    : CRC16
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t tool_calculator_crc16(uint8_t *buf, uint16_t length, uint16_t crc_reg)
{
  uint16_t value;

  for (uint16_t i = 0; i < length; i ++) 
  {
    value = buf[i] << 8;

    for (uint8_t j = 0; j < 8; j ++) 
    { 
      if ((short)(crc_reg ^ value) < 0)
        crc_reg = (crc_reg << 1) ^ 0x1021;
      else
        crc_reg <<= 1; 
      value <<= 1;            
    }
  }
  return crc_reg;
}

