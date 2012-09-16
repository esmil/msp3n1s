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

.macro timera_off
	bic #0x30, &__TACTL
.endm

.macro timera_mode_up
	bis #0x10, &__TACTL
.endm

.macro timera_mode_continuous
	bis #0x20, &__TACTL
.endm

.macro timera_clear
	bis #0x04, &__TACTL
.endm

.macro timera_interrupt_clear
	bic #0x01, &__TACTL
.endm

.macro timera_interrupt_enable
	bis #0x02, &__TACTL
.endm

.macro timera_interrupt_disable
	bic #0x02, &__TACTL
.endm

.macro timera_interrupt_enabled
	bit #0x02, &__TACTL
.endm

.macro timera_cc_mode_capture n
	bis	#0x100,	&__TACCTL\n
.endm
.macro timera_cc0_mode_capture
	timera_cc_mode_capture 0
.endm
.macro timera_cc1_mode_capture
	timera_cc_mode_capture 1
.endm

.macro timera_cc_mode_compare n
	bic	#0x100,	&__TACCTL\n
.endm
.macro timera_cc0_mode_compare
	timera_cc_mode_compare 0
.endm
.macro timera_cc1_mode_compare
	timera_cc_mode_compare 1
.endm

.macro timera_cc_capture_rising n
	bic	#0x8000,&__TACCTL\n
	bis	#0x4000,&__TACCTL\n
.endm
.macro timera_cc0_capture_rising
	timera_cc_capture_rising 0
.endm
.macro timera_cc1_capture_rising
	timera_cc_capture_rising 1
.endm

.macro timera_cc_capture_falling n
	bis	#0x8000,&__TACCTL\n
	bic	#0x4000,&__TACCTL\n
.endm
.macro timera_cc0_capture_falling
	timera_cc_capture_falling 0
.endm
.macro timera_cc1_capture_falling
	timera_cc_capture_falling 1
.endm

.macro timera_cc_interrupt_enable n
	bis	#0x10,	&__TACCTL\n
.endm
.macro timera_cc0_interrupt_enable
	timera_cc_interrupt_enable 0
.endm
.macro timera_cc1_interrupt_enable
	timera_cc_interrupt_enable 1
.endm

.macro timera_cc_interrupt_disable n
	bic	#0x10,	&__TACCTL\n
.endm
.macro timera_cc0_interrupt_disable
	timera_cc_interrupt_disable 0
.endm
.macro timera_cc1_interrupt_disable
	timera_cc_interrupt_disable 1
.endm

.macro timera_cc_interrupt_clear n
	bic	#0x01,	&__TACCTL\n
.endm
.macro timera_cc0_interrupt_clear
	timera_cc_interrupt_clear 0
.endm
.macro timera_cc1_interrupt_clear
	timera_cc_interrupt_clear 1
.endm

.macro pin_high m n
	bis.b   #(1<<\n),       &__P\m\()OUT
.endm

.macro pin_low m n
	bic.b   #(1<<\n),       &__P\m\()OUT
.endm

.macro pin_toggle m n
	xor.b	#(1<<\n),	&__P\m\()OUT
.endm

.macro pin_mode_output m n
	bis.b	#(1<<\n),	&__P\m\()DIR
.endm

.macro pin_mode_input m n
	bic.b	#(1<<\n),	&__P\m\()DIR
.endm

.macro pin_is_high m n
	bit.b	#(1<<\n),	&__P\m\()IN
.endm

.macro pin_function_primary m n
	bis.b	#(1<<\n),	&__P\m\()SEL
.endm

.macro LPM0
	bis	#16,	r2
.endm
.macro LPM0_EXIT
	bic	#16,	0(r1)
.endm

.global onewire_getcmd
.global __isr_8
.extern rom
.extern commands

.section .text, "ax"

; r10 state
; r11 scratch, mostly wait count
; r12 bitmask
; r13 length or one-byte input
; r14 address
; r15 flags/command

onewire_getcmd:
	push	r11
	push	r10
	; BCSCTL1 = CALBC1_1MHZ + 2
	mov.b	&__CALBC1_1MHZ,r11
	add.b	#2,	r11
	mov.b	r11,	&__BCSCTL1
	mov 	#idle,	r10
	timera_cc1_interrupt_clear
	timera_cc1_interrupt_enable
	timera_off
	mov 	#900,	&__TACCR0
	timera_interrupt_clear
	timera_interrupt_enable
	LPM0
	mov.b	&__CALBC1_1MHZ,&__BCSCTL1
	pop	r10
	pop	r11
	ret

; timera_interrupt
__isr_8:
	add	&__TAIV,r0	; it's a jump table!
	reti			;  0: unused
	br	r10		;  2: TACCR1
	reti			;  4: unused
	reti			;  6: reserved
	reti			;  8: reserved
				; 10: TAIFG
	pin_is_high 1 2
	jnz	1f
	timera_cc1_capture_rising
	pin_is_high 1 2
	jnz	2f
	mov	#ack,	r10

1:	timera_off
	reti
2:	timera_off

ack:
	mov	#8,	r11
	dec	r11
	jnz	$-2
	pin_mode_output 1 2
	mov	#80,	r11
	dec	r11
	jnz	$-2
	timera_cc1_capture_falling
	mov	#0,	r13
	mov.b	#1,	r12
	mov	#ow_cmd,r10
	pin_mode_input 1 2
	timera_cc1_interrupt_clear
	reti

idle:
	timera_clear
	timera_mode_up
	reti

sample_wait:
	timera_clear
	timera_mode_up
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

	mov	#rom,	r14
	mov.b	#1,	r12

	; alarm search
	;FIXME: we're always in alarm mode
	cmp.b	#0xEC,	r13
	jz	search

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
	timera_clear
	timera_mode_up
	; wait a few cycles
	mov	#12,	r11
	dec	r11
	jnz	$-2

	mov	#search02,r10
	timera_cc1_interrupt_clear
	pin_mode_input 1 2
	reti

search02:
	timera_clear
	timera_mode_up
	mov	#search03,r10
	reti

search11:
	timera_clear
	timera_mode_up
	mov	#search12,r10
	reti

search12:
	pin_mode_output 1 2
	timera_clear
	timera_mode_up
	; wait a few cycles
	mov	#12,	r11
	dec	r11
	jnz	$-2

	mov	#search13,r10
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

	mov	#commands,r11
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
	timera_clear
	timera_mode_up
	jmp	read_inc

read0:
	pin_mode_output 1 2
	timera_clear
	timera_mode_up
	; wait a few cycles
	mov	#12,	r11
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
	timera_cc1_interrupt_disable
	timera_interrupt_disable
	LPM0_EXIT
	reti

sleep:
	mov	#idle,	r10
	reti

; vim: set syntax=asm:
