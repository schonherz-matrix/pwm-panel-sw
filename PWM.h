		array	szint_r,8,0	;pwm interval (EZ A SORREND MARAD!)
		array	szint_g,8,0	;pwm interval (EZ A SORREND MARAD!)
		array	szint_b,8,0	;pwm interval (EZ A SORREND MARAD!)
		array	fenyero,12,0	;3 bit fenyero (rgb*4pixel)
		byte	pwm_cnt,0
		byte	pwm_cnt_shift,0

PWM_SHIFT	=	64/4

#define		PORT_P0R	PORTA
#define		PORT_P0G	PORTA
#define		PORT_P0B	PORTA

#define		PORT_P1R	PORTA
#define		PORT_P1G	PORTA
#define		PORT_P1B	PORTA

#define		PORT_P2R	PORTC
#define		PORT_P2G	PORTC
#define		PORT_P2B	PORTC

#define		PORT_P3R	PORTC
#define		PORT_P3G	PORTC
#define		PORT_P3B	PORTC

#define		BIT_P0R		0
#define		BIT_P0G		2
#define		BIT_P0B		1

#define		BIT_P1R		5
#define		BIT_P1G		4
#define		BIT_P1B		3

#define		BIT_P2R		0
#define		BIT_P2G		5
#define		BIT_P2B		4

#define		BIT_P3R		3
#define		BIT_P3G		2
#define		BIT_P3B		1



