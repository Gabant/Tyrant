
#include "Mycommon.h"

//设置优先级

float Fuzzy_Servo ;
int midline[ROW] ;

int time1,time2;

/***************************拨码开关标志位*****************************************/
int SendData_Flag = 0, SingleRun_Flag = 0, DoubleRun_Flag = 0, Adjustment_Flag = 0;

void nvic_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure0;
	NVIC_InitStructure0.NVIC_IRQChannel = PIT0_IRQn;                         //指明要配置的通道号 
	NVIC_InitStructure0.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure0.NVIC_IRQChannelPreemptionPriority = 0x03;            //抢占优先级1  
	NVIC_InitStructure0.NVIC_IRQChannelSubPriority = 0x00;                   //响应优先级1 
	NVIC_InitStructure0.NVIC_IRQChannelEnable = TRUE;                        //使能PIT0_IRQn中断通道 
	LPLD_NVIC_Init(NVIC_InitStructure0);
        //摄像头
	NVIC_InitTypeDef NVIC_InitStructure1;
	NVIC_InitStructure1.NVIC_IRQChannel = PORTB_IRQn;
	NVIC_InitStructure1.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 0x00;  //抢占优先级0  
	NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 0x00;     //响应优先级0  
	NVIC_InitStructure1.NVIC_IRQChannelEnable = TRUE;      //使能外部中断通道 
	LPLD_NVIC_Init(NVIC_InitStructure1);     //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	
        
        //超声波
        NVIC_InitTypeDef NVIC_InitStructure3;
        
	NVIC_InitStructure3.NVIC_IRQChannel = PORTD_IRQn;
	NVIC_InitStructure3.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure3.NVIC_IRQChannelPreemptionPriority = 0x02;  //抢占优先级2 
	NVIC_InitStructure3.NVIC_IRQChannelSubPriority = 0x00;     //响应优先级0 
	NVIC_InitStructure3.NVIC_IRQChannelEnable = TRUE;      //使能外部中断通道
	LPLD_NVIC_Init(NVIC_InitStructure3);     //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	//nRF
        NVIC_InitTypeDef NVIC_InitStructure4;
        
	NVIC_InitStructure4.NVIC_IRQChannel = PORTC_IRQn;
	NVIC_InitStructure4.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_2;
	NVIC_InitStructure4.NVIC_IRQChannelPreemptionPriority = 0x01;  //抢占优先级1 
	NVIC_InitStructure4.NVIC_IRQChannelSubPriority = 0x00;     //响应优先级0 
	NVIC_InitStructure4.NVIC_IRQChannelEnable = TRUE;      //使能外部中断通道
	LPLD_NVIC_Init(NVIC_InitStructure4);     //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

}

void Show_LCD_Image(unsigned char videodata[ROW][COLUMN])
{	
        
        unsigned char j = 0, temp = 0, a = 0, k = 0;
        
	OLED_Set_Address(0, 0);
	for (k = 0; k < 128; k++)
	{
                  
          for (j = 0; j < 8; j++)
		{
			OLED_Set_Address(j, k);
			//按比例显示在显示屏上
			temp = (videodata[j * 8 * (ROW - 1) / 63][k*(COLUMN - 1) / 127]) | (videodata[(j * 8 + 1)*(ROW - 1) / 63][k*(COLUMN - 1) / 127] << 1)
				| (videodata[(j * 8 + 2)*(ROW - 1) / 63][k*(COLUMN - 1) / 127] << 2) | (videodata[(j * 8 + 3)*(ROW - 1) / 63][k*(COLUMN - 1) / 127] << 3)
				| (videodata[(j * 8 + 4)*(ROW - 1) / 63][k*(COLUMN - 1) / 127] << 4) | (videodata[(j * 8 + 5)*(ROW - 1) / 63][k*(COLUMN - 1) / 127] << 5)
				| (videodata[(j * 8 + 6)*(ROW - 1) / 63][k*(COLUMN - 1) / 127] << 6) | (videodata[(j * 8 + 7)*(ROW - 1) / 63][k*(COLUMN - 1) / 127] << 7);
			

                        if(j==contrline_use/8)
                        {
                          a=0xff&(~(1<<(contrline_use%8)));
                          temp=temp&a;
                        }
                        /*
                        if(j==(contrline_use+6)/8)
                        {
                          b=0xff&(~(1<<((contrline_use+6)%8)));
                          temp=temp&b;
                        }
                        
                        if(j==(contrline_use-6)/8)
                        {
                          c=0xff&(~(1<<((contrline_use-6)%8)));
                          temp=temp&c;
                        }
                        */
                        if(midline[contrline_use]==k*(COLUMN - 1) / 127||k==64)
                        {
                          temp=0;
                        }
                       
                        OLED_Write_Data(temp);
		}
	}
}



