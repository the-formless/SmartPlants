                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ISO C Compiler
                                      3 ; Version 4.5.12 #15786 (Linux)
                                      4 ;--------------------------------------------------------
                                      5 	.module hal_gpio
                                      6 	
                                      7 ;--------------------------------------------------------
                                      8 ; Public variables in this module
                                      9 ;--------------------------------------------------------
                                     10 	.globl _hal_gpio_init
                                     11 	.globl _hal_gpio_set_mode
                                     12 	.globl _hal_gpio_write
                                     13 	.globl _hal_gpio_read
                                     14 ;--------------------------------------------------------
                                     15 ; ram data
                                     16 ;--------------------------------------------------------
                                     17 	.area DATA
                                     18 ;--------------------------------------------------------
                                     19 ; ram data
                                     20 ;--------------------------------------------------------
                                     21 	.area INITIALIZED
                                     22 ;--------------------------------------------------------
                                     23 ; absolute external ram data
                                     24 ;--------------------------------------------------------
                                     25 	.area DABS (ABS)
                                     26 
                                     27 ; default segment ordering for linker
                                     28 	.area HOME
                                     29 	.area GSINIT
                                     30 	.area GSFINAL
                                     31 	.area CONST
                                     32 	.area INITIALIZER
                                     33 	.area CODE
                                     34 
                                     35 ;--------------------------------------------------------
                                     36 ; global & static initialisations
                                     37 ;--------------------------------------------------------
                                     38 	.area HOME
                                     39 	.area GSINIT
                                     40 	.area GSFINAL
                                     41 	.area GSINIT
                                     42 ;--------------------------------------------------------
                                     43 ; Home
                                     44 ;--------------------------------------------------------
                                     45 	.area HOME
                                     46 	.area HOME
                                     47 ;--------------------------------------------------------
                                     48 ; code
                                     49 ;--------------------------------------------------------
                                     50 	.area CODE
                                     51 ;	src/hal_gpio.c: 15: void hal_gpio_init(void) {
                                     52 ;	-----------------------------------------
                                     53 ;	 function hal_gpio_init
                                     54 ;	-----------------------------------------
      00806C                         55 _hal_gpio_init:
                                     56 ;	src/hal_gpio.c: 17: }
      00806C 81               [ 4]   57 	ret
                                     58 ;	src/hal_gpio.c: 18: void hal_gpio_set_mode(uint8_t port, uint8_t pin, uint8_t mode) {
                                     59 ;	-----------------------------------------
                                     60 ;	 function hal_gpio_set_mode
                                     61 ;	-----------------------------------------
      00806D                         62 _hal_gpio_set_mode:
      00806D 52 05            [ 2]   63 	sub	sp, #5
      00806F 6B 05            [ 1]   64 	ld	(0x05, sp), a
                                     65 ;	src/hal_gpio.c: 19: uint8_t pin_mask = (uint8_t)(1 << pin);
      008071 A6 01            [ 1]   66 	ld	a, #0x01
      008073 88               [ 1]   67 	push	a
      008074 7B 09            [ 1]   68 	ld	a, (0x09, sp)
      008076 27 05            [ 1]   69 	jreq	00114$
      008078                         70 00113$:
      008078 08 01            [ 1]   71 	sll	(1, sp)
      00807A 4A               [ 1]   72 	dec	a
      00807B 26 FB            [ 1]   73 	jrne	00113$
      00807D                         74 00114$:
      00807D 84               [ 1]   75 	pop	a
      00807E 6B 01            [ 1]   76 	ld	(0x01, sp), a
                                     77 ;	src/hal_gpio.c: 22: if (mode & 0x10) {  // Output modes have bit 4 set
                                     78 ;	src/hal_gpio.c: 23: GPIO_DDR(port) |= pin_mask;
      008080 7B 05            [ 1]   79 	ld	a, (0x05, sp)
      008082 6B 04            [ 1]   80 	ld	(0x04, sp), a
      008084 0F 03            [ 1]   81 	clr	(0x03, sp)
                                     82 ;	src/hal_gpio.c: 25: GPIO_DDR(port) &= (uint8_t)~pin_mask;
      008086 7B 01            [ 1]   83 	ld	a, (0x01, sp)
      008088 43               [ 1]   84 	cpl	a
      008089 6B 02            [ 1]   85 	ld	(0x02, sp), a
                                     86 ;	src/hal_gpio.c: 23: GPIO_DDR(port) |= pin_mask;
      00808B 1E 03            [ 2]   87 	ldw	x, (0x03, sp)
      00808D 58               [ 2]   88 	sllw	x
      00808E 58               [ 2]   89 	sllw	x
      00808F 72 FB 03         [ 2]   90 	addw	x, (0x03, sp)
      008092 1F 03            [ 2]   91 	ldw	(0x03, sp), x
      008094 1C 50 02         [ 2]   92 	addw	x, #0x5002
      008097 F6               [ 1]   93 	ld	a, (x)
                                     94 ;	src/hal_gpio.c: 22: if (mode & 0x10) {  // Output modes have bit 4 set
      008098 88               [ 1]   95 	push	a
      008099 7B 0A            [ 1]   96 	ld	a, (0x0a, sp)
      00809B A5 10            [ 1]   97 	bcp	a, #0x10
      00809D 84               [ 1]   98 	pop	a
      00809E 27 05            [ 1]   99 	jreq	00102$
                                    100 ;	src/hal_gpio.c: 23: GPIO_DDR(port) |= pin_mask;
      0080A0 1A 01            [ 1]  101 	or	a, (0x01, sp)
      0080A2 F7               [ 1]  102 	ld	(x), a
      0080A3 20 03            [ 2]  103 	jra	00103$
      0080A5                        104 00102$:
                                    105 ;	src/hal_gpio.c: 25: GPIO_DDR(port) &= (uint8_t)~pin_mask;
      0080A5 14 02            [ 1]  106 	and	a, (0x02, sp)
      0080A7 F7               [ 1]  107 	ld	(x), a
      0080A8                        108 00103$:
                                    109 ;	src/hal_gpio.c: 29: GPIO_CR1(port) &= (uint8_t)~pin_mask;
      0080A8 1E 03            [ 2]  110 	ldw	x, (0x03, sp)
      0080AA 1C 50 03         [ 2]  111 	addw	x, #0x5003
      0080AD F6               [ 1]  112 	ld	a, (x)
      0080AE 14 02            [ 1]  113 	and	a, (0x02, sp)
      0080B0 F7               [ 1]  114 	ld	(x), a
                                    115 ;	src/hal_gpio.c: 30: GPIO_CR2(port) &= (uint8_t)~pin_mask;
      0080B1 16 03            [ 2]  116 	ldw	y, (0x03, sp)
      0080B3 72 A9 50 04      [ 2]  117 	addw	y, #0x5004
      0080B7 90 F6            [ 1]  118 	ld	a, (y)
      0080B9 14 02            [ 1]  119 	and	a, (0x02, sp)
      0080BB 90 F7            [ 1]  120 	ld	(y), a
                                    121 ;	src/hal_gpio.c: 33: GPIO_CR1(port) |= (uint8_t)(mode & 0x01) << pin;
      0080BD F6               [ 1]  122 	ld	a, (x)
      0080BE 6B 04            [ 1]  123 	ld	(0x04, sp), a
      0080C0 7B 09            [ 1]  124 	ld	a, (0x09, sp)
      0080C2 A4 01            [ 1]  125 	and	a, #0x01
      0080C4 88               [ 1]  126 	push	a
      0080C5 7B 09            [ 1]  127 	ld	a, (0x09, sp)
      0080C7 27 05            [ 1]  128 	jreq	00117$
      0080C9                        129 00116$:
      0080C9 08 01            [ 1]  130 	sll	(1, sp)
      0080CB 4A               [ 1]  131 	dec	a
      0080CC 26 FB            [ 1]  132 	jrne	00116$
      0080CE                        133 00117$:
      0080CE 84               [ 1]  134 	pop	a
      0080CF 1A 04            [ 1]  135 	or	a, (0x04, sp)
      0080D1 F7               [ 1]  136 	ld	(x), a
                                    137 ;	src/hal_gpio.c: 34: GPIO_CR2(port) |= (uint8_t)((mode >> 4) & 0x01) << pin;
      0080D2 90 F6            [ 1]  138 	ld	a, (y)
      0080D4 6B 04            [ 1]  139 	ld	(0x04, sp), a
      0080D6 7B 09            [ 1]  140 	ld	a, (0x09, sp)
      0080D8 44               [ 1]  141 	srl	a
      0080D9 44               [ 1]  142 	srl	a
      0080DA 44               [ 1]  143 	srl	a
      0080DB 44               [ 1]  144 	srl	a
      0080DC A4 01            [ 1]  145 	and	a, #0x01
      0080DE 88               [ 1]  146 	push	a
      0080DF 7B 09            [ 1]  147 	ld	a, (0x09, sp)
      0080E1 27 05            [ 1]  148 	jreq	00119$
      0080E3                        149 00118$:
      0080E3 08 01            [ 1]  150 	sll	(1, sp)
      0080E5 4A               [ 1]  151 	dec	a
      0080E6 26 FB            [ 1]  152 	jrne	00118$
      0080E8                        153 00119$:
      0080E8 84               [ 1]  154 	pop	a
      0080E9 1A 04            [ 1]  155 	or	a, (0x04, sp)
      0080EB 90 F7            [ 1]  156 	ld	(y), a
                                    157 ;	src/hal_gpio.c: 35: }
      0080ED 1E 06            [ 2]  158 	ldw	x, (6, sp)
      0080EF 5B 09            [ 2]  159 	addw	sp, #9
      0080F1 FC               [ 2]  160 	jp	(x)
                                    161 ;	src/hal_gpio.c: 36: void hal_gpio_write(uint8_t port, uint8_t pin, uint8_t state) {
                                    162 ;	-----------------------------------------
                                    163 ;	 function hal_gpio_write
                                    164 ;	-----------------------------------------
      0080F2                        165 _hal_gpio_write:
      0080F2 88               [ 1]  166 	push	a
                                    167 ;	src/hal_gpio.c: 38: GPIO_ODR(port) |= (uint8_t)(1 << pin);
      0080F3 5F               [ 1]  168 	clrw	x
      0080F4 97               [ 1]  169 	ld	xl, a
      0080F5 A6 01            [ 1]  170 	ld	a, #0x01
      0080F7 88               [ 1]  171 	push	a
      0080F8 7B 05            [ 1]  172 	ld	a, (0x05, sp)
      0080FA 27 05            [ 1]  173 	jreq	00114$
      0080FC                        174 00113$:
      0080FC 08 01            [ 1]  175 	sll	(1, sp)
      0080FE 4A               [ 1]  176 	dec	a
      0080FF 26 FB            [ 1]  177 	jrne	00113$
      008101                        178 00114$:
      008101 84               [ 1]  179 	pop	a
      008102 89               [ 2]  180 	pushw	x
      008103 58               [ 2]  181 	sllw	x
      008104 58               [ 2]  182 	sllw	x
      008105 72 FB 01         [ 2]  183 	addw	x, (1, sp)
      008108 5B 02            [ 2]  184 	addw	sp, #2
      00810A 1C 50 00         [ 2]  185 	addw	x, #0x5000
      00810D 88               [ 1]  186 	push	a
      00810E F6               [ 1]  187 	ld	a, (x)
      00810F 6B 02            [ 1]  188 	ld	(0x02, sp), a
      008111 84               [ 1]  189 	pop	a
                                    190 ;	src/hal_gpio.c: 37: if (state) {
      008112 0D 05            [ 1]  191 	tnz	(0x05, sp)
      008114 27 05            [ 1]  192 	jreq	00102$
                                    193 ;	src/hal_gpio.c: 38: GPIO_ODR(port) |= (uint8_t)(1 << pin);
      008116 1A 01            [ 1]  194 	or	a, (0x01, sp)
      008118 F7               [ 1]  195 	ld	(x), a
      008119 20 04            [ 2]  196 	jra	00104$
      00811B                        197 00102$:
                                    198 ;	src/hal_gpio.c: 40: GPIO_ODR(port) &= (uint8_t)~(1 << pin);
      00811B 43               [ 1]  199 	cpl	a
      00811C 14 01            [ 1]  200 	and	a, (0x01, sp)
      00811E F7               [ 1]  201 	ld	(x), a
      00811F                        202 00104$:
                                    203 ;	src/hal_gpio.c: 42: }
      00811F 1E 02            [ 2]  204 	ldw	x, (2, sp)
      008121 5B 05            [ 2]  205 	addw	sp, #5
      008123 FC               [ 2]  206 	jp	(x)
                                    207 ;	src/hal_gpio.c: 43: uint8_t hal_gpio_read(uint8_t port, uint8_t pin) {
                                    208 ;	-----------------------------------------
                                    209 ;	 function hal_gpio_read
                                    210 ;	-----------------------------------------
      008124                        211 _hal_gpio_read:
                                    212 ;	src/hal_gpio.c: 44: return (GPIO_IDR(port) >> pin) & 0x01;
      008124 5F               [ 1]  213 	clrw	x
      008125 97               [ 1]  214 	ld	xl, a
      008126 89               [ 2]  215 	pushw	x
      008127 58               [ 2]  216 	sllw	x
      008128 58               [ 2]  217 	sllw	x
      008129 72 FB 01         [ 2]  218 	addw	x, (1, sp)
      00812C 5B 02            [ 2]  219 	addw	sp, #2
      00812E 1C 50 01         [ 2]  220 	addw	x, #0x5001
      008131 F6               [ 1]  221 	ld	a, (x)
      008132 88               [ 1]  222 	push	a
      008133 7B 04            [ 1]  223 	ld	a, (0x04, sp)
      008135 27 05            [ 1]  224 	jreq	00104$
      008137                        225 00103$:
      008137 04 01            [ 1]  226 	srl	(1, sp)
      008139 4A               [ 1]  227 	dec	a
      00813A 26 FB            [ 1]  228 	jrne	00103$
      00813C                        229 00104$:
      00813C 84               [ 1]  230 	pop	a
      00813D A4 01            [ 1]  231 	and	a, #0x01
                                    232 ;	src/hal_gpio.c: 45: }
      00813F 85               [ 2]  233 	popw	x
      008140 5B 01            [ 2]  234 	addw	sp, #1
      008142 FC               [ 2]  235 	jp	(x)
                                    236 	.area CODE
                                    237 	.area CONST
                                    238 	.area INITIALIZER
                                    239 	.area CABS (ABS)
