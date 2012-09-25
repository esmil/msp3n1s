/*
 * This file is part of msp3n1s
 * Copyright 2011-2012 Emil Renner Berthing
 *
 * msp3n1s is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * msp3n1s is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with msp3n1s.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <msp430.h>

/* Workaround for TI header bug */
#ifdef LPM0_bits
#undef LPM0
#define LPM0 LPM0_bits
#endif

.macro timera_off
	mov	#0,	&TACTL
.endm
.macro timera_idle
	mov	#0x0200,&TACTL
.endm
.macro timera_reset
	mov	#0x0216,&TACTL
.endm
.macro timera_cc0_off
	mov	#0,	&TACCTL0
.endm
.macro timera_cc1_off
	mov	#0,	&TACCTL1
.endm
.macro timera_cc1_capture_rising
	mov	#0x4910,&TACCTL1
.endm
.macro timera_cc1_capture_falling
	mov	#0x8910,&TACCTL1
.endm
.macro timera_cc1_interrupt_clear
	bic	#1,	&TACCTL1
.endm
.macro pin_high m n
	bis.b   #(1<<\n),       &P\m\()OUT
.endm
.macro pin_low m n
	bic.b   #(1<<\n),       &P\m\()OUT
.endm
.macro pin_toggle m n
	xor.b	#(1<<\n),	&P\m\()OUT
.endm
.macro pin_mode_output m n
	bis.b	#(1<<\n),	&P\m\()DIR
.endm
.macro pin_mode_input m n
	bic.b	#(1<<\n),	&P\m\()DIR
.endm
.macro pin_is_high m n
	bit.b	#(1<<\n),	&P\m\()IN
.endm
.macro pin_function_primary m n
	bis.b	#(1<<\n),	&P\m\()SEL
.endm

.global onewire_getcmd
.global __isr_8
.extern onewire_rom
.extern onewire_commands
.extern onewire_alarm

.section .text, "ax"

; r10 state
; r11 scratch, mostly wait count
; r12 bitmask
; r13 length or one-byte input
; r14 memory address
; r15 flags/command

onewire_getcmd:
	push	r11
	push	r10
	timera_cc0_off
	timera_cc1_off
	timera_off
	; BCSCTL1 = CALBC1_1MHZ + 2
	mov.b	&CALBC1_1MHZ,r11
	add.b	#2,	r11
	mov.b	r11,	&BCSCTL1
	mov 	#900,	&TACCR0
	mov 	#idle,	r10
	timera_cc1_capture_falling
	timera_idle
	bis	#LPM0,	r2
	mov.b	&CALBC1_1MHZ,&BCSCTL1
	pop	r10
	pop	r11
	ret

; timera_interrupt
__isr_8:
	add	&TAIV,	r0	; it's a jump table!
	reti			;  0: unused
	br	r10		;  2: TACCR1
	reti			;  4: unused
	reti			;  6: reserved
	reti			;  8: reserved
				; 10: TAIFG
	pin_is_high 1 2
	timera_idle
	jnz	1f
	timera_cc1_capture_rising
	pin_is_high 1 2
	jnz	ack
	mov	#ack,	r10
1:	reti

ack:
	mov	#8,	r11
	dec	r11
	jnz	$-2
	pin_mode_output 1 2
	mov	#91,	r11
	dec	r11
	jnz	$-2
	mov	#0,	r13
	mov.b	#1,	r12
	mov	#ow_cmd,r10
	timera_cc1_capture_falling
	pin_mode_input 1 2
	reti

idle:
	timera_reset
	reti

sample_wait:
	timera_reset
	mov	#8,	r11
	dec	r11
	jnz	$-2
	ret

ow_cmd:
	call	#sample_wait
	pin_is_high 1 2
	jz	1f
	bis.b	r12,	r13

1:	rla.b	r12
	jnz	4f

	mov	#onewire_rom,r14
	mov.b	#1,	r12

	; alarm search
	cmp.b	#0xEC,	r13
	jz	search_alarm

	; search rom
	cmp.b	#0xF0,	r13
	jz	search

	; read rom
	cmp.b	#0x33,	r13
	jnz	2f
	mov	#-1,	r15
	mov	#8,	r13
	jmp	read_next

