;******************************************************************************
;*									      *
;*									      *
;******************************************************************************
;
;
;-------------- Device data ---------------------------------------------------
;
;
		list	p=PIC16f882, r=DEC
		#include <p16f882.inc>
		#include "ft_macro.asm"
;

;------------------------------------------------------------------------------
;-------------- RAM PARAMÉTEREK -------------------------------------------
;------------------------------------------------------------------------------
RAM_B0_BEG	=	0x0020		;Bank0 RAM inicializalasok kezdete
RAM_B0_END	=	0x006F		;Bank0 RAM inicializalasok vege
RAM_B1_BEG	=	0x00A0		;Bank1 RAM inicializalasok kezdete
RAM_B1_END	=	0x00BF		;Bank1 RAM inicializalasok vege

        variable        _RAM_B0=RAM_B0_BEG
        variable        _RAM_B1=RAM_B1_BEG


;******************************************************************************
;*									      *
;*									      *
;*									      *
;******************************************************************************
;-------------- IT rutinhoz szukseges mentesi regiszterek
	cblock 0x70	; put these up in unbanked RAM
W_Save
STATUS_Save
PCLATH_Save
	endc


SYSTEM_TIMER_10MS_INIT	=	156
TIMER0_CYCLE		=	256		;Instruction cycle per Interrupt
TMR0_INIT		=	256- TIMER0_CYCLE/2 +8

	byte	SystemTimer10ms,0

;******************************************************************************
;*									      *
;*		RESET							      *
;*									      *
;******************************************************************************

		org	0x0
		nop
		nop
		goto	Start_Reset


;******************************************************************************
;*									      *
;*		IT ADDRESS - IT ROUTINE					      *
;*									      *
;******************************************************************************
		org	0x4
ISR
		movwf	W_Save		;Copy W to W_Save register
		swapf	STATUS,w	;Swap status to be saved into W
					;Swaps are used because they do not affect the status bits
		movwf	STATUS_Save	;Save status to bank zero STATUS_Save register

		movf	PCLATH,w	;save PCLATH
		movwf	PCLATH_Save
;-----ISR------
		bank0

		btfss	INTCON,T0IF	;TMR0 okozta az interruptot?
		goto	IT_USART	;nem
		bcf	INTCON,T0IF	;igen, clear TMR0 Interrupt flag
		movlw	TMR0_INIT	;init TMR0
		movwf	TMR0
;---TIMER0	
		TMR_DEC	SystemTimer10ms	;decrement timer
		call	PWM_Control	;PWM control

IT_USART
		btfsc	PIR1,RCIF	;Usart-on jött adat?
		call	USART_Recv	;igen, feldolgozzuk

;-----ISR-END--
		movf	PCLATH_Save,w	;Restore PCLATH
		movwf	PCLATH

		swapf	STATUS_Save,w	;Swap STATUS_Save register into W
					;(sets bank to original state)
		movwf	STATUS		;Move W into STATUS register
		swapf	W_Save,f	;Swap W_Save
		swapf	W_Save,w	;Swap W_Save into W
		retfie
;******************************************************************************
;*									      *
;*		TABLES					 	      *
;*									      *
;******************************************************************************
PWM_Szin_Table
		andlw		0x0F
		addwf		PCL,f

		retlw		szint_r
		retlw		szint_r
		retlw		szint_r
		retlw		szint_r
		retlw		szint_r
		retlw		szint_r
		retlw		szint_r
		retlw		szint_r
		retlw		szint_r
		retlw		szint_r
		retlw		szint_r
		retlw		szint_r
		;Nem szükséges, de nehogy túlfusson
		retlw		szint_r
		retlw		szint_r
		retlw		szint_r
		retlw		szint_r
;******************************************************************************
;*									      *
;*		INCLUDES					 	      *
;*									      *
;******************************************************************************
		#include	"main.h"
		#include	"usart.h"
		#include	"PWM.h"
