
#include "Define.h"

//*****************************************
//**************�жϴ���*******************
void interrupt ISR(void)//PIC_HI-TECHʹ��
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
//*************ϵͳ����*******************
void POWER_INITIAL (void) 
{
 OSCCON = 0x70;		//0111 0000; //WDT 32KHZ IRCF=111=16MHZ/4=4MHZ,0.25US (2T)
 INTCON = 0x00;		//�ݽ�ֹ�����ж�
 
 PORTA = 0x00; 		//0000 0000//���0��1
 TRISA = 0x04;		//0000 0100//RA2,3 in; OTHER out(0:OUT,1:IN.) //�����������״̬
 WPUA = 0x04;    	//0000 0100//PA2������

 WDTCON = 0x00;		//off WDT
}

//**************��ʱ����ʼ��***************************
void TIMER_INITIAL (void) 
{
//T1
 OPTION = 0x03;		//0000 0011; //Bit3=0 T0 MODE,PS<2-0>011 T0 RATE 1:16(0.25*16=4US)(run 4M)
                    //Bit7(RAPU)=0 ENABLED PULL UP PA
 T0IE = 1;	//T0 ����ж�ʹ��
 T0IF = 0;	//T0 ����жϱ�־	

//T2 (on)
 T2CON = 0x79;  //Bit[1,0]=01,T2ʱ�ӷ�Ƶ 1:4  0111 1001
            	//Bit[6-3]=1111,T2���ʱ�ӷ�Ƶ1:1 6   =0.25*16*4=16us
 TMR2 = 0;  	//TMR2����ֵ
 PR2 = 125; 	//����TMR2����Ƚ�ֵ��ʱ2ms=16us*125(PR2)
 TMR2IE = 1;	//ʹ��TMER2���ж�
 TMR2ON = 1;	//ʹ��TMER2����
 PEIE = 1;    	//ʹ�������ж�

 GIE = 1;   	//ʹ��ȫ���ж�
}
//***********���ݳ�ʼ��**********************
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


