                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ISO C Compiler
                                      3 ; Version 4.5.12 #15786 (Linux)
                                      4 ;--------------------------------------------------------
                                      5 	.module led
                                      6 	
                                      7 ;--------------------------------------------------------
                                      8 ; Public variables in this module
                                      9 ;--------------------------------------------------------
                                     10 	.globl _hal_gpio_write
                                     11 	.globl _hal_gpio_set_mode
                                     12 	.globl _led_init
                                     13 	.globl _led_on
                                     14 	.globl _led_off
                                     15 	.globl _led_toggle
                                     16 ;--------------------------------------------------------
                                     17 ; ram data
                                     18 ;--------------------------------------------------------
                                     19 	.area DATA
      000001                         20 _led_toggle_state_10000_16:
      000001                         21 	.ds 1
                                     22 ;--------------------------------------------------------
                                     23 ; ram data
                                     24 ;--------------------------------------------------------
                                     25 	.area INITIALIZED
                                     26 ;--------------------------------------------------------
                                     27 ; absolute external ram data
                                     28 ;--------------------------------------------------------
                                     29 	.area DABS (ABS)
                                     30 
                                     31 ; default segment ordering for linker
                                     32 	.area HOME
                                     33 	.area GSINIT
                                     34 	.area GSFINAL
                                     35 	.area CONST
                                     36 	.area INITIALIZER
                                     37 	.area CODE
                                     38 
                                     39 ;--------------------------------------------------------
                                     40 ; global & static initialisations
                                     41 ;--------------------------------------------------------
                                     42 	.area HOME
                                     43 	.area GSINIT
                                     44 	.area GSFINAL
                                     45 	.area GSINIT
                                     46 ;	src/led.c: 14: static uint8_t state = 0;
      00802A 72 5F 00 01      [ 1]   47 	clr	_led_toggle_state_10000_16+0
                                     48 ;--------------------------------------------------------
                                     49 ; Home
                                     50 ;--------------------------------------------------------
                                     51 	.area HOME
                                     52 	.area HOME
                                     53 ;--------------------------------------------------------
                                     54 ; code
                                     55 ;--------------------------------------------------------
                                     56 	.area CODE
                                     57 ;	src/led.c: 3: void led_init(void) {
                                     58 ;	-----------------------------------------
                                     59 ;	 function led_init
                                     60 ;	-----------------------------------------
      008143                         61 _led_init:
                                     62 ;	src/led.c: 4: hal_gpio_set_mode(LED_PORT, LED_PIN, GPIO_MODE_OUTPUT_PP_SLOW);
      008143 4B 12            [ 1]   63 	push	#0x12
      008145 4B 04            [ 1]   64 	push	#0x04
      008147 A6 02            [ 1]   65 	ld	a, #0x02
      008149 CD 80 6D         [ 4]   66 	call	_hal_gpio_set_mode
                                     67 ;	src/led.c: 5: led_off();
                                     68 ;	src/led.c: 6: }
      00814C CC 81 59         [ 2]   69 	jp	_led_off
                                     70 ;	src/led.c: 7: void led_on(void) {
                                     71 ;	-----------------------------------------
                                     72 ;	 function led_on
                                     73 ;	-----------------------------------------
      00814F                         74 _led_on:
                                     75 ;	src/led.c: 8: hal_gpio_write(LED_PORT, LED_PIN, GPIO_HIGH);
      00814F 4B 01            [ 1]   76 	push	#0x01
      008151 4B 04            [ 1]   77 	push	#0x04
      008153 A6 02            [ 1]   78 	ld	a, #0x02
      008155 CD 80 F2         [ 4]   79 	call	_hal_gpio_write
                                     80 ;	src/led.c: 9: }
      008158 81               [ 4]   81 	ret
                                     82 ;	src/led.c: 10: void led_off(void) {
                                     83 ;	-----------------------------------------
                                     84 ;	 function led_off
                                     85 ;	-----------------------------------------
      008159                         86 _led_off:
                                     87 ;	src/led.c: 11: hal_gpio_write(LED_PORT, LED_PIN, GPIO_LOW);
      008159 4B 00            [ 1]   88 	push	#0x00
      00815B 4B 04            [ 1]   89 	push	#0x04
      00815D A6 02            [ 1]   90 	ld	a, #0x02
      00815F CD 80 F2         [ 4]   91 	call	_hal_gpio_write
                                     92 ;	src/led.c: 12: }
      008162 81               [ 4]   93 	ret
                                     94 ;	src/led.c: 13: void led_toggle(void) {
                                     95 ;	-----------------------------------------
                                     96 ;	 function led_toggle
                                     97 ;	-----------------------------------------
      008163                         98 _led_toggle:
                                     99 ;	src/led.c: 15: state = !state;
      008163 C6 00 01         [ 1]  100 	ld	a, _led_toggle_state_10000_16+0
      008166 A0 01            [ 1]  101 	sub	a, #0x01
      008168 4F               [ 1]  102 	clr	a
      008169 49               [ 1]  103 	rlc	a
      00816A C7 00 01         [ 1]  104 	ld	_led_toggle_state_10000_16+0, a
                                    105 ;	src/led.c: 16: hal_gpio_write(LED_PORT, LED_PIN, state);
      00816D 3B 00 01         [ 1]  106 	push	_led_toggle_state_10000_16+0
      008170 4B 04            [ 1]  107 	push	#0x04
      008172 A6 02            [ 1]  108 	ld	a, #0x02
      008174 CD 80 F2         [ 4]  109 	call	_hal_gpio_write
                                    110 ;	src/led.c: 17: }
      008177 81               [ 4]  111 	ret
                                    112 	.area CODE
                                    113 	.area CONST
                                    114 	.area INITIALIZER
                                    115 	.area CABS (ABS)
