#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define ACCEL_REG_SIZE  0x39  // 57 em hexadecimal
#define ACCEL_WRITE_ADDR 0xA7 //0xA7 = Offset de Leitura do I2C
#define ACCEL_READ_ADDR 0xA6  //0xA6 = Offset de Escrita do I2C
