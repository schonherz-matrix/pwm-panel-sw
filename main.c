#include "main.h"

char fazis[12];
char cnt = 0;

// VER1 max: 0xF
#define VER1 0x03
// VER2 max: 0x7
#define VER2 0x00

#define CNT_1SEC	(15625)

//lookup tables for color brightness
//each byte has value between 0-63
char ph_r[8];
char ph_g[8];
char ph_b[8];
//counter used for indexing color level in config mode
char ph_cnt = 0;

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

//colors' brightness in PWM cycles (0-63)
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

//Timer interrupt for SW PWM
//Called every 256 cycles
#int_TIMER0
void OLD_TIMER_isr(void)
{
	clear_interrupt(INT_TIMER0);
	char i = 0;
	char out1 = 0;
	char out2 = 0;

	//Software PWM for each channel
	// Channel 0
	if( PCH0[fazis[CH0]] > cnt) { out1 |= (1<<0);}
	// Channel 1
	if( PCH1[fazis[CH1]] > cnt) { out1 |= (1<<1);}
	// Channel 2
	if( PCH2[fazis[CH2]] > cnt) { out1 |= (1<<2);}
	// Channel 3
	if( PCH3[fazis[CH3]] > cnt) { out1 |= (1<<3);}
	// Channel 4
	if( PCH4[fazis[CH4]] > cnt) { out1 |= (1<<4);}
	// Channel 5
	if( PCH5[fazis[CH5]] > cnt) { out1 |= (1<<5);}
	
	// Channel 6
	if( PCH6[fazis[CH6]] > cnt) { out2 |= (1<<0);}
	// Channel 7
	if( PCH7[fazis[CH7]] > cnt) { out2 |= (1<<1);}
	// Channel 8
	if( PCH8[fazis[CH8]] > cnt) { out2 |= (1<<2);}
	// Channel 9
	if( PCH9[fazis[CH9]] > cnt) { out2 |= (1<<3);}
	// Channel 10
	if( PCH10[fazis[CH10]] > cnt) { out2 |= (1<<4);}
	// Channel 11
	if( PCH11[fazis[CH11]] > cnt) { out2 |= (1<<5);}

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
	if (send_cnt < CNT_1SEC)
	{
		send_cnt++;
	}
}

//IT Received data on UART
#int_RDA
void OLD_RDA_isr(void)
{
	char c;

	c = getc();

	int8 id = (c>>4);
	if (c == 0xF0)
	{
		uartact = 1;
	}
	else if (c == 0xF8)
	{
		rest_cause = 0;
	}
	else if (c == 0xE0)
	{
		//enter config mode
		uartact = 2;
		ph_cnt = 0;
	}
	else if (c == 0xD0)
	{
		//return version
		putc(((VER1&0x0F)<<3) | ((VER2&0x07)));
	}
	else if (id <= 12) //id <= 12
	{
		if((uartact == 2)) //config mode
		{
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
			//ph_cnt=21 -> do nothing
			if (ph_cnt <= 20) ph_cnt++;
		}
		else if (uartact == 1)
		{
			if(id < 12) //normal mode, set color accordingly
			{
				fazis[id] = (c & 0x07);
			}
		}
	}
}

void main()
{
	int8 i;
	i = restart_cause();
	switch (i)
	{
		case BROWNOUT_RESTART: rest_cause = 1; break;
		case WDT_TIMEOUT: rest_cause = 2; break;
		case MCLR_FROM_RUN: rest_cause = 3; break;
		default: rest_cause = 4; break;
	}
	restart_wdt();

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

	//reset all outputs (0 for leds)
	output_a(0);
	output_c(0);

	//init leds
	for(i = 0; i<12; i++)
	{
		fazis[i] = 0;
	}
	putc(0x80 | rest_cause);

	while(1)
	{
		disable_interrupts(GLOBAL);
		//Watchdog timer reset
		restart_wdt();

		//send 1sec heartbeat + last restart cause on UART
		if (send_cnt == CNT_1SEC)//15625=1sec
		{
			putc(0x80 | rest_cause);
		}
		enable_interrupts(GLOBAL);
	}
}
