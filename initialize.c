#include "iodefine.h"
#include "initialize.h"
#include "lcd.h"

unsigned char cnt = 0;

void init(void) 
{
  PM5 = 0x0F;
  PM7 = 0x00;
  P5 = 0x00;
  P7 = 0xFF;
  
  // PWM初期設定
  PM3_bit.no1 = 0;
  TAU0EN = 1;
  // ch2設定
  TPS0 |= 0x0050;
  TMR02 = 0x8801;
  TDR02 = 999;
  // ch3設定
  TMR03 = 0x8409;
  TDR03 = 0;
  TOM0 = 0x0008;
  TOE0 = 0x0008;
  TS0L_bit.no2 = 1;
  TS0L_bit.no3 = 1;
  // LCDの設定
	
  PM6 = 0x00;
  PM1 = 0x00;
  P6 = 0x00;
  P1 = 0x00;
  
  // ADコンバータの設定
  PM2 = 0x07;
  ADCEN = 1;
  ADPC = 0x04;
  ADM0 = 0x08;
  ADM1 = 0x20;
  ADM2 = 0x40;
  ADS = 0x00;
  ADCE = 1;
	
	// TM00インターバル割り込み設定
  __DI();
  TMMK00 = 0;
  TAU0EN = 1;
  TPS0 = 9;
  TDR00 = 62500 - 1;
  TMR00 = 0;
  TMPR000 = 0;
  TMPR100 = 0;
  //TS0L_bit.no0 = 1;
  //TT0L_bit.no0 = 1;
  __EI();
  // もうOK
	
}

void delay_ms(unsigned int ms)
{
  volatile unsigned long i,j;  
  for(i=0;i<ms;i++) {
	  for(j=0;j<TM;j++);
  }
}

unsigned int AD_start(char ch)
{
	ADS = ch & 0x1F;
	delay_ms(1);
	ADCS = 1;
	while(ADCS) continue;
	return (ADCR >> 6);
}

void adc_put(unsigned int adc_num)
{
	// adc_num は1023など4桁までの整数
	wr_data((adc_num / 1000)+0x30);
  wr_data(((adc_num%1000)/100)+0x30);
	wr_data(((adc_num%100)/10)+0x30);
	wr_data((adc_num%10)+0x30);
}

void adc_v_put(unsigned int adc_num)
{
	unsigned int mv;
	mv = (float)5000 * adc_num / 1023;
	// mvは[mv]で考える
	wr_data((mv / 1000)+0x30);
	wr_data('.');
  wr_data(((mv%1000)/100)+0x30);
	wr_data(((mv%100)/10)+0x30);
	wr_data((mv%10)+0x30);
	LCD_put("[V]");
}

void inter_tmr(void)
{
	cnt++;
}
