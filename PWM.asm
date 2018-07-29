Init_PWM_Control
		clrf	fenyero
		clrf	fenyero+1
		clrf	fenyero+2
		clrf	fenyero+3
		clrf	fenyero+4
		clrf	fenyero+5
		clrf	fenyero+6
		clrf	fenyero+7
		clrf	fenyero+8
		clrf	fenyero+9
		clrf	fenyero+10
		clrf	fenyero+11

		movlw	0x00
		movwf	szint_r
		movwf	szint_g
		movwf	szint_b
		movlw	0x01
		movwf	szint_r+1
		movwf	szint_g+1
		movwf	szint_b+1
		movlw	0x02
		movwf	szint_r+2
		movwf	szint_g+2
		movwf	szint_b+2
		movlw	0x04
		movwf	szint_r+3
		movwf	szint_g+3
		movwf	szint_b+3
		movlw	0x08
		movwf	szint_r+4
		movwf	szint_g+4
		movwf	szint_b+4
		movlw	0x10
		movwf	szint_r+5
		movwf	szint_g+5
		movwf	szint_b+5
		movlw	0x20
		movwf	szint_r+6
		movwf	szint_g+6
		movwf	szint_b+6
		movlw	0x40
		movwf	szint_r+7
		movwf	szint_g+7
		movwf	szint_b+7
			
		clrf	pwm_cnt
		clrf	pwm_cnt_shift

		return

PWM_Control
		nop
		movfw	pwm_cnt		;pwm_cnt másolása a pwm_cnt_shift-be
		movwf	pwm_cnt_shift

		bcf	STATUS,IRP		;bank0 indirect
		movlw	fenyero			;INDF->fenyero[0]
		movwf	FSR

;---Pixel-0-Red
		movfw	INDF
		subwf	pwm_cnt_shift,w		;cnt - feny[0]
		sc
		bsf	PORT_P0R,BIT_P0R	;C=0 (feny[0] > cnt)
		snc
		bcf	PORT_P0R,BIT_P0R	;C=1 (feny[0] <= cnt)
		incf	FSR,f			;INDF->fenyero[1]
;---Pixel-0-Green
		movfw	INDF
		subwf	pwm_cnt_shift,w		;cnt - feny[1]
		sc
		bsf	PORT_P0G,BIT_P0G	;C=0 (feny[1] > cnt)
		snc
		bcf	PORT_P0G,BIT_P0G	;C=1 (feny[1] <= cnt)
		incf	FSR,f			;INDF->fenyero[2]
;---Pixel-0-Blue
		movfw	INDF
		subwf	pwm_cnt_shift,w		;cnt - feny[2]
		sc
		bsf	PORT_P0B,BIT_P0B	;C=0 (feny[2] > cnt)
		snc
		bcf	PORT_P0B,BIT_P0B	;C=1 (feny[2] <= cnt)
		incf	FSR,f			;INDF->fenyero[3]
		movlw	PWM_SHIFT		;PWM counter igaítás (eltolva kapcsolgatjuk a paneleket)
		subwf	pwm_cnt_shift,f
		movlw	0x3F
		andwf	pwm_cnt_shift,f

;---Pixel-1-Red		(Pontosan ugyanaz mint feljebb, csak másik panelre)
		movfw	INDF
		subwf	pwm_cnt_shift,w
		sc
		bsf	PORT_P1R,BIT_P1R
		snc
		bcf	PORT_P1R,BIT_P1R
		incf	FSR,f
;---Pixel-1-Green	(Pontosan ugyanaz mint feljebb, csak másik panelre)
		movfw	INDF
		subwf	pwm_cnt_shift,w
		sc
		bsf	PORT_P1G,BIT_P1G
		snc
		bcf	PORT_P1G,BIT_P1G
		incf	FSR,f
;---Pixel-1-Blue	(Pontosan ugyanaz mint feljebb, csak másik panelre)
		movfw	INDF
		subwf	pwm_cnt_shift,w
		sc
		bsf	PORT_P1B,BIT_P1B
		snc
		bcf	PORT_P1B,BIT_P1B
		incf	FSR,f
		movlw	PWM_SHIFT
		subwf	pwm_cnt_shift,f
		movlw	0x3F
		andwf	pwm_cnt_shift,f

;---Pixel-2-Red		(Pontosan ugyanaz mint feljebb, csak másik panelre)
		movfw	INDF
		subwf	pwm_cnt_shift,w
		sc
		bsf	PORT_P2R,BIT_P2R
		snc
		bcf	PORT_P2R,BIT_P2R
		incf	FSR,f
;---Pixel-2-Green	(Pontosan ugyanaz mint feljebb, csak másik panelre)
		movfw	INDF
		subwf	pwm_cnt_shift,w
		sc
		bsf	PORT_P2G,BIT_P2G
		snc
		bcf	PORT_P2G,BIT_P2G
		incf	FSR,f
;---Pixel-2-Blue	(Pontosan ugyanaz mint feljebb, csak másik panelre)
		movfw	INDF
		subwf	pwm_cnt_shift,w
		sc
		bsf	PORT_P2B,BIT_P2B
		snc
		bcf	PORT_P2B,BIT_P2B
		incf	FSR,f
		movlw	PWM_SHIFT
		subwf	pwm_cnt_shift,f
		movlw	0x3F
		andwf	pwm_cnt_shift,f

;---Pixel-3-Red		(Pontosan ugyanaz mint feljebb, csak másik panelre)
		movfw	INDF
		subwf	pwm_cnt_shift,w
		sc
		bsf	PORT_P3R,BIT_P3R
		snc
		bcf	PORT_P3R,BIT_P3R
		incf	FSR,f
;---Pixel-3-Green	(Pontosan ugyanaz mint feljebb, csak másik panelre)
		movfw	INDF
		subwf	pwm_cnt_shift,w
		sc
		bsf	PORT_P3G,BIT_P3G
		snc
		bcf	PORT_P3G,BIT_P3G
		incf	FSR,f
;---Pixel-3-Blue	(Pontosan ugyanaz mint feljebb, csak másik panelre)
		movfw	INDF
		subwf	pwm_cnt_shift,w
		sc
		bsf	PORT_P3B,BIT_P3B
		snc
		bcf	PORT_P3B,BIT_P3B
		incf	FSR,f
		movlw	PWM_SHIFT
		subwf	pwm_cnt_shift,f
		movlw	0x3F
		andwf	pwm_cnt_shift,f

		incf	pwm_cnt,f
		btfsc	pwm_cnt,6
		clrf	pwm_cnt
		return









