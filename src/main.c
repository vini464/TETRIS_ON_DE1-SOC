#include <stdint.h>
#include <stdio.h>
#include "../headers/accel.h"

int main(){
    int16_t value = 0, XYZ_data[3];
    int i = 0;
    int fd = open_and_mmap_dev_mem();
    if (fd == -1) {
	    return -1;
    }
    I2C0_init();
    accel_init();
    accel_calibrate(128);
    while (i < 100000)
    {   
	get_direction(&value);
	printf("Direction: %d \n",value);
	i++;
    }
    close_and_unmap_dev_mem(fd);
    return 0;
}
