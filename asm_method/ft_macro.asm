#define		sz	btfss	STATUS,Z
#define		snz	btfsc	STATUS,Z
#define		sc	btfss	STATUS,C
#define		snc	btfsc	STATUS,C
#define		tstw	xorlw	0

;#define		setz	bsf	STATUS,Z,0
;#define		clrz	bcf	STATUS,Z,0
;#define		setc	bsf	STATUS,C,0
;#define		clrc	bcf	STATUS,C,0


;Error 207 - Found label after column 1

byte	macro	nev,bank
	errorlevel -207

	if bank== 0
		cblock	_RAM_B0
nev	
		endc
		_RAM_B0++
	endif		
	if bank== 1
		cblock	_RAM_B1
nev	
		endc
		_RAM_B1++
	endif
	if bank== 2
		cblock	_RAM_B2
nev	
		endc
		_RAM_B2++
	endif 
	if bank== 3
		cblock	_RAM_B3
nev	
		endc
		_RAM_B3++
	endif
	errorlevel +207
	endm

word	macro	nev,bank
	errorlevel -207

	if bank== 0
		cblock	_RAM_B0
nev:2	
		endc
		_RAM_B0+=2
	endif		
	if bank== 1
		cblock	_RAM_B1
nev:2
		endc
		_RAM_B1+=2
	endif
	if bank== 2
		cblock	_RAM_B2
nev:2
		endc
		_RAM_B2+=2
	endif 
	if bank== 3
		cblock	_RAM_B3
nev:2
		endc
		_RAM_B3+=2
	endif
	errorlevel +207
	endm

array	macro	nev,length,bank
	errorlevel -207
	if bank== 0

		cblock	_RAM_B0
nev:length
		endc
		_RAM_B0+=length
	endif		
	if bank== 1
		cblock	_RAM_B1
nev:length
		endc
		_RAM_B1+=length
	endif
	if bank== 2
		cblock	_RAM_B2
nev:length
		endc
		_RAM_B2+=length
	endif 
	if bank== 3
		cblock	_RAM_B3
nev:length
		endc
		_RAM_B3+=length
	endif
	errorlevel +207
	endm







TMR_DEC         macro   timer
	NOEXPAND
                tstf  timer
		sz
                decf    timer,f
	EXPAND
                endm


TMR_DEC_FSR     macro   timer
	NOEXPAND
		lfsr	FSR1,timer
                tstfsz  INDF1
                decf    INDF1,f
	EXPAND
                endm


SUB16	macro	Fa,Fb	;Fa-Fb->Fa
	NOEXPAND
	errorlevel -302
	if high(Fb)!= 0
		banksel	Fb
	endif
		movf	Fb,w

	if high(Fa)!=high(Fb)
		banksel	Fa
	endif
		subwf	Fa,f
		sc
		decf	Fa+1,f

	if high(Fa)!=high(Fb)
		banksel	Fb
	endif
		movf	Fb+1,w

	if high(Fa)!=high(Fb)
		banksel	Fa
	endif
		subwf	Fa+1,f
	if high(Fa)!= 0
		banksel	0
	endif
	errorlevel +302
	EXPAND
	endm	


bank0 	macro
	NOEXPAND
		bcf	STATUS,RP0
		bcf	STATUS,RP1
	EXPAND
	endm
bank1	macro	
	NOEXPAND
		bsf	STATUS,RP0
		bcf	STATUS,RP1
	EXPAND
	endm
bank2	macro	
	NOEXPAND
		bcf	STATUS,RP0
		bsf	STATUS,RP1
	EXPAND
	endm
bank3	macro	
	NOEXPAND
		bsf	STATUS,RP0
		bsf	STATUS,RP1
	EXPAND
	endm





