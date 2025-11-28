#include "uart.h"
#include "protocol.h"
#include <stdint.h>

// static PacketState pkt_state = PKT_WAIT_STX;
// static Packet current_pkt;
// static uint16_t payload_index = 0;
// static uint8_t crc_buffer[6 + PAYLOAD_MAX_SIZE]; //addr + opcode + len + payload 
// static uint16_t crc_index = 0;

static uint8_t slave_id = 0x01; //example slave ID

uint16_t crc16_modbus(const uint8_t *data, uint16_t length) {
    uint16_t crc = 0xFFFF;

    for (uint16_t i = 0; i < length; i++) {
        crc ^= data[i];

        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }

    return crc;
}

uint8_t parse_packets(Packet *out) {
    uint8_t byte;

    while(UART1_Available()){
        switch(pkt_state) {
            case PKT_WAIT_STX:
            if(byte == STX) {
                pkt_state = PKT_WAIT_ADDR;
                crc_index = 0;
            }
            break;
            case PKT_WAIT_ADDR:
                current_pkt.addr = byte;
                crc_buffer[crc_index++] = byte;
                pkt_state = PKT_WAIT_OPCODE;
                break;
            case PKT_WAIT_OPCODE:
                current_pkt.opcode = byte;
                crc_buffer[crc_index++] = byte;
                pkt_state = PKT_WAIT_LEN;
                break;
            case PKT_WAIT_LEN:
                current_pkt.len = byte;
                crc_buffer[crc_index++] = byte;

                if(byte > 64) {
                    pkt_state = PKT_WAIT_STX;
                } else {
                    payload_index = 0;
                    pkt_state = PKT_WAIT_PAYLOAD;
                }
                break;
            case PKT_WAIT_PAYLOAD:
                current_pkt.payload[payload_index] = byte;
                crc_buffer[crc_index++] = byte;

                if(++payload_index >= current_pkt.len) {
                    pkt_state = PKT_WAIT_CRC_L;
                }
                break;
            case PKT_WAIT_CRC_L:
                current_pkt.crc = byte;
                pkt_state = PKT_WAIT_CRC_H;
                break;
            case PKT_WAIT_CRC_H:
                current_pkt.crc |= (byte << 8);

                uint16_t calc = crc16_modbus(crc_buffer, crc_index);

                if(calc == current_pkt.crc){
                    *out = current_pkt;
                    pkt_state = PKT_WAIT_STX;
                    return 1; //packet ready
                } else {
                    //Bad CRC -> resync
                    pkt_state = PKT_WAIT_STX;
                }
                break;
        }
    }

    return 0; //no complete packet yet
}

void handle_packet(const Packet *p) 
{
    //Address check
    if(p->addr != slave_id && p->addr != 0x00) {
        return; //not for us
    }
    return;
    // switch(p->opcode)
    // {
    //     //fill later
    // }
}