;
; cronometer_teste.asm
;
; Created: 17/05/2024 20:55:26
; Author : stefa
;
	rjmp start
	.include "my_lib.inc"

start:
	call	config
	
the_wait:
	call	transmit_digits			// we use a function to save space. (a macro would be called twice, ocuupying twice the size!)

	store_io	TCCR0B, 0x00		// no clock connected (its stopped)

	// S1 (if pressed, it will go to loop)
	sbic	PINB, 4
	rjmp	loop 
	rjmp	the_wait

loop:
	call start_count

	// S2 (if pressed, it will stop the timer)
	sbic	PINB, 5
	rjmp	the_wait

	call	transmit_digits
	
	rjmp	loop

num_table:
	.db		0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F		// 0-9 sem DP
num_table_dp:
	.db		0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF		// 0-9 com DP