;------------------------------------------
		#include	"PWM.asm"
		#include	"usart.asm"
;******************************************************************************
;*									      *
;*		INITIALIZING PERIPHERALS			 	      *
;*									      *
;******************************************************************************
Start_Reset
		clrwdt

		BANKSEL	PORTA	;select bank
		clrf	PORTA	;clear all ports
		clrf	PORTB
		clrf	PORTC

		BANKSEL	TRISA
		clrf	TRISA	;output
		movlw	0xFF
		movwf	TRISB	;input
		movlw	0x80
		movwf	TRISC	;output (except RC7 -> USART RX, but RCSTA,SPEN configs that)

		BANKSEL	ANSEL	;analóg: nem kell semmi
		clrf	ANSEL
		clrf	ANSELH

		BANKSEL	OPTION_REG
		movlw	OPTION_REG_INIT	;see main.h
		movwf	OPTION_REG

#ifndef	__DEBUG	;Debug-ban nincs watchdog
		BANKSEL	WDTCON		;Watchdog init
		movlw	WDTCON_INIT
		movwf	WDTCON
#endif
		BANKSEL	TMR0
		movlw	TMR0_INIT	;Timer0 init
		movwf	TMR0
		bcf	INTCON,T0IF	;Clear Timer0 IT flag
		bsf	INTCON,T0IE	;Enable Timer0 Interrupt


		bank0
;-------------- GENERAL INITS
		clrf	SystemTimer10ms

		call	USART_Init
		call	Init_PWM_Control
;--------------

		EN_PERIPH_IT	;Enable peripheral interrupts
		EN_GLOBAL_IT	;Enable global interrupts

		goto	Main_Loop


;******************************************************************************
;*									      *
;*		MAIN PROGRAM LOOP					      *
;*									      *
;******************************************************************************
Main_Loop
		clrwdt		;clear watchdog timer

		tstf	SystemTimer10ms	;timer lejárt?
		sz
		goto	Main_Loop_end		;nope, nincs tennivaló
		movlw	SYSTEM_TIMER_10MS_INIT	;igen, timer init
		movwf	SystemTimer10ms

;--------------	BEGIN 100ms tasks
		movlw	0
		xorwf	usart_rx_mode,w
		sz
		goto	Main_Loop_end
		nop
		movlw	0x55
		btfss	PIR1,TXIF
		goto	Main_Loop_end
		movwf	TXREG
		nop
;-------------- END 100ms

Main_Loop_end
		bank0
		call	USART_Task

		goto	Main_Loop
		goto	Main_Loop
;******************************************************************************
;*									      *
;*		READ EEPROM DATA					      *
;*									      *
;******************************************************************************
	;parameter: WREG
EE_Read_Data
		BANKSEL	EEADR		;
;		MOVLW	DATA_EE_ADDR	;
		MOVWF	EEADR		;Data Memory
					;Address to read
		BANKSEL	EECON1		;
		BCF	EECON1,EEPGD	;Point to DATA memory
		BSF	EECON1,RD	;EE Read
		BANKSEL	EEDAT		;
		MOVF	EEDAT,W		;W = EEDAT
		bank0
		RETURN

		org	0x2100
EE_VERSION_ADDR
		de	"v1.0"




;******************************************************************************
;*   CHECK RAM USAGE                                                          *
;******************************************************************************
	if _RAM_B0>RAM_B0_END
		error	 "Az BANK0-ba szánt változók nem férnek el!!!!"
	endif
	messg	Bank0 regs: #V(_RAM_B0-RAM_B0_BEG)/#V(RAM_B0_END-RAM_B0_BEG)

	if _RAM_B1>RAM_B1_END
		error	 "Az BANK1-ba szánt változók nem férnek el!!!!"
	endif
	messg	Bank1 regs: #V(_RAM_B1-RAM_B1_BEG)/#V(RAM_B1_END-RAM_B1_BEG)

	end