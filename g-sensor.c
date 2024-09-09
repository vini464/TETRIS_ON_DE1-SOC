#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "address_map_arm.h"

#define ACCEL_REG_SIZE  0x39  // 57 em hexadecimal
#define ACCEL_WRITE_ADDR 0xA7 //0xA7 = Offset de Leitura do I2C
#define ACCEL_READ_ADDR 0xA6  //0xA6 = Offset de Escrita do I2C
int fd = -1;

int open_physical (int){
    if (fd == -1) {
        if ((fd = open("/dev/mem",(O_RDWR | O_SYNC))) == -1){
            printf ("ERROR: could not open \ ""/dev/mem""\n");
            return -1;
        }
    return fd;
    }
}

void close_physical (int fd){
    close(fd);
}

void * map_physical(int fd, unsigned int base, unsigned int span) {
	void * virtual_base;

	// Get a mapping from physical addresses to virtual addresses
	virtual_base = mmap(NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, base);
	if (virtual_base == NULL) {
		printf("ERROR: mmap() failed...\n");
		close(fd);
		return(NULL);
	}

	return virtual_base;
}

int unmap_physical(void * virtual_base, unsigned int span)
{
   if (munmap (virtual_base, span) != 0)
   {
      printf ("ERROR: munmap() failed...\n");
      return (-1);
   }
   return 0;
}

