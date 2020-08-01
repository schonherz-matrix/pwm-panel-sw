#include "main.h"

//#BYTE WPUB = 0x95

char fazis[12];
char cnt = 0;

// VER1 max: 0xF
#define VER1 0x02
// VER2 max: 0x7
#define VER2 0x01

//lookup tables for color brightness
//each byte has value between 0-63
char ph_r[8];
char ph_g[8];
char ph_b[8];
//counter used for indexing color level in config mode
char ph_cnt = 0;

//side, according to jumper
//0-left; 1-right
char oldal = 0; 

//uart mode
//0: uninitialized
//1: config mode
//2: normal mode
char uartact = 0;
volatile unsigned int16 send_cnt = 0;
volatile int8 rest_cause = 0;

/*
// ORIGINAL LOOKUP TABLE
#define CH0 2
#define CH1 0
#define CH2 1
#define CH3 3
#define CH4 4
#define CH5 5
#define CH6 6
#define CH7 11
#define CH8 10
#define CH9 9
#define CH10 7
#define CH11 8
*/

// UPPER RED-BLUE SWAPPED LOOKUP TABLE
#define CH0 0
#define CH1 2
#define CH2 1
#define CH3 5
#define CH4 4
#define CH5 3
#define CH6 6
#define CH7 11
#define CH8 10
#define CH9 9
#define CH10 7
#define CH11 8

/*
// ORIGINAL CHANNEL ID
#define PCH0 ph_b
#define PCH1 ph_r
#define PCH2 ph_g
#define PCH3 ph_r
#define PCH4 ph_g
#define PCH5 ph_b
#define PCH6 ph_r
#define PCH7 ph_b
#define PCH8 ph_g
#define PCH9 ph_r
#define PCH10 ph_g
#define PCH11 ph_b
*/
// UPPER RED-BLUE SWAPPED LOOKUP TABLE
#define PCH0 ph_r
#define PCH1 ph_b
#define PCH2 ph_g
#define PCH3 ph_b
#define PCH4 ph_g
#define PCH5 ph_r
#define PCH6 ph_r
#define PCH7 ph_b
#define PCH8 ph_g
#define PCH9 ph_r
#define PCH10 ph_g
#define PCH11 ph_b

void initPhases()
{
	ph_r[0] = 0;
	ph_r[1] = 1;
	ph_r[2] = 2;
	ph_r[3] = 4;
	ph_r[4] = 8;
	ph_r[5] = 16;
	ph_r[6] = 32;
	ph_r[7] = 63;
	ph_g[0] = 0;
	ph_g[1] = 1;
	ph_g[2] = 2;
	ph_g[3] = 4;
	ph_g[4] = 8;
	ph_g[5] = 16;
	ph_g[6] = 32;
	ph_g[7] = 63;
	ph_b[0] = 0;
	ph_b[1] = 1;
	ph_b[2] = 2;
	ph_b[3] = 4;
	ph_b[4] = 8;
	ph_b[5] = 16;
	ph_b[6] = 32;
	ph_b[7] = 63;
}

#int_TIMER0
void OLD_TIMER_isr(void)
{
	//Watchdog timer reset
	restart_wdt();
	char i = 0;
	char out1 = 0;
	char out2 = 0;

	//Software PWM for each channel
	// Channel 0
	if( PCH0[fazis[CH0]] > cnt) { out1 = (out1>>1)|0x80; } else { out1 = (out1>>1); }
	// Channel 1
	if( PCH1[fazis[CH1]] > cnt) { out1 = (out1>>1)|0x80; } else { out1 = (out1>>1); }
	// Channel 2
	if( PCH2[fazis[CH2]] > cnt) { out1 = (out1>>1)|0x80; } else { out1 = (out1>>1); }
	// Channel 3
	if( PCH3[fazis[CH3]] > cnt) { out1 = (out1>>1)|0x80; } else { out1 = (out1>>1); }
	// Channel 4
	if( PCH4[fazis[CH4]] > cnt) { out1 = (out1>>1)|0x80; } else { out1 = (out1>>1); }
	// Channel 5
	if( PCH5[fazis[CH5]] > cnt) { out1 = (out1>>1)|0x80; } else { out1 = (out1>>1); }
	
	// Channel 6
	if( PCH6[fazis[CH6]] > cnt) { out2 = (out2>>1)|0x80; } else { out2 = (out2>>1); }
	// Channel 7
	if( PCH7[fazis[CH7]] > cnt) { out2 = (out2>>1)|0x80; } else { out2 = (out2>>1); }
	// Channel 8
	if( PCH8[fazis[CH8]] > cnt) { out2 = (out2>>1)|0x80; } else { out2 = (out2>>1); }
	// Channel 9
	if( PCH9[fazis[CH9]] > cnt) { out2 = (out2>>1)|0x80; } else { out2 = (out2>>1); }
	// Channel 10
	if( PCH10[fazis[CH10]] > cnt) { out2 = (out2>>1)|0x80; } else { out2 = (out2>>1); }
	// Channel 11
	if( PCH11[fazis[CH11]] > cnt) { out2 = (out2>>1)|0x80; } else { out2 = (out2>>1); }

	out1 = (out1>>2);
	out2 = (out2>>2);

	//set output ports
	output_a(out1);
	output_c(out2);

	//Counter increment for next cycle
	cnt++;
	if(cnt >= 63)
	{
		cnt = 0;
	}
	//heartbeat counter increment
	if (send_cnt < 16000)
	{
		send_cnt++;
	}
}

