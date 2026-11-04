#include <monkey.h>

#include <stdint.h>
#include <limits.h>
#include <stdio.h>

#include "pages.h"
#include "internal.h"

size_t monkey_os_page_size = 0;

uint64_t seed = 0;

int goof(void* ptr){
    if(ptr == NULL)return 1;
    if(monkey_os_page_size == 0){
        if(monkey_init())return 1;
    }
    void* page = monkey_get_page(ptr);

    if(!seed){
        rng_initialize_seed(&seed);
        rng_seed(seed);
    }

    unsigned char* cooked = (unsigned char *)page + rng_bounded(monkey_os_page_size);

    *cooked = (unsigned char)rng_bounded(UCHAR_MAX + 1);

    return 0;
}

int mgoof(void* ptr, size_t times){
    if(ptr == NULL)return 1;
    if(monkey_os_page_size == 0){
        if(monkey_init())return 1;
    }
    void* page = monkey_get_page(ptr);

    if(!seed){
        rng_initialize_seed(&seed);
        rng_seed(seed);
    }

    unsigned char* cooked;

    for(size_t i = 0; i < times; i++){
        cooked = (unsigned char *)page + rng_bounded(monkey_os_page_size);
        *cooked = (unsigned char)rng_bounded(UCHAR_MAX + 1);
    }

    return 0;
}

int chaos(void* ptr){
    if(ptr == NULL)return 1;
    if(monkey_os_page_size == 0){
        if(monkey_init())return 1;
    }
    
    unsigned char* page = monkey_get_page(ptr);

    if(!seed){
        rng_initialize_seed(&seed);
        rng_seed(seed);
    }

    for(size_t i = 0; i < monkey_os_page_size; i++){
        page[i] = (unsigned char)rng_bounded(UCHAR_MAX + 1);
    }

    return 0;
}

int cosmic_rays(void* ptr){
    if(ptr == NULL)return 1;
    if(monkey_os_page_size == 0){
        if(monkey_init())return 1;
    }
    
    unsigned char* page = monkey_get_page(ptr);

    if(!seed){
        rng_initialize_seed(&seed);
        rng_seed(seed);
    }

    unsigned char* cooked = (unsigned char *)page + rng_bounded(monkey_os_page_size);

    *cooked ^= (unsigned char)(1u << rng_bounded(CHAR_BIT));

    return 0;
}

int solar_flare(void* ptr, size_t category){
    if(ptr == NULL)return 1;
    if(monkey_os_page_size == 0){
        if(monkey_init())return 1;
    }
    
    unsigned char* page = monkey_get_page(ptr);

    if(!seed){
        rng_initialize_seed(&seed);
        rng_seed(seed);
    }

    unsigned char* cooked;

    for(size_t i = 0; i < category; i++){
        cooked = (unsigned char *)page + rng_bounded(monkey_os_page_size);
        *cooked ^= (unsigned char)(1u << rng_bounded(CHAR_BIT));
    }

    return 0;
}

int archeology(void* ptr){
    if(ptr == NULL)return 1;
    if(monkey_os_page_size == 0){
        if(monkey_init())return 1;
    }
    void* page = monkey_get_page(ptr);
    
    printf("--- Found durring archeology mission ---\n");
    printf("Init: %p\n", ptr);
    printf("Page: %p\n", page);
    printf("Size: %zu bytes\n", monkey_os_page_size);

    printf("\nI hope you find something interesting or even ancient fossils.\n");

    printf("\nSoil sample:\n");

    for (size_t i = 0; i < monkey_os_page_size; i += 16) {
        printf("%08zX  ", i);

        for (size_t j = 0; j < 16; j++) {
            if (i + j < monkey_os_page_size)
                printf("%02X ", ((const unsigned char *)page)[i + j]);
            else
                printf("   ");

            if (j == 7)
                putchar(' ');
        }

        printf(" |");

        for (size_t j = 0; j < 16 && i + j < monkey_os_page_size; j++) {
            unsigned char c = ((const unsigned char *)page)[i + j];
            putchar(c >= 32 && c <= 126 ? c : '.');
        }

        printf("|\n");
    }

    return 0;
}