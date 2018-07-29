
USART_Init ;(see usart.h)
		BANKSEL	TXSTA
		movlw	TXSTA_INIT
		movwf	TXSTA
		BANKSEL	RCSTA
		movlw	RCSTA_INIT
		movwf	RCSTA
		BANKSEL	BAUDCTL
		movlw	BAUDCTL_INIT
		movwf	BAUDCTL
		BANKSEL	SPBRG
		movlw	SPBRG_INIT
		movwf	SPBRG
		BANKSEL	SPBRGH
		movlw	SPBRGH_INIT
		movwf	SPBRGH
		;RC7-RX and RC6-TX
		BANKSEL	PIE1
		bcf	PIE1,RCIE	;enable RX interrupt

		bank0
		clrf	RX_TMP
		clrf	usart_rx_mode
		clrf	usart_rx_id
		clrf	usart_rx_fenyero
		clrf	usart_rx_ph_cnt
		clrf	fenyero_szin
		clrf	usart_status
		return

USART_Recv
		movfw	RCREG		;Read uart buffer
		movwf	RX_TMP		;store in temp register

		andlw	0x07
		movwf	usart_rx_fenyero;alsó 3 bit: fenyero
		swapf	RX_TMP,w
		andlw	0x0F
		movwf	usart_rx_id	;felsõ 4 bit: id

		movlw	USART_RX_ACK_MARKER
		xorwf	RX_TMP,w
		snz
		bsf	usart_sendAck

;switch (usart_rx_mode)
USART_Recv_Modes
		movlw	0
		xorwf	usart_rx_mode,w
		snz
		goto	USART_Mode_0	;case 0: uninitialized
		movlw	1
		xorwf	usart_rx_mode,w
		snz
		goto	USART_Mode_1	;case 1: normal mode
		movlw	2
		xorwf	usart_rx_mode,w
		snz
		goto	USART_Mode_2

		clrf	usart_rx_mode
		return
;---------------
USART_Mode_0
		movlw	USART_INIT_MARKER
		xorwf	RX_TMP,w
		sz
		return
		bsf	usart_sendVersion
		movlw	1
		movwf	usart_rx_mode
		return
;---------------
USART_Mode_1
		movlw	USART_CONFIG_MARKER
		xorwf	usart_rx_id,w
		snz
		goto	USART_Mode_1_ToConfig	;usart_rx_id = 14

		movlw	12
		subwf	usart_rx_id,w
		sc
		goto	USART_Mode_1_Fenyero	;usart_rx_id < 12
		return
;-----
USART_Mode_1_ToConfig
		movlw	2
		movwf	usart_rx_mode		;move to config mode
		clrf	usart_rx_ph_cnt
		return
;-----
USART_Mode_1_Fenyero
		movfw	usart_rx_id		;fenyero_szin: szint_r v szint_g v szint_b
		call	PWM_Szin_Table

		addwf	usart_rx_fenyero,w
		movwf	FSR
		movfw	INDF
		movwf	usart_rx_fenyero	;itt van a tényleges pwm fényerõ

		movlw	fenyero
		addwf	usart_rx_id,w
		movwf	FSR
		movfw	usart_rx_fenyero
		movwf	INDF

		return
;---------------
USART_Mode_2
		movlw	15
		xorwf	usart_rx_id,w	;usart_rx_id = 15?
		snz
		goto	USART_Mode_2_To1;igen, vissza normál módba

		movlw	24
		subwf	usart_rx_ph_cnt,w
		sc
		return			;usart_rx_ph_cnt >= 24 -> return

		movlw	szint_r		;usart_rx_ph_cnt < 24
		movwf	FSR		;load szint_r
		movfw	usart_rx_ph_cnt
		addwf	FSR,f		;select szint_r[usart_rx_ph_cnt]
		movfw	RX_TMP
		movwf	INDF		;szint_r[usart_rx_ph_cnt] = Received byte

		incf	usart_rx_ph_cnt,f
		return
;-----
USART_Mode_2_To1
		movlw	1
		movwf	usart_rx_mode
		return


;--------------------------------------------------------------------------------

USART_Task
		btfss	PIR1,TXIF
		return
		btfsc	usart_sendVersion
		goto	USART_Task_Ver
		btfsc	usart_sendAck
		goto	USART_Task_Ack
		return

USART_Task_Ver
		bcf	usart_sendVersion
		movlw	0
		call	EE_Read_Data
		movwf	TXREG
		nop
		nop
USART_Task_Ver1
		btfss	PIR1,TXIF
		goto	USART_Task_Ver1
		movlw	1
		call	EE_Read_Data
		movwf	TXREG
		nop
		nop
USART_Task_Ver2
		btfss	PIR1,TXIF
		goto	USART_Task_Ver2
		movlw	2
		call	EE_Read_Data
		movwf	TXREG
		nop
		nop
USART_Task_Ver3
		btfss	PIR1,TXIF
		goto	USART_Task_Ver3
		movlw	3
		call	EE_Read_Data
		movwf	TXREG
		nop
		nop
		return

USART_Task_Ack
		bcf	usart_sendAck
		movlw	USART_TX_ACK_BASE
		addwf	usart_rx_mode,w
		movwf	TXREG
		return


