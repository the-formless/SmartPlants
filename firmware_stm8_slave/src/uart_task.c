#include "uart.h"
#include <string.h>

static char uart_line[32];
static uint8_t uart_index = 0;

void UART_Task(void)
{
    // Consume all bytes in RX ring buffer
    while (UART1_Available())
    {
        uint8_t c = UART1_Read();

        // Echo back async (non-blocking)
        UART1_WriteAsync(c);

        // Line termination
        if (c == '\n' || c == '\r')
        {
            uart_line[uart_index] = '\0';

            // Send the line to command handler (later)
            // Command_PushLine(uart_line);

            uart_index = 0;    // reset for next line
            continue;
        }

        // Normal character
        if (uart_index < sizeof(uart_line) - 1)
        {
            uart_line[uart_index++] = c;
        }
    }
}
