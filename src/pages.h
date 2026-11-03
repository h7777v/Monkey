#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

extern size_t monkey_os_page_size;

static int monkey_init(void){
#ifdef _WIN32
    SYSTEM_INFO info;
    GetSystemInfo(&info);

    monkey_os_page_size = (size_t)info.dwPageSize;
#else
    long size = sysconf(_SC_PAGESIZE);

    if (size <= 0)
        return -1;

    monkey_os_page_size = (size_t)size;
#endif

    if (monkey_os_page_size == 0)return 1;

    return 0;
}

static void *monkey_get_page(const void *ptr){
    if (ptr == NULL || monkey_os_page_size == 0)
        return NULL;

    uintptr_t address = (uintptr_t)ptr;

    uintptr_t page = address - (address % monkey_os_page_size);

    return (void *)page;
}