2:	; match rom
	cmp.b	#0x55,	r13
	jnz	3f
	mov	#8,	r13
	mov	#match,	r10
	reti

3:	; skip rom
	cmp.b	#0xCC,	r13
	jnz	sleep
	mov	#0,	r13
	mov	#cmd,	r10
4:	reti


search_alarm:
	cmp.b	#0,	&onewire_alarm
	jz	sleep

search:
	mov	#8,	r13

search_next:
	bit.b	@r14,	r12
	jnz	1f

	mov	#search01,r10
	reti

1:	mov	#search11,r10
	reti

search01:
	pin_mode_output 1 2
	timera_reset
	mov	#search02,r10
	jmp	search_tail

search02:
	timera_reset
	mov	#search03,r10
	jmp	search_tail

search11:
	timera_reset
	mov	#search12,r10
	jmp	search_tail

search12:
	pin_mode_output 1 2
	timera_reset
	mov	#search13,r10
	nop

search_tail:
	mov	#8,	r11
	dec	r11
	jnz	$-2
	timera_cc1_interrupt_clear
	pin_mode_input 1 2
	reti

search03:
	call	#sample_wait
	pin_is_high 1 2
	jnz	sleep
	jmp 	search_inc

search13:
	call	#sample_wait
	pin_is_high 1 2
	jz	sleep

search_inc:
	rla.b	r12
	jnz	search_next
	mov.b	#1,	r12
	inc	r14
	dec	r13
	jnz	search_next
get_cmd:
	mov	#cmd,	r10
	reti

match:
	call	#sample_wait
	pin_is_high 1 2
	jz	1f

	bit.b	@r14,	r12
	jz	sleep
	jmp	2f

1:	bit.b	@r14,	r12
	jnz	sleep

2:	rla.b	r12
	jnz	3f
	mov.b	#1,	r12
	inc	r14
	dec	r13
	jz	get_cmd
3:	reti

cmd:
	call	#sample_wait
	pin_is_high 1 2
	jz	1f
	bis.b	r12,	r13

1:	rla.b	r12
	jnz	4f

	mov	#onewire_commands,r11
2:	mov	@r11+,	r15
	cmp.b	r15,	r13
	swpb	r15
	jz	3f
	bit.b	#1,	r15
	jnz	sleep
	add	#4,	r11
	jmp	2b

3:	mov	@r11+,	r13
	mov.b	#1,	r12
	cmp	#0,	r13
	jz	cmd_done
	mov	@r11,	r14
	bit.b	#4,	r15
	jz	read_next
	mov.b	#0,	@r14
	mov	#write,	r10
4:	reti

read1:
	timera_reset
	jmp	read_inc

read0:
	pin_mode_output 1 2
	timera_reset
	mov	#8,	r11
	dec	r11
	jnz	$-2
	timera_cc1_interrupt_clear
	pin_mode_input 1 2

read_inc:
	rla.b	r12
	jnz	read_next
	mov.b	#1,	r12
	inc	r14
	dec	r13
	jz	cmd_done

read_next:
	bit.b	@r14,	r12
	jnz	1f

	mov	#read0,	r10
	reti

1:	mov	#read1,	r10
	reti

write:
	call	#sample_wait
	pin_is_high 1 2
	jz	1f
	bis.b	r12,	@r14
1:	rla.b	r12
	jnz	2f
	mov.b	#1,	r12
	inc	r14
	dec	r13
	jz	cmd_done
	mov.b	#0,	@r14
2:	reti

cmd_done:
	bit.b	#2,	r15
	jz	sleep
	cmp	#-1,	r15
	jz	get_cmd
	swpb	r15
	timera_cc1_off
	timera_off
	bic	#LPM0,	0(r1)
	reti

sleep:
	mov	#idle,	r10
	reti

; vim: set syntax=asm:
