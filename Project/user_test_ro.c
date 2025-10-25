// 保存为 user_test_ro.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

static inline unsigned long long ns_now(void){
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
    return (unsigned long long)ts.tv_sec*1000000000ull + ts.tv_nsec;
}

int main(int argc, char **argv){
    const char *dev="/dev/ssdmem0";
    size_t mb = (argc>=2)? strtoull(argv[1],NULL,10) : 4; // 默认4MB
    size_t len = mb*1024ull*1024ull;

    int fd = open(dev, O_RDONLY|O_CLOEXEC);
    if(fd<0){perror("open"); return 1;}

    void *addr = mmap(NULL,len,PROT_READ,MAP_PRIVATE,fd,0); // 只读+私有
    if(addr==MAP_FAILED){perror("mmap"); return 1;}

    volatile unsigned char *p=(volatile unsigned char*)addr;
    size_t step=4096, pages=len/step;

    unsigned long long t0=ns_now();
    for(size_t i=0;i<pages;i++){ volatile unsigned char x=p[i*step]; (void)x; }
    unsigned long long t1=ns_now();

    printf("RO touched %zu pages in %.3f ms (avg %.3f us/page)\n",
           pages,(t1-t0)/1e6,(t1-t0)/1e3/pages);

    munmap(addr,len); close(fd); return 0;
}

