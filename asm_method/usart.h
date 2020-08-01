		byte	RX_TMP,0
		byte	usart_rx_mode,0
			;0 - uninitialized, wait for receiving USART_INIT_MARKER
			;1 - normal mode
		byte	usart_rx_id,0
		byte	usart_rx_fenyero,0
		byte	usart_rx_fenyero_tmp,0
		byte	usart_rx_ph_cnt,0
		byte	fenyero_szin,0
		byte	usart_status,0


#define		usart_sendVersion	usart_status,0
#define		usart_sendAck		usart_status,1

VERSION_TEXT_1		=	'v'
VERSION_TEXT_2		=	'1'
VERSION_TEXT_3		=	'.'
VERSION_TEXT_4		=	'0'

USART_INIT_MARKER	=	0xF0
USART_RX_ACK_MARKER	=	0xF5
USART_TX_ACK_BASE	=	'A'
USART_CONFIG_MARKER	=	14


TXSTA_INIT	=	B'00100100'
;		bit 7 CSRC: Clock Source Select bit
;			Asynchronous mode:
;		-->		Don’t care
;			Synchronous mode:
;				1 = Master mode (clock generated internally from BRG)
;				0 = Slave mode (clock from external source)
;		bit 6 TX9: 9-bit Transmit Enable bit
;			1 = Selects 9-bit transmission
;		-->	0 = Selects 8-bit transmission
;		bit 5 TXEN: Transmit Enable bit(1)
;		-->	1 = Transmit enabled
;			0 = Transmit disabled
;		bit 4 SYNC: EUSART Mode Select bit
;			1 = Synchronous mode
;		-->	0 = Asynchronous mode
;		bit 3 SENDB: Send Break Character bit
;			Asynchronous mode:
;				1 = Send Sync Break on next transmission (cleared by hardware upon completion)
;		-->		0 = Sync Break transmission completed
;			Synchronous mode:
;				Don’t care
;		bit 2 BRGH: High Baud Rate Select bit
;			Asynchronous mode:
;		-->		1 = High speed
;				0 = Low speed
;			Synchronous mode:
;				Unused in this mode
;		bit 1 TRMT: Transmit Shift Register Status bit
;			1 = TSR empty
;		X	0 = TSR full
;		bit 0 TX9D: Ninth bit of Transmit Data
;		X	Can be address/data bit or a parity bit.


RCSTA_INIT	=	B'10010000'
;		bit 7 SPEN: Serial Port Enable bit
;		-->	1 = Serial port enabled (configures RX/DT and TX/CK pins as serial port pins)
;			0 = Serial port disabled (held in Reset)
;		bit 6 RX9: 9-bit Receive Enable bit
;			1 = Selects 9-bit reception
;		-->	0 = Selects 8-bit reception
;		bit 5 SREN: Single Receive Enable bit
;			Asynchronous mode:
;		-->		Don’t care
;			Synchronous mode – Master:
;				1 = Enables single receive
;				0 = Disables single receive
;				This bit is cleared after reception is complete.
;			Synchronous mode – Slave
;				Don’t care
;		bit 4 CREN: Continuous Receive Enable bit
;			Asynchronous mode:
;		-->		1 = Enables receiver
;				0 = Disables receiver
;			Synchronous mode:
;				1 = Enables continuous receive until enable bit CREN is cleared (CREN overrides SREN)
;				0 = Disables continuous receive
;		bit 3 ADDEN: Address Detect Enable bit
;			Asynchronous mode 9-bit (RX9 = 1):
;				1 = Enables address detection, enable interrupt and load the receive buffer when RSR<8> is set
;				0 = Disables address detection, all bytes are received and ninth bit can be used as parity bit
;			Asynchronous mode 8-bit (RX9 = 0):
;		-->		Don’t care
;		bit 2 FERR: Framing Error bit
;			1 = Framing error (can be updated by reading RCREG register and receive next valid byte)
;		X	0 = No framing error
;		bit 1 OERR: Overrun Error bit
;			1 = Overrun error (can be cleared by clearing bit CREN)
;		X	0 = No overrun error
;		bit 0 RX9D: Ninth bit of Received Data
;		X	This can be address/data bit or a parity bit and must be calculated by user firmware.


BAUDCTL_INIT		=	B'00001000'
;		bit 7 ABDOVF: Auto-Baud Detect Overflow bit
;			Asynchronous mode:
;				1 = Auto-baud timer overflowed
;		X		0 = Auto-baud timer did not overflow
;			Synchronous mode:
;				Don’t care
;		bit 6 RCIDL: Receive Idle Flag bit
;			Asynchronous mode:
;				1 = Receiver is Idle
;		X		0 = Start bit has been received and the receiver is receiving
;			Synchronous mode:
;				Don’t care
;		bit 5 Unimplemented: Read as ‘0’
;		bit 4 SCKP: Synchronous Clock Polarity Select bit
;			Asynchronous mode:
;				1 = Transmit inverted data to the RB7/TX/CK pin
;		-->		0 = Transmit non-inverted data to the RB7/TX/CK pin
;			Synchronous mode:
;				1 = Data is clocked on rising edge of the clock
;				0 = Data is clocked on falling edge of the clock
;		bit 3 BRG16: 16-bit Baud Rate Generator bit
;		-->	1 = 16-bit Baud Rate Generator is used
;			0 = 8-bit Baud Rate Generator is used
;		bit 2 Unimplemented: Read as ‘0’
;		bit 1 WUE: Wake-up Enable bit
;			Asynchronous mode:
;				1 = Receiver is waiting for a falling edge. No character will be received byte RCIF will be set. WUE will
;					automatically clear after RCIF is set.
;		-->		0 = Receiver is operating normally
;			Synchronous mode:
;				Don’t care
;		bit 0 ABDEN: Auto-Baud Detect Enable bit
;			Asynchronous mode:
;				1 = Auto-Baud Detect mode is enabled (clears when auto-baud is complete)
;		-->		0 = Auto-Baud Detect mode is disabled
;			Synchronous mode:
;				Don’t care


BAUD_REG_INIT	=	31
SPBRG_INIT	=	LOW(BAUD_REG_INIT)
SPBRGH_INIT	=	HIGH(BAUD_REG_INIT)


; TODO
;	if (TXSTA_INIT & (1<<SYNC)) > 0
;		#define alma	0x12
;	else
;		#define	alma	0x23
;	endif

