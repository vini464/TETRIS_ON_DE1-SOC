#include <stdint.h>
#include "../headers/accel.h"

int main(){
    int16_t value, XYZ_data[3];
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
        if(accel_hadActivity())
        { 
            accel_readXYZ(XYZ_data);
            printf("x: %d - y: %d - z: %d\n", XYZ_data[0], XYZ_data[1], XYZ_data[2]);
    	}
	i++;
	}
    close_and_unmap_dev_mem(fd);
    return 0;
}