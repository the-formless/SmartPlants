                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ISO C Compiler
                                      3 ; Version 4.5.12 #15786 (Linux)
                                      4 ;--------------------------------------------------------
                                      5 	.module main
                                      6 	
                                      7 ;--------------------------------------------------------
                                      8 ; Public variables in this module
                                      9 ;--------------------------------------------------------
                                     10 	.globl _main
                                     11 	.globl _delay_ms
                                     12 	.globl _led_toggle
                                     13 	.globl _led_init
                                     14 	.globl _hal_gpio_init
                                     15 ;--------------------------------------------------------
                                     16 ; ram data
                                     17 ;--------------------------------------------------------
                                     18 	.area DATA
                                     19 ;--------------------------------------------------------
                                     20 ; ram data
                                     21 ;--------------------------------------------------------
                                     22 	.area INITIALIZED
                                     23 ;--------------------------------------------------------
                                     24 ; Stack segment in internal ram
                                     25 ;--------------------------------------------------------
                                     26 	.area SSEG
      000002                         27 __start__stack:
      000002                         28 	.ds	1
                                     29 
                                     30 ;--------------------------------------------------------
                                     31 ; absolute external ram data
                                     32 ;--------------------------------------------------------
                                     33 	.area DABS (ABS)
                                     34 
                                     35 ; default segment ordering for linker
                                     36 	.area HOME
                                     37 	.area GSINIT
                                     38 	.area GSFINAL
                                     39 	.area CONST
                                     40 	.area INITIALIZER
                                     41 	.area CODE
                                     42 
                                     43 ;--------------------------------------------------------
                                     44 ; interrupt vector
                                     45 ;--------------------------------------------------------
                                     46 	.area HOME
      008000                         47 __interrupt_vect:
      008000 82 00 80 07             48 	int s_GSINIT ; reset
                                     49 ;--------------------------------------------------------
                                     50 ; global & static initialisations
                                     51 ;--------------------------------------------------------
                                     52 	.area HOME
                                     53 	.area GSINIT
                                     54 	.area GSFINAL
                                     55 	.area GSINIT
      008007 CD 81 78         [ 4]   56 	call	___sdcc_external_startup
      00800A 4D               [ 1]   57 	tnz	a
      00800B 27 03            [ 1]   58 	jreq	__sdcc_init_data
      00800D CC 80 04         [ 2]   59 	jp	__sdcc_program_startup
      008010                         60 __sdcc_init_data:
                                     61 ; stm8_genXINIT() start
      008010 AE 00 01         [ 2]   62 	ldw x, #l_DATA
      008013 27 07            [ 1]   63 	jreq	00002$
      008015                         64 00001$:
      008015 72 4F 00 00      [ 1]   65 	clr (s_DATA - 1, x)
      008019 5A               [ 2]   66 	decw x
      00801A 26 F9            [ 1]   67 	jrne	00001$
      00801C                         68 00002$:
      00801C AE 00 00         [ 2]   69 	ldw	x, #l_INITIALIZER
      00801F 27 09            [ 1]   70 	jreq	00004$
      008021                         71 00003$:
      008021 D6 80 30         [ 1]   72 	ld	a, (s_INITIALIZER - 1, x)
      008024 D7 00 01         [ 1]   73 	ld	(s_INITIALIZED - 1, x), a
      008027 5A               [ 2]   74 	decw	x
      008028 26 F7            [ 1]   75 	jrne	00003$
      00802A                         76 00004$:
                                     77 ; stm8_genXINIT() end
                                     78 	.area GSFINAL
      00802E CC 80 04         [ 2]   79 	jp	__sdcc_program_startup
                                     80 ;--------------------------------------------------------
                                     81 ; Home
                                     82 ;--------------------------------------------------------
                                     83 	.area HOME
                                     84 	.area HOME
      008004                         85 __sdcc_program_startup:
      008004 CC 80 5A         [ 2]   86 	jp	_main
                                     87 ;	return from main will return to caller
                                     88 ;--------------------------------------------------------
                                     89 ; code
                                     90 ;--------------------------------------------------------
                                     91 	.area CODE
                                     92 ;	src/main.c: 5: void delay_ms(uint16_t ms) {
                                     93 ;	-----------------------------------------
                                     94 ;	 function delay_ms
                                     95 ;	-----------------------------------------
      008031                         96 _delay_ms:
      008031 52 06            [ 2]   97 	sub	sp, #6
      008033 1F 05            [ 2]   98 	ldw	(0x05, sp), x
                                     99 ;	src/main.c: 7: for (i = 0; i < ms; i++) {
      008035 5F               [ 1]  100 	clrw	x
      008036 1F 01            [ 2]  101 	ldw	(0x01, sp), x
      008038                        102 00107$:
      008038 1E 01            [ 2]  103 	ldw	x, (0x01, sp)
      00803A 13 05            [ 2]  104 	cpw	x, (0x05, sp)
      00803C 24 19            [ 1]  105 	jrnc	00109$
                                    106 ;	src/main.c: 8: for (j = 0; j < 1600; j++) {  // Approximate 1ms at 16MHz
      00803E 5F               [ 1]  107 	clrw	x
      00803F 1F 03            [ 2]  108 	ldw	(0x03, sp), x
      008041                        109 00104$:
      008041 1E 03            [ 2]  110 	ldw	x, (0x03, sp)
      008043 A3 06 40         [ 2]  111 	cpw	x, #0x0640
      008046 24 08            [ 1]  112 	jrnc	00108$
                                    113 ;	src/main.c: 9: __asm__("nop");
      008048 9D               [ 1]  114 	nop
                                    115 ;	src/main.c: 8: for (j = 0; j < 1600; j++) {  // Approximate 1ms at 16MHz
      008049 1E 03            [ 2]  116 	ldw	x, (0x03, sp)
      00804B 5C               [ 1]  117 	incw	x
      00804C 1F 03            [ 2]  118 	ldw	(0x03, sp), x
      00804E 20 F1            [ 2]  119 	jra	00104$
      008050                        120 00108$:
                                    121 ;	src/main.c: 7: for (i = 0; i < ms; i++) {
      008050 1E 01            [ 2]  122 	ldw	x, (0x01, sp)
      008052 5C               [ 1]  123 	incw	x
      008053 1F 01            [ 2]  124 	ldw	(0x01, sp), x
      008055 20 E1            [ 2]  125 	jra	00107$
      008057                        126 00109$:
                                    127 ;	src/main.c: 12: }
      008057 5B 06            [ 2]  128 	addw	sp, #6
      008059 81               [ 4]  129 	ret
                                    130 ;	src/main.c: 13: int main(void) {
                                    131 ;	-----------------------------------------
                                    132 ;	 function main
                                    133 ;	-----------------------------------------
      00805A                        134 _main:
                                    135 ;	src/main.c: 15: hal_gpio_init();
      00805A CD 80 6C         [ 4]  136 	call	_hal_gpio_init
                                    137 ;	src/main.c: 16: led_init();
      00805D CD 81 43         [ 4]  138 	call	_led_init
                                    139 ;	src/main.c: 19: while (1) {
      008060                        140 00102$:
                                    141 ;	src/main.c: 20: led_toggle();
      008060 CD 81 63         [ 4]  142 	call	_led_toggle
                                    143 ;	src/main.c: 22: delay_ms(1000);
      008063 AE 03 E8         [ 2]  144 	ldw	x, #0x03e8
      008066 CD 80 31         [ 4]  145 	call	_delay_ms
      008069 20 F5            [ 2]  146 	jra	00102$
                                    147 ;	src/main.c: 25: return 0;
                                    148 ;	src/main.c: 26: }
      00806B 81               [ 4]  149 	ret
                                    150 	.area CODE
                                    151 	.area CONST
                                    152 	.area INITIALIZER
                                    153 	.area CABS (ABS)
