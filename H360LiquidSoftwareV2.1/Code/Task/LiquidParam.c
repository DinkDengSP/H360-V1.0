#include "LiquidParam.h"
#include "ResAD5272.h"

//滑动变阻器的基准值
static volatile uint16_t resWithAD5272RefValue = 0;

static void LiquidWriteParam(uint8_t index,uint16_t param)
{
    EEpromWriteWord(index,param);
}

static uint16_t LiquidReadParam(uint8_t index)
{
    return EEpromReadWord(index);
}

//读取液面探测电压基准值
uint16_t LiquidGetParamResWithAD5272RefValue()
{
    /*从存储器中读取 */
        resWithAD5272RefValue = LiquidReadParam(1);
    /*数据检查,不能超过1023,超过就修正为1023,同时写入EEPROM */
        if(resWithAD5272RefValue > 1023)
        {
            resWithAD5272RefValue = 1023;
            LiquidWriteParam(1,resWithAD5272RefValue);
        }
    /*返回最终的结果 */
        return resWithAD5272RefValue;
}


//写入液面探测电压基准值
void LiquidSetParamResWithAD5272RefValue(uint16_t paramValue)
{
    /*对输入参数检查,不能大于1023,否则就是1023*/
        if(paramValue > 1023)
        {
            paramValue = 1023;
        }
    /*内部状态设定 */
        resWithAD5272RefValue = paramValue;
    /*将数据写入EEPROM */
        LiquidWriteParam(1,resWithAD5272RefValue);
    /*将数据写入AD5272 */
        ResAD5272Write(resWithAD5272RefValue);
    /*延时一段时间生效,但是不再API中延时了 */
}




