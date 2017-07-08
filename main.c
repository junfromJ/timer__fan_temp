/***********************************************************************/
/*                                                                     */
/*  FILE        :Main.c                                                */
/*  DATE        :                                                      */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
#include <stdio.h>
#include "iodefine.h"
#include "initialize.h"
#include "lcd.h"


void main(void);

void main(void)
{
	unsigned int adc;
	unsigned int mv; //電圧値(mV)
	unsigned char LEDPIN[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02};
	unsigned int duty[] = {0, 400, 750, 1000};
	int h, m, s;
	unsigned long t;
	unsigned long time = 0;
	int fan_mode = 0;
	char fan_char[][5] = {"ｵﾌ  ", "ｼﾞｬｸ", "ﾁｭｳ ", "ｷｮｳ "};
	init();
	init_LCD();
	
	LCD_clr();
	LCD_loc(1, 1);
	// 時計表示（仮） 
	h = (int) ( (cnt%(3600L*24))/3600 );
	m = (int) ( (cnt%3600L) / 60);
	s = (int) (  cnt%60 );
	printf("%02d:%02d:%02d", h, m, s);
	
	LCD_loc(1, 2);
	printf("ﾌｧﾝﾉｼﾞｮｳﾀｲ ");
	
	while(1) {
		// LCDの右端に温度を表示する
		adc = AD_start(1);	//ANI1をA/D変換
		LCD_loc(11,1);
		mv = 1000L*adc/1023;
		wr_data((mv%1000)/100 + 0x30); 	//百の位
		wr_data((mv%100)/10 + 0x30); 	//十の位
		wr_data('.'); 					//ピリオド
		wr_data(mv%10 + 0x30); 			//一の位
		LCD_put("ﾟC");
		
		delay_ms(500);
		
		switch(~P5 & 0x0F) {
			case 1:
				fan_mode = 0;
				TT0L_bit.no0 = 1;
				break;
			case 2:
				fan_mode = 1;
				TS0L_bit.no0 = 1;
				t = 62500-1;
				break;
			case 4:
				fan_mode = 2;
				t = 42500-1;
				TS0L_bit.no0 = 1;
				break;
			case 8:
				TS0L_bit.no0 = 1;
				t = 22500-1;
				fan_mode = 3;
				break;
			default:
				break;
		}
		TDR00 = t;
		LCD_loc(12, 2);
		printf("%s", fan_char[fan_mode]);
		TDR03 = duty[fan_mode];
		
		if(cnt==14) {
			cnt=0;
			P7 = ~LEDPIN[cnt];
		} else {
			P7 = ~LEDPIN[cnt];
		}
		
	}
}
