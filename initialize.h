#ifndef _initialize
#define _initialize


#define TM  1545
#define S1 P5_bit.no0
#define S2 P5_bit.no1
#define S3 P5_bit.no2
#define S4 P5_bit.no3
#define ON 0
#define OFF 1

void delay_ms(unsigned int ms);
void init(void);
unsigned int AD_start(char ch);
/*
void adc_put(unsigned int adc_num);
void adc_v_put(unsigned int adc_num);
*/
#pragma interrupt inter_tmr (vect = INTTM00)

extern unsigned char cnt;

#endif