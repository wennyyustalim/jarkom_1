#include "PacketData.h"

#include <cstdio>
#include <numeric>

bool PacketData::verify (void) const {
    uint8_t sum = std::accumulate ((uint8_t*) this, (uint8_t*) &checksum, 0);

    fprintf (stderr, "PacketData: checksum=%d calculated=%d\n", checksum, sum);
    return checksum == sum;
}

void PacketData::prepare (void) {
    soh = 0x1;
    checksum = std::accumulate ((uint8_t*) this, (uint8_t*) &checksum, 0);
}
