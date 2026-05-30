#pragma once

#include <stdint.h>

#pragma pack(push, 1)

struct PK9BlockA {
    uint16_t species;
    uint16_t held_item;
    uint16_t tid;
    uint16_t sid;
    uint8_t padding[72];
};

struct PK9BlockB {
    uint8_t data[80];
};

struct PK9BlockC {
    uint8_t data[80];
};

struct PK9BlockD {
    uint8_t data[80];
};

struct PK9 {
    uint32_t encryption_constant;
    uint16_t sanity;
    uint16_t checksum;

    union {
        struct {
            PK9BlockA blockA;
            PK9BlockB blockB;
            PK9BlockC blockC;
            PK9BlockD blockD;
        } blocks;
        uint8_t block_data[320];
    };

    uint8_t party_stats[16];
};

#pragma pack(pop)

static_assert(sizeof(PK9) == 344, "PK9 struct size must be 344 bytes");

void decryptPK9(uint8_t* raw_data);
