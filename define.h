
#include "FMD.h"
#include "FT60F01X.h"

#define	false 0
#define true 1
#define  uchar     unsigned char 
#define  uint      unsigned int
#define  ulong     unsigned long
/**************************************
Define  I/O Pin
***************************************/
#define 	p_Key    	RA2
#define		DEBUG		RA5

/**************************************
Define constant
***************************************/
void Debug(uint high);
void Debug(uint high)
{
    for(high;high>0;--high)
    {
        DEBUG = 1;
        _nop();
        _nop();
        DEBUG = 0;
        _nop();
        _nop();
    }    
}

/**************************************
***************************************/


enum MODE{
   LongPress,
   ShortPress,
   DoublePress,
   NoPress,
   TemPress//过渡按键
}LastKey;
struct {
    enum MODE mode;
    uint PressCount;
    uint LongPressFlag;
    uint DoubleCount;
    uchar DoubleJudgeFlag;
    uint DoubleTotal;
}key;//////////////////按键类型



struct {
    uchar b_T2ms;

}sysPar;//////其他系统参数





