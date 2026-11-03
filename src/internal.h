#pragma once

#include <stdint.h>

int rng_initialize_seed(uint64_t *seed);

uint64_t rng_bounded(uint64_t bound);
void rng_seed(uint64_t seed);