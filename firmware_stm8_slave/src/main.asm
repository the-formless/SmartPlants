;--------------------------------------------------------
; File Created by SDCC : free open source ISO C Compiler
; Version 4.5.12 #15786 (Linux)
;--------------------------------------------------------
	.module main
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _main
	.globl _delay_ms
	.globl _led_toggle
	.globl _led_init
	.globl _hal_gpio_init
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area DATA
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area INITIALIZED
;--------------------------------------------------------
; Stack segment in internal ram
;--------------------------------------------------------
	.area SSEG
__start__stack:
	.ds	1

;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area DABS (ABS)

; default segment ordering for linker
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area CONST
	.area INITIALIZER
	.area CODE

;--------------------------------------------------------
; interrupt vector
;--------------------------------------------------------
	.area HOME
__interrupt_vect:
	int s_GSINIT ; reset
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area GSINIT
	call	___sdcc_external_startup
	tnz	a
	jreq	__sdcc_init_data
	jp	__sdcc_program_startup
__sdcc_init_data:
; stm8_genXINIT() start
	ldw x, #l_DATA
	jreq	00002$
00001$:
	clr (s_DATA - 1, x)
	decw x
	jrne	00001$
00002$:
	ldw	x, #l_INITIALIZER
	jreq	00004$
00003$:
	ld	a, (s_INITIALIZER - 1, x)
	ld	(s_INITIALIZED - 1, x), a
	decw	x
	jrne	00003$
00004$:
; stm8_genXINIT() end
	.area GSFINAL
	jp	__sdcc_program_startup
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME
	.area HOME
__sdcc_program_startup:
	jp	_main
;	return from main will return to caller
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE
;	main.c: 5: void delay_ms(uint16_t ms) {
;	-----------------------------------------
;	 function delay_ms
;	-----------------------------------------
_delay_ms:
	sub	sp, #6
	ldw	(0x05, sp), x
;	main.c: 7: for (i = 0; i < ms; i++) {
	clrw	x
	ldw	(0x01, sp), x
00107$:
	ldw	x, (0x01, sp)
	cpw	x, (0x05, sp)
	jrnc	00109$
;	main.c: 8: for (j = 0; j < 1600; j++) {  // Approximate 1ms at 16MHz
	clrw	x
	ldw	(0x03, sp), x
00104$:
	ldw	x, (0x03, sp)
	cpw	x, #0x0640
	jrnc	00108$
;	main.c: 9: __asm__("nop");
	nop
;	main.c: 8: for (j = 0; j < 1600; j++) {  // Approximate 1ms at 16MHz
	ldw	x, (0x03, sp)
	incw	x
	ldw	(0x03, sp), x
	jra	00104$
00108$:
;	main.c: 7: for (i = 0; i < ms; i++) {
	ldw	x, (0x01, sp)
	incw	x
	ldw	(0x01, sp), x
	jra	00107$
00109$:
;	main.c: 12: }
	addw	sp, #6
	ret
;	main.c: 13: int main(void) {
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
;	main.c: 15: hal_gpio_init();
	call	_hal_gpio_init
;	main.c: 16: led_init();
	call	_led_init
;	main.c: 19: while (1) {
00102$:
;	main.c: 20: led_toggle();
	call	_led_toggle
;	main.c: 21: delay_ms(1000);
	ldw	x, #0x03e8
	call	_delay_ms
	jra	00102$
;	main.c: 24: return 0;
;	main.c: 25: }
	ret
	.area CODE
	.area CONST
	.area INITIALIZER
	.area CABS (ABS)
