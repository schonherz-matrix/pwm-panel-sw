; CONFIG1
; __config 0xEFFA
; __CONFIG _CONFIG1, _FOSC_HS & _WDTE_ON & _PWRTE_ON & _MCLRE_ON & _CP_OFF & _CPD_OFF & _BOREN_ON & _IESO_ON & _FCMEN_ON & _LVP_OFF & _DEBUG_OFF
; CONFIG2
; __config 0xFEFF
; __CONFIG _CONFIG2, _BOR4V_BOR21V & _WRT_OFF



;---------------Enable/Disable Interrupts
#define		EN_GLOBAL_IT		bsf	INTCON,GIE
#define		DIS_GLOBAL_IT		bcf	INTCON,GIE

#define		EN_PERIPH_IT		bsf	INTCON,PEIE
#define		DIS_PERIPH_IT		bcf	INTCON,PEIE




OPTION_REG_INIT	=	B'00000000'
;		bit 7 RBPU: PORTB Pull-up Enable bit
;			1 = PORTB pull-ups are disabled
;		-->	0 = PORTB pull-ups are enabled by individual PORT latch values
;		bit 6 INTEDG: Interrupt Edge Select bit
;			1 = Interrupt on rising edge of INT pin
;		-->	0 = Interrupt on falling edge of INT pin
;		bit 5 T0CS: TMR0 Clock Source Select bit
;			1 = Transition on T0CKI pin
;		-->	0 = Internal instruction cycle clock (FOSC/4)
;		bit 4 T0SE: TMR0 Source Edge Select bit
;			1 = Increment on high-to-low transition on T0CKI pin
;		-->	0 = Increment on low-to-high transition on T0CKI pin
;		bit 3 PSA: Prescaler Assignment bit
;			1 = Prescaler is assigned to the WDT
;		-->	0 = Prescaler is assigned to the Timer0 module
;		bit 2-0 PS<2:0>: Prescaler Rate Select bits
;			BIT VALUE	TMR0 RATE	WDT RATE
;		-->	000			1 : 2		1 : 1	
;			001			1 : 4		1 : 2	
;			010			1 : 8		1 : 4	
;			011			1 : 16		1 : 8	
;			100			1 : 32		1 : 16	
;			101			1 : 64		1 : 32	
;			110			1 : 128		1 : 64	
;			111			1 : 256		1 : 128	


WDTCON_INIT	=	B'00010101'	;enable, 1:32
;		bit 7-5 Unimplemented: Read as ‘0’
;		bit 4-1 WDTPS<3:0>: Watchdog Timer Period Select bits
;			Bit Value	= Prescale Rate
;			0000 		= 1:32
;			0001 		= 1:64
;			0010 		= 1:128
;			0011 		= 1:256
;			0100 		= 1:512 (Reset value)
;			0101 		= 1:1024
;			0110 		= 1:2048
;			0111 		= 1:4096
;			1000 		= 1:8192
;			1001 		= 1:16384
;		-->	1010 		= 1:32768
;			1011 		= 1:65536
;			1100 		= reserved
;			1101 		= reserved
;			1110 		= reserved
;			1111 		= reserved
;		bit 0 SWDTEN: Software Enable or Disable the Watchdog Timer(1)
;		-->	1 = WDT is turned on
;			0 = WDT is turned off (Reset value)






