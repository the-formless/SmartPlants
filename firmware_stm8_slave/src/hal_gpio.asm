;--------------------------------------------------------
; File Created by SDCC : free open source ISO C Compiler
; Version 4.5.12 #15786 (Linux)
;--------------------------------------------------------
	.module hal_gpio
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _hal_gpio_init
	.globl _hal_gpio_set_mode
	.globl _hal_gpio_write
	.globl _hal_gpio_read
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area DATA
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
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME
	.area HOME
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE
;	src/hal_gpio.c: 15: void hal_gpio_init(void) {
;	-----------------------------------------
;	 function hal_gpio_init
;	-----------------------------------------
_hal_gpio_init:
;	src/hal_gpio.c: 17: }
	ret
;	src/hal_gpio.c: 18: void hal_gpio_set_mode(uint8_t port, uint8_t pin, uint8_t mode) {
;	-----------------------------------------
;	 function hal_gpio_set_mode
;	-----------------------------------------
_hal_gpio_set_mode:
	sub	sp, #5
	ld	(0x05, sp), a
;	src/hal_gpio.c: 19: uint8_t pin_mask = (uint8_t)(1 << pin);
	ld	a, #0x01
	push	a
	ld	a, (0x09, sp)
	jreq	00114$
00113$:
	sll	(1, sp)
	dec	a
	jrne	00113$
00114$:
	pop	a
	ld	(0x01, sp), a
;	src/hal_gpio.c: 22: if (mode & 0x10) {  // Output modes have bit 4 set
;	src/hal_gpio.c: 23: GPIO_DDR(port) |= pin_mask;
	ld	a, (0x05, sp)
	ld	(0x04, sp), a
	clr	(0x03, sp)
;	src/hal_gpio.c: 25: GPIO_DDR(port) &= (uint8_t)~pin_mask;
	ld	a, (0x01, sp)
	cpl	a
	ld	(0x02, sp), a
;	src/hal_gpio.c: 23: GPIO_DDR(port) |= pin_mask;
	ldw	x, (0x03, sp)
	sllw	x
	sllw	x
	addw	x, (0x03, sp)
	ldw	(0x03, sp), x
	addw	x, #0x5002
	ld	a, (x)
;	src/hal_gpio.c: 22: if (mode & 0x10) {  // Output modes have bit 4 set
	push	a
	ld	a, (0x0a, sp)
	bcp	a, #0x10
	pop	a
	jreq	00102$
;	src/hal_gpio.c: 23: GPIO_DDR(port) |= pin_mask;
	or	a, (0x01, sp)
	ld	(x), a
	jra	00103$
00102$:
;	src/hal_gpio.c: 25: GPIO_DDR(port) &= (uint8_t)~pin_mask;
	and	a, (0x02, sp)
	ld	(x), a
00103$:
;	src/hal_gpio.c: 29: GPIO_CR1(port) &= (uint8_t)~pin_mask;
	ldw	x, (0x03, sp)
	addw	x, #0x5003
	ld	a, (x)
	and	a, (0x02, sp)
	ld	(x), a
;	src/hal_gpio.c: 30: GPIO_CR2(port) &= (uint8_t)~pin_mask;
	ldw	y, (0x03, sp)
	addw	y, #0x5004
	ld	a, (y)
	and	a, (0x02, sp)
	ld	(y), a
;	src/hal_gpio.c: 33: GPIO_CR1(port) |= (uint8_t)(mode & 0x01) << pin;
	ld	a, (x)
	ld	(0x04, sp), a
	ld	a, (0x09, sp)
	and	a, #0x01
	push	a
	ld	a, (0x09, sp)
	jreq	00117$
00116$:
	sll	(1, sp)
	dec	a
	jrne	00116$
00117$:
	pop	a
	or	a, (0x04, sp)
	ld	(x), a
;	src/hal_gpio.c: 34: GPIO_CR2(port) |= (uint8_t)((mode >> 4) & 0x01) << pin;
	ld	a, (y)
	ld	(0x04, sp), a
	ld	a, (0x09, sp)
	srl	a
	srl	a
	srl	a
	srl	a
	and	a, #0x01
	push	a
	ld	a, (0x09, sp)
	jreq	00119$
00118$:
	sll	(1, sp)
	dec	a
	jrne	00118$
00119$:
	pop	a
	or	a, (0x04, sp)
	ld	(y), a
;	src/hal_gpio.c: 35: }
	ldw	x, (6, sp)
	addw	sp, #9
	jp	(x)
;	src/hal_gpio.c: 36: void hal_gpio_write(uint8_t port, uint8_t pin, uint8_t state) {
;	-----------------------------------------
;	 function hal_gpio_write
;	-----------------------------------------
_hal_gpio_write:
	push	a
;	src/hal_gpio.c: 38: GPIO_ODR(port) |= (uint8_t)(1 << pin);
	clrw	x
	ld	xl, a
	ld	a, #0x01
	push	a
	ld	a, (0x05, sp)
	jreq	00114$
00113$:
	sll	(1, sp)
	dec	a
	jrne	00113$
00114$:
	pop	a
	pushw	x
	sllw	x
	sllw	x
	addw	x, (1, sp)
	addw	sp, #2
	addw	x, #0x5000
	push	a
	ld	a, (x)
	ld	(0x02, sp), a
	pop	a
;	src/hal_gpio.c: 37: if (state) {
	tnz	(0x05, sp)
	jreq	00102$
;	src/hal_gpio.c: 38: GPIO_ODR(port) |= (uint8_t)(1 << pin);
	or	a, (0x01, sp)
	ld	(x), a
	jra	00104$
00102$:
;	src/hal_gpio.c: 40: GPIO_ODR(port) &= (uint8_t)~(1 << pin);
	cpl	a
	and	a, (0x01, sp)
	ld	(x), a
00104$:
;	src/hal_gpio.c: 42: }
	ldw	x, (2, sp)
	addw	sp, #5
	jp	(x)
;	src/hal_gpio.c: 43: uint8_t hal_gpio_read(uint8_t port, uint8_t pin) {
;	-----------------------------------------
;	 function hal_gpio_read
;	-----------------------------------------
_hal_gpio_read:
;	src/hal_gpio.c: 44: return (GPIO_IDR(port) >> pin) & 0x01;
	clrw	x
	ld	xl, a
	pushw	x
	sllw	x
	sllw	x
	addw	x, (1, sp)
	addw	sp, #2
	addw	x, #0x5001
	ld	a, (x)
	push	a
	ld	a, (0x04, sp)
	jreq	00104$
00103$:
	srl	(1, sp)
	dec	a
	jrne	00103$
00104$:
	pop	a
	and	a, #0x01
;	src/hal_gpio.c: 45: }
	popw	x
	addw	sp, #1
	jp	(x)
	.area CODE
	.area CONST
	.area INITIALIZER
	.area CABS (ABS)
