#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define PAYLOAD_MAX_SIZE 64
#define STX 0xAA

typedef struct {
    uint8_t addr;
    uint8_t opcode;
    uint8_t len;
    uint8_t payload[PAYLOAD_MAX_SIZE];
    uint16_t crc;
} Packet;

typedef enum {
    PKT_WAIT_STX,
    PKT_WAIT_ADDR,
    PKT_WAIT_OPCODE,
    PKT_WAIT_LEN,
    PKT_WAIT_PAYLOAD,
    PKT_WAIT_CRC_L,
    PKT_WAIT_CRC_H
} PacketState;

static PacketState pkt_state = PKT_WAIT_STX;
static Packet current_pkt;
static uint16_t payload_index = 0;
static uint8_t crc_buffer[6 + PAYLOAD_MAX_SIZE]; //addr + opcode + len + payload 
static uint16_t crc_index = 0;


void handle_packet(const Packet *p);
uint8_t parse_packets(Packet *out);
uint16_t crc16_modbus(const uint8_t *data, uint16_t length);

#endif