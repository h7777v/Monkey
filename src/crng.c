#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <bcrypt.h>

#ifdef _MSC_VER
#pragma comment(lib, "bcrypt.lib")
#endif

static int rng_get_os_entropy(uint64_t *seed) {
    return BCryptGenRandom(
        NULL,
        (PUCHAR)seed,
        sizeof(*seed),
        BCRYPT_USE_SYSTEM_PREFERRED_RNG
    ) == 0;
}

#else

#include <unistd.h>
#if defined(__APPLE__)
#include <sys/random.h>
#endif

static int rng_get_os_entropy(uint64_t *seed) {
#if defined(__linux__) || defined(__APPLE__) || \
    defined(__FreeBSD__) || defined(__OpenBSD__)
    if (getentropy(seed, sizeof(*seed)) == 0)
        return 1;
#endif

    FILE *f = fopen("/dev/urandom", "rb");

    if (!f)
        return 0;

    setvbuf(f, NULL, _IONBF, 0)

    int ok = fread(seed, sizeof(*seed), 1, f) == 1;
    fclose(f);

    return ok;
}

#endif

static uint64_t rng_mix(uint64_t x) {
    x ^= x >> 30;
    x *= UINT64_C(0xBF58476D1CE4E5B9);
    x ^= x >> 27;
    x *= UINT64_C(0x94D049BB133111EB);
    x ^= x >> 31;

    return x;
}

static uint64_t rng_fallback_seed(void) {
    struct timespec ts = {0, 0};
    timespec_get(&ts, TIME_UTC);

    uint64_t x = rng_mix((uint64_t)ts.tv_sec);
    x = rng_mix(x ^ (uint64_t)ts.tv_nsec);
    x = rng_mix(x ^ (uint64_t)clock());
    x = rng_mix(x ^ (uint64_t)(uintptr_t)&ts);

    return x;
}

int rng_initialize_seed(uint64_t *seed) {
    if (!seed)
        return 0;

    int ok = rng_get_os_entropy(seed);

    if (!ok)
        *seed = rng_fallback_seed();

    if (*seed == 0)
        *seed = UINT64_C(0x9E3779B97F4A7C15);

    return ok;
}