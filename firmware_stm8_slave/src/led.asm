;--------------------------------------------------------
; File Created by SDCC : free open source ISO C Compiler
; Version 4.5.12 #15786 (Linux)
;--------------------------------------------------------
	.module led
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _hal_gpio_write
	.globl _hal_gpio_set_mode
	.globl _led_init
	.globl _led_on
	.globl _led_off
	.globl _led_toggle
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area DATA
_led_toggle_state_10000_16:
	.ds 1
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area INITIALIZED
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
; global & static initialisations
;--------------------------------------------------------
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area GSINIT
;	src/led.c: 14: static uint8_t state = 0;
	clr	_led_toggle_state_10000_16+0
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME
	.area HOME
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE
;	src/led.c: 3: void led_init(void) {
;	-----------------------------------------
;	 function led_init
;	-----------------------------------------
_led_init:
;	src/led.c: 4: hal_gpio_set_mode(LED_PORT, LED_PIN, GPIO_MODE_OUTPUT_PP_SLOW);
	push	#0x12
	push	#0x04
	ld	a, #0x02
	call	_hal_gpio_set_mode
;	src/led.c: 5: led_off();
;	src/led.c: 6: }
	jp	_led_off
;	src/led.c: 7: void led_on(void) {
;	-----------------------------------------
;	 function led_on
;	-----------------------------------------
_led_on:
;	src/led.c: 8: hal_gpio_write(LED_PORT, LED_PIN, GPIO_HIGH);
	push	#0x01
	push	#0x04
	ld	a, #0x02
	call	_hal_gpio_write
;	src/led.c: 9: }
	ret
;	src/led.c: 10: void led_off(void) {
;	-----------------------------------------
;	 function led_off
;	-----------------------------------------
_led_off:
;	src/led.c: 11: hal_gpio_write(LED_PORT, LED_PIN, GPIO_LOW);
	push	#0x00
	push	#0x04
	ld	a, #0x02
	call	_hal_gpio_write
;	src/led.c: 12: }
	ret
;	src/led.c: 13: void led_toggle(void) {
;	-----------------------------------------
;	 function led_toggle
;	-----------------------------------------
_led_toggle:
;	src/led.c: 15: state = !state;
	ld	a, _led_toggle_state_10000_16+0
	sub	a, #0x01
	clr	a
	rlc	a
	ld	_led_toggle_state_10000_16+0, a
;	src/led.c: 16: hal_gpio_write(LED_PORT, LED_PIN, state);
	push	_led_toggle_state_10000_16+0
	push	#0x04
	ld	a, #0x02
	call	_hal_gpio_write
;	src/led.c: 17: }
	ret
	.area CODE
	.area CONST
	.area INITIALIZER
	.area CABS (ABS)
