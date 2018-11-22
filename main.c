
#include "Define.h"

//*****************************************
//**************中断处理*******************
void interrupt ISR(void)//PIC_HI-TECH使用
{
 // GIE = 0;
  if(T0IF == 1)
  {
   	T0IF = 0;
	TMR0 = 231;			//(256-231)25*4= 100us 
//----------------------------------------------


//-----------------------------------------------

  }
  else if(TMR2IE && TMR2IF)
  {
  	TMR2IF = 0;
	sysPar.b_T2ms = true;
  }
}


/*************************************************
FT60F011A
;------------------------------------------------
        |---u---|
   VDD--|1     8|--VSS
   PA2--|2     7|--PA4	B
   PA1--|3     6|--PA5	R
   PA3--|4     5|--PA0	G
        |-------|
*************************************************/
//*************系统函数*******************
void POWER_INITIAL (void) 
{
 OSCCON = 0x70;		//0111 0000; //WDT 32KHZ IRCF=111=16MHZ/4=4MHZ,0.25US (2T)
 INTCON = 0x00;		//暂禁止所有中断
 
 PORTA = 0x00; 		//0000 0000//输出0和1
 TRISA = 0x04;		//0000 0100//RA2,3 in; OTHER out(0:OUT,1:IN.) //设置输入输出状态
 WPUA = 0x04;    	//0000 0100//PA2口上拉

 WDTCON = 0x00;		//off WDT
}

//**************定时器初始化***************************
void TIMER_INITIAL (void) 
{
//T1
 OPTION = 0x03;		//0000 0011; //Bit3=0 T0 MODE,PS<2-0>011 T0 RATE 1:16(0.25*16=4US)(run 4M)
                    //Bit7(RAPU)=0 ENABLED PULL UP PA
 T0IE = 1;	//T0 溢出中断使能
 T0IF = 0;	//T0 溢出中断标志	

//T2 (on)
 T2CON = 0x79;  //Bit[1,0]=01,T2时钟分频 1:4  0111 1001
            	//Bit[6-3]=1111,T2输出时钟分频1:1 6   =0.25*16*4=16us
 TMR2 = 0;  	//TMR2赋初值
 PR2 = 125; 	//设置TMR2输出比较值定时2ms=16us*125(PR2)
 TMR2IE = 1;	//使能TMER2的中断
 TMR2ON = 1;	//使能TMER2启动
 PEIE = 1;    	//使能外设中断

 GIE = 1;   	//使能全局中断
}
//***********数据初始化**********************
void Parameter_INITIAL(void)
{
    key.DoubleTotal = 0;
    key.DoubleJudgeFlag = 0;
    key.PressCount = 0;
    LastKey = TemPress;
    key.LongPressFlag = 0;
}
void ScanKey(void)
{
    if(p_Key == 0)
    {
        
        if(key.LongPressFlag == 0) key.PressCount++;
        if(key.PressCount > 600)//2ms*600=1.2s
        {

            key.PressCount = 0;
            key.mode = LongPress;
            key.LongPressFlag = 1;
        }
        else
        {
            key.mode = NoPress;
        }
    }
    else if(key.PressCount > 20 && key.PressCount < 600 && key.mode != LongPress)
        {
            key.LongPressFlag = 0;
            key.PressCount = 0;
            ++key.DoubleTotal;
            key.DoubleJudgeFlag = 1;
        }
    else
    {
        key.LongPressFlag = 0;
        key.PressCount = 0;
        key.mode = NoPress;
    }
    if(key.mode != NoPress) LastKey = key.mode;
}
void DoubleJudge(void)
{
    if(key.DoubleJudgeFlag == 1)
    {
        if(p_Key != 0) key.DoubleCount++;
        if(key.DoubleCount > 100)
        {
            key.mode = ShortPress;
            key.DoubleTotal = 0;
            key.DoubleCount = 0;
            key.DoubleJudgeFlag = 0;
        }
        else
        {
            if(key.DoubleTotal > 1)
            {
                key.mode = DoublePress;
                key.DoubleTotal = 0;
                key.DoubleCount = 0;
                key.DoubleJudgeFlag = 0;
            }
        }
    }
}
void HandleKey(void)
{
    if(key.mode == ShortPress)  Debug(2);
    else if(key.mode == DoublePress)    Debug(3);
    else if(key.mode == LongPress)  Debug(4);
    else    DEBUG = 0;
}
/***************************************
;main
****************************************/
void main(void)
{
	POWER_INITIAL();
	TIMER_INITIAL();
	Parameter_INITIAL();
	while(1)
	{
        if(sysPar.b_T2ms == true)
        {
            sysPar.b_T2ms = false;
            ScanKey();
            DoubleJudge();
            HandleKey();
        }
    }
}


