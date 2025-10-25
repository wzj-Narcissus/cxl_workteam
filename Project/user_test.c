// user_test.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>     // ✅ strtoull 在这里
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>

static inline uint64_t ns_now(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ull + ts.tv_nsec;
}

int main(int argc, char **argv) {
    const char *dev = "/dev/ssdmem0";
    size_t map_mb = 64; // 默认映射64MB
    if (argc >= 2) map_mb = strtoull(argv[1], NULL, 10);

    size_t length = map_mb * 1024ull * 1024ull;

    int fd = open(dev, O_RDWR | O_CLOEXEC);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    void *addr = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    printf("mapped %zu MB at %p, touching every 4KB...\n", map_mb, addr);

    uint64_t t0 = ns_now();
    volatile uint8_t *p = (volatile uint8_t *)addr;
    size_t step = 4096;
    size_t pages = length / step;
    for (size_t i = 0; i < pages; i++) {
       volatile uint8_t x = p[i * step]; (void)x;// 触发缺页：写每个页的第一个字节
    }
    uint64_t t1 = ns_now();
    printf("touched %zu pages in %.3f ms (avg %.3f us/page)\n",
           pages, (t1 - t0)/1e6, (t1 - t0)/1e3 / pages);

    munmap(addr, length);
    close(fd);
    return 0;
}

