#include "pk9.h"
#include <string.h>

static const uint8_t BlockPosition[32 * 4] = {
    0, 1, 2, 3,  0, 1, 3, 2,  0, 2, 1, 3,  0, 3, 1, 2,
    0, 2, 3, 1,  0, 3, 2, 1,  1, 0, 2, 3,  1, 0, 3, 2,
    2, 0, 1, 3,  3, 0, 1, 2,  2, 0, 3, 1,  3, 0, 2, 1,
    1, 2, 0, 3,  1, 3, 0, 2,  2, 1, 0, 3,  3, 1, 0, 2,
    2, 3, 0, 1,  3, 2, 0, 1,  1, 2, 3, 0,  1, 3, 2, 0,
    2, 1, 3, 0,  3, 1, 2, 0,  2, 3, 1, 0,  3, 2, 1, 0,

    // duplicates of 0-7 to eliminate modulus (32 => 24)
    0, 1, 2, 3,  0, 1, 3, 2,  0, 2, 1, 3,  0, 3, 1, 2,
    0, 2, 3, 1,  0, 3, 2, 1,  1, 0, 2, 3,  1, 0, 3, 2,
};

static void cryptArray(uint8_t* data, size_t length, uint32_t seed) {
    uint16_t* u16_data = (uint16_t*)data;
    size_t count = length / 2;
    for (size_t i = 0; i < count; i++) {
        seed = (0x41C64E6D * seed) + 0x00006073;
        uint16_t xor_val = (uint16_t)(seed >> 16);
        u16_data[i] ^= xor_val;
    }
}

static void shuffle8(uint8_t* data, uint32_t sv) {
    if (sv == 0) return;

    uint8_t perm[4];
    uint8_t slotOf[4];

    for (uint8_t s = 0; s < 4; s++) {
        perm[s] = s;
        slotOf[s] = s;
    }

    const uint8_t* shuffle = &BlockPosition[sv * 4];

    uint64_t* u64_data = (uint64_t*)data;
    size_t count = 80 / 8; // 10 uint64_t per block

    for (uint8_t i = 0; i < 3; i++) {
        uint8_t desired = shuffle[i];
        uint8_t j = slotOf[desired];

        if (j == i) continue;

        for (size_t k = 0; k < count; k++) {
            uint64_t temp = u64_data[i * count + k];
            u64_data[i * count + k] = u64_data[j * count + k];
            u64_data[j * count + k] = temp;
        }

        uint8_t blockAtI = perm[i];
        perm[j] = blockAtI;
        slotOf[blockAtI] = j;
    }
}

void decryptPK9(uint8_t* raw_data) {
    PK9* pk = (PK9*)raw_data;
    uint32_t pv = pk->encryption_constant;
    uint32_t sv = (pv >> 13) & 31;

    cryptArray(pk->block_data, sizeof(pk->block_data), pv);
    cryptArray(pk->party_stats, sizeof(pk->party_stats), pv);
    shuffle8(pk->block_data, sv);
}