//IT Received data on UART
#int_RDA
void OLD_RDA_isr(void)
{
	disable_interrupts(GLOBAL);

	char c;

	c = getc();

	int8 id = (c>>4);
	if(id == 15)
	{
		//if 
		if((c & 0x0F) == (oldal))
		{
			uartact = 1;
		} else {
			uartact = 0;
		}
	}
	else if(id == 14)
	{
		//enter config mode
		uartact = 2;
		ph_cnt = 0;
	}
	if(id == 13)
	{
		//return version and side
		putc(((VER1&0x0F)<<4) | ((VER2&0x07)<<1) | (oldal&0x01));
	}
	else //id <= 12
	{
		//config/normal mode
		if((uartact == 2))
		{
			//config mode
			switch(ph_cnt)
			{
				case 0: ph_r[1] = c; break;
				case 1: ph_r[2] = c; break;
				case 2: ph_r[3] = c; break;
				case 3: ph_r[4] = c; break;
				case 4: ph_r[5] = c; break;
				case 5: ph_r[6] = c; break;
				case 6: ph_r[7] = c; break;
				case 7: ph_g[1] = c; break;
				case 8: ph_g[2] = c; break;
				case 9: ph_g[3] = c; break;
				case 10: ph_g[4] = c; break;
				case 11: ph_g[5] = c; break;
				case 12: ph_g[6] = c; break;
				case 13: ph_g[7] = c; break;
				case 14: ph_b[1] = c; break;
				case 15: ph_b[2] = c; break;
				case 16: ph_b[3] = c; break;
				case 17: ph_b[4] = c; break;
				case 18: ph_b[5] = c; break;
				case 19: ph_b[6] = c; break;
				case 20: ph_b[7] = c; break;
			}
			ph_cnt++;
		}
		else if (uartact == 1)
		{
			//normal mode, set color accordingly
			if(id < 12)
			{
				fazis[id] = (c & 0x0F);
			}
		}
	}

	enable_interrupts(GLOBAL);
}

void main()
{
	int8 i;
	restart_wdt();
	rest_cause = restart_cause();
	switch (rest_cause)
	{
		case WDT_TIMEOUT: rest_cause = 1; break;
		case MCLR_FROM_RUN: rest_cause = 2; break;
		case BROWNOUT_RESTART: rest_cause = 3; break;
		default: rest_cause = 0; break; //normal
	}

	setup_adc_ports(NO_ANALOGS|VSS_VDD);
	setup_adc(ADC_CLOCK_DIV_2);
	setup_spi(SPI_SS_DISABLED);
	setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
	setup_wdt(WDT_2304MS);
	setup_timer_1(T1_DISABLED);
	setup_timer_2(T2_DISABLED,0,1);
	enable_interrupts(INT_TIMER0);
	enable_interrupts(INT_RDA);

	initPhases();

	
	set_TRIS_A(0x00); // 0=kimenet, 1=bemenet
	set_TRIS_B(0b11111111); // 0=kimenet, 1=bemenet
	set_TRIS_C(0x00); // 0=kimenet, 1=bemenet

	// RB5 jumperes (ablak selector)
	char tempb = input_b();

	if(tempb & 0b00100000) {
		oldal = 0;
	} else {
		oldal = 1;
	}

	//reset all outputs (0 for leds)
	output_a(255);
	output_c(255);

	//TODO no longer needed
	putc('\n');
	putc('\n');
	putc('g');
	putc('e');
	putc('r');
	putc('p');
	putc('e');
	putc('t');
	putc('y');
	putc('a');

	//init leds
	for(i = 0; i<12; i++)
	{
		fazis[i] = 0;//(i/3) + 4;
	}

	
	while(1)
	{
		disable_interrupts(GLOBAL);
		if (send_cnt >= 15625)//15625=1sec
		{
			send_cnt = 0;
			putc(0x80 | rest_cause);
		}
		enable_interrupts(GLOBAL);
	}
}