/*
void Show_LCD_Bianjie(int L[ROW],int R[ROW],int midline[ROW])
{
  unsigned char i = 0;
  unsigned char temp;
  
  OLED_Set_Address(0,0);
  
  for(i=end_Row;i<ROW;i++)
  {
    temp=1<<(i%8);
  
   {
    if(L[i]<138&&L[i]>10)
    {
      OLED_Set_Address(i/8,L[i]-10);
      OLED_Write_Data(temp);
    }
    
    if(R[i]<138&&R[i]>10)
    {
      OLED_Set_Address(i/8,R[i]-10);
      OLED_Write_Data(temp);
    }
    
    if(midline[i]<138&&midline[i]>10)
    {
      OLED_Set_Address(i/8,midline[i]-10);
      OLED_Write_Data(temp);
    }
   }
  }
    
}
*/


//显示左线，右线，中线，截止最大行，截止行，控制行
void showmoi()
{
  unsigned char i = 0, j = 0, temp = 0;
  show_3line();
  OLED_Set_Address(0, 0);
  for (i = 0; i < 128; i++)
  {
          for (j = 0; j < 8; j++)
          {
                  OLED_Set_Address(j, i);
                  //按比例显示在显示屏上
                  temp = (showline[j * 8 * (ROW - 1) / 63][i*(COLUMN - 1) / 127]) | (showline[(j * 8 + 1)*(ROW - 1) / 63][i*(COLUMN - 1) / 127] << 1)
                          | (showline[(j * 8 + 2)*(ROW - 1) / 63][i*(COLUMN - 1) / 127] << 2) | (showline[(j * 8 + 3)*(ROW - 1) / 63][i*(COLUMN - 1) / 127] << 3)
                          | (showline[(j * 8 + 4)*(ROW - 1) / 63][i*(COLUMN - 1) / 127] << 4) | (showline[(j * 8 + 5)*(ROW - 1) / 63][i*(COLUMN - 1) / 127] << 5)
                          | (showline[(j * 8 + 6)*(ROW - 1) / 63][i*(COLUMN - 1) / 127] << 6) | (showline[(j * 8 + 7)*(ROW - 1) / 63][i*(COLUMN - 1) / 127] << 7);
                  OLED_Write_Data(temp);
          
             
          }
  }
  
}

void System_Init()
{
      uint8 status=0;
     status=LPLD_Nrf24L01_Init();
     //检测nrf是否正常工作
     if(!status)        //报警提示
     {
       OLED_Init();
       OLED_Write_Str16X8(0,0,"NRF maybe WRONG");
       while(OLED_Key_Scan()==0);
     }
  
     BOMA_Scanf();//拨码开关，相应的函数在flash.c,引脚初始化在oled.c中的OLED_IO_Init中
     direction_pwm_init();
    
      
     if(SingleRun_Flag)
     {
       OLED_Init();
       while(OLED_Key_Scan()==0)
         OLED_Write_Str16X8(1,0,"   SingleRun  ");
     }   
     
      if(DoubleRun_Flag)
     {
       OLED_Init();
       nvic_init();
       nrfReceive_init();
       ultrasonic_init();
       PIT1_Init();
       while(OLED_Key_Scan()==0)
         OLED_Write_Str16X8(1,0,"   DoubleRun   ");
     } 
     
     if(Adjustment_Flag)
     {
       OLED_Init();
       while(OLED_Key_Scan()==0)
         OLED_Write_Str16X8(1,0,"  AdjustmentPic  ");
     }   
     
     if(SendData_Flag)
     {
       OLED_Init(); 
       while(OLED_Key_Scan()==0)
         OLED_Write_Str16X8(1,0,"   VisualScope   ");
     }   
  
  
     motor_pwm_init();
     
     Video_init();
     BELL_init();
     motor_DMA_Init();
     uart_init();
    
     pit_init();     
     OLED_Set_OLED();
}
 


unsigned char show;
uint8 IncP=0;
void main(void)         /////uart模块传输数据会严重拖慢时间
{

   DisableInterrupts; 
   System_Init();
   EnableInterrupts;
     
  while(1)
  { 
   
    if(run_oled_Scan())
    {
      show++;
      OLED_Full( 0 );    
    }
    
    if(show==5) show=0;
   
   
   
   if (field_over)
    {    
         if(show==0) {}
         else
         {
           if (show==1) OLED_Page_1();
                       
           if (show==2) OLED_Page_2();

           if(show==3)  Show_LCD_Image(videoData);

           if(show==4)  showmoi();
          }
         
       camare_deal();
       field_over = 0;
       
       
    /*****************************跑车模式*************************************/
       if(SingleRun_Flag || DoubleRun_Flag)
       {
         direction();
       }
       
    }
   
     /****************************示波器程序*********************************/
     if(SendData_Flag)
       Visual_Scope_Output(); 
     
     /**************************图像校正程序**********************************/
     if(Adjustment_Flag)
     {
       int i,j;       
       for(i=0;i<ROW;i++)
          for(j=0;j<COLUMN;j++)
             {
                if(videoData[i][j]==0xff)
                   {
                   LPLD_UART_PutChar(UART3,videoData[i][j]-1);
                   }
                else
                   {
                    LPLD_UART_PutChar(UART3,videoData[i][j]);
                   }
              }
       LPLD_UART_PutChar(UART3,0xff);
     }
    
    // LPLD_WDOG_Feed();
  }   
  
}




 
