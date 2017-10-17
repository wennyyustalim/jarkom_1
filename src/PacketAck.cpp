#include "PacketAck.h"

#include <numeric>

bool PacketAck::verify (void) const {
    uint8_t sum = std::accumulate ((uint8_t*) this, (uint8_t*) &checksum, 0);
    return checksum == sum;
}

void PacketAck::prepare (void) {
    checksum = std::accumulate ((uint8_t*) this, (uint8_t*) &checksum, 0);
}
