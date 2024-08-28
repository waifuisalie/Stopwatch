/*
 * my_lib.inc
 *
 *  Created: 17/05/2024 20:57:18
 *   Author: stefa
 */ 



.macro le_tabela_R17
	push	R31
	push	R30
	push	@1
		ldi		R31, HIGH(@0*2)			// loading the position of memory I want to read
		ldi		R30, LOW(@0*2)
		add		R30, @1                // adding the index I want to select to read
		ldi		@1, 0					// for carry operation
		adc		R31, @1
		lpm		@2, Z					// finally, the result is assigned to a given register
	pop		@1
	pop		R30 
	pop		R31
.endmacro	

 .org	0x20
overflow:
	push	R16
		in	R16, SREG
		push	R16

			ldi		R16, 131			// putting 131 back in our counter so it overflows in the correct time (2ms)
			out		TCNT0, R16


			inc		R19
			cpi		R19,  5				// 5 interrupts are equal to 10ms
			breq	keep_going

			rjmp	update_done

			// 10ms have passed, time to update!
			keep_going:
 				ldi		R19, 0			// resetting our 10ms "timer"
				// D1
				inc		R23 
				cpi		R23, 10
				brlo	update_done		

				ldi		R23, 0			// resetting R23
				// D2
				inc		R22
				cpi		R22, 10
				brlo	update_done		

				ldi		R22, 0			// resetting R22
				// D3
				inc		R21
				cpi		R21, 10
				brlo	update_done

				ldi		R21, 0			// resetting R21
				// D4
				inc		R20
				cpi		R20, 6
				brlo	update_done

				ldi		R20, 0			// resetting R20
	update_done:
		pop R16
		out SREG, R16
	pop R16
	reti


.macro		 store
	push	R16
		ldi		R16, @1
		sts		@0, R16
	pop		R16
.endm

.macro		store_io
	push	R16
		ldi		R16, @1
		out		@0, R16
	pop		R16
.endm

.macro		store_io_register
	push	R16
		mov		R16, @1
		out		@0, R16
	pop		R16
.endm


.macro pausa_us
	ldi		R16,@0
	call	pausaR16us
.endmacro

.macro pausas
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
.endm

// for 30fps
.macro	pausa_8ms
	pausa_us	250
	pausa_us	250
	pausa_us	250
	pausa_us	250
	pausa_us	250
	pausa_us	250
	pausa_us	250
	pausa_us	250
	pausa_us	250
	pausa_us	250
	pausa_us	230
.endm

// for 60fps
.macro	pausa_4ms
	pausa_us	250
	pausa_us	250
	pausa_us	250
	pausa_us	250
	pausa_us	250
	pausa_us	115
.endm

// for 120fps
.macro	pausa_2ms
	pausa_us	250
	pausa_us	250
	pausa_us	182
.endm

// function made in class
pausaR16us:
	; pausa 1
	push	R16
	nop

pausaR16us_loop:
	cpi		R16,1
	breq	pausaR16us_fim
	dec		R16
	; pausa 2
	pausas
	pausas
	pausas
	pausas

	rjmp	pausaR16us_loop
pausaR16us_fim:
	pop		R16
	ret


transmit_digits:

	// D1
	sbi		PORTB, 3					// D1 = PORTB, 3; D2 = PORTB, 2; D3 = PORTB, 1; D4 = PORTB, 0
	le_tabela_R17 num_table_dp, R23, R17
	store_io_register PORTD, R17
	pausa_2ms
	cbi		PORTB, 3

	// D2
	sbi		PORTB, 2
	le_tabela_R17 num_table, R22, R17
	store_io_register PORTD, R17
	pausa_2ms
	cbi		PORTB, 2

	// D3 
	sbi		PORTB, 1
	le_tabela_R17 num_table_dp, R21, R17
	store_io_register PORTD, R17
	pausa_2ms
	cbi		PORTB, 1

	// D4
	sbi		PORTB, 0
	le_tabela_R17 num_table, R20, R17
	store_io_register PORTD, R17
	pausa_2ms
	cbi		PORTB, 0

	ret



 config:
	// setting the DDRs and PORTB
	store_io	DDRD,  0xFF
	store_io	DDRB,  0xCF
	store_io	PORTB, 0x00

	// clock stuff
	store_io	TCCR0A, 0x00			// we want everything to be in normal mode
	store_io	TCNT0,	131				// we start TCNT0 with 131, so it overflows after 125 counts 
	store		TIMSK0, 0x01			// we want to enable interrupt by overflow

	// making sure our registers start with zero
	ldi		R17, 0
	ldi		R19, 0
	ldi		R20, 0
	ldi		R21, 0
	ldi		R22, 0
	ldi		R23, 0
	ret

start_count:
	store_io	TCCR0B, 0x04			// setting prescaler to 256, which is f/256, which is 62,5kHz
	sei									// enabling global interrupt
	ret




