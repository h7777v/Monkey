#include <stdint.h>

static uint64_t rng_state[4];

static inline uint64_t rotl64(uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}

static uint64_t splitmix64(uint64_t *state) {
    uint64_t z = (*state += UINT64_C(0x9E3779B97F4A7C15));

    z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);

    return z ^ (z >> 31);
}

void rng_seed(uint64_t seed) {
    for (int i = 0; i < 4; i++)rng_state[i] = splitmix64(&seed);
}

static inline uint64_t rng_u64(void) {
    const uint64_t result = rotl64(rng_state[0] + rng_state[3], 23) + rng_state[0];

    const uint64_t t = rng_state[1] << 17;

    rng_state[2] ^= rng_state[0];
    rng_state[3] ^= rng_state[1];
    rng_state[1] ^= rng_state[2];
    rng_state[0] ^= rng_state[3];

    rng_state[2] ^= t;
    rng_state[3] = rotl64(rng_state[3], 45);

    return result;
}

uint64_t rng_bounded(uint64_t bound) {
    if (bound == 0)return 0;

    const uint64_t threshold = -bound % bound;

    for (;;) {
        uint64_t r = rng_u64();

        if (r >= threshold)return r % bound;
    }
}