#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include "accel_register_map.h"

#define I2C0_BASE_ADDR 0xFFC04000  // Endereço base do controlador I2C0
#define I2C0_REG_SIZE  0x1000      // Tamanho da região mapeada (4KB)

typedef int bool;
#define false 0;
#define true 1;

#define ROUNDED_DIVISION(num, divisor) (((num) + (divisor / 2)) / divisor)


volatile uint32_t *i2c0_regs;

// Função para abrir especificamente o /dev/mem e retornar o descritor
int open_physical() {
    int fd;

    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        printf("Erro ao abrir /dev/mem");
        return -1;
    }

    return fd;
}

void close_physical(int fd) {
    close(fd);
}

// Função para mapear a região de memória do I2C0
int mmap_i2c0(int fd) {
    i2c0_regs = (volatile uint32_t *) mmap(NULL, I2C0_REG_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, I2C0_BASE_ADDR);
    if (i2c0_regs == MAP_FAILED) {
        printf("Erro ao mapear /dev/mem");
        return -1;
    }
    return 0;
}

// Função para abrir e mapear /dev/mem
int open_and_mmap_dev_mem() {
    int fd = open_physical();
    if (fd == -1) {
        return -1;
    }

    if (mmap_i2c0(fd) == -1) {
        close(fd);
        return -1;
    }

    return fd;
}

// Função para fechar e liberar o mapeamento
void close_and_unmap_dev_mem(int fd) {
    if (i2c0_regs) {
        munmap((void *)i2c0_regs, I2C0_REG_SIZE);
    }
    close_physical(fd);
}

#define I2C0_CON                0x00
#define I2C0_TAR                0x04
#define I2C0_DATA_CMD           0x10
#define I2C0_FS_SCL_HCNT        0x1C
#define I2C0_FS_SCL_LCNT        0x20
#define I2C0_CLR_INTR           0x40
#define I2C0_ENABLE             0x6C
#define I2C0_TXFLR              0x74
#define I2C0_RXFLR              0x78
#define I2C0_ENABLE_STATUS      0x9C



#define I2C0_SS_SCL_HCNT        0x14
#define I2C0_SS_SCL_LCNT        0x28
#define I2C0_INTR_MASK          0x30
#define I2C0_TX_ABRT            0x54
#define I2C0_TX_ABRT_SOURCE     0x80

void write_register(volatile uint32_t *base, uint32_t offset, uint32_t value){
    base[offset / 4] = value;

}

uint32_t read_register(volatile uint32_t *base, uint32_t offset) {
    return base[offset / 4];
}


void I2C0_init(){
    write_register(i2c0_regs,I2C0_ENABLE, 0x02); //Aborta qualquer comunicação
    while ((read_register(i2c0_regs,I2C0_ENABLE_STATUS) & 0x1) == 1 )
    {} //Espera a comunicação voltar ao normal
    write_register(i2c0_regs,I2C0_CON, 0x65); //Configura HPS como master, fast mode (400kb/s) e 7 bit adressing
    write_register(i2c0_regs,I2C0_TAR, 0x53); // Coloca o acelerometro como target.
    
    write_register(i2c0_regs,I2C0_FS_SCL_HCNT, 60+30);  //
    write_register(i2c0_regs,I2C0_FS_SCL_LCNT, 130+30); //

    write_register(i2c0_regs,I2C0_ENABLE, 0x01);
    while ((read_register(i2c0_regs,I2C0_ENABLE_STATUS) & 0x1) == 0 )
    {}
}

bool test_communication() {

    uint8_t device_id;
    accel_reg_read(DEVID, &device_id);

    if (device_id == 0xE5){
        //printf("Device ID: 0x%02X\n", device_id);
        return true;
        //levar para um
        //printf("Comunicacao estabelecida!")
    }
    return false;
}

void accel_init() {
    accel_reg_write(DATA_FORMAT, 0x03 | 0x08); //Coloca o formato de dados em 0x03 (+-16g) e resolução completa
    accel_reg_write(BW_RATE, 0x0B); //Coloca o fluxo de saida de dados em 200Hz
    accel_reg_write(THRESH_ACT,0x10); // Calibra a detecção de movimento(62,5 mg por unidade em g (gravidade da Terra))
    accel_reg_write(INT_ENABLE,0x80 | 0x10); // Permitir detecção de Data_ready e Activity
    accel_reg_write(POWER_CTL,0x00); //Seta o power para 0, parando
    accel_reg_write(POWER_CTL,0x08); //Inicia a medição.
}

void accel_calibrate(int average_index){

    int average_x = 0;
    int average_y = 0;
    int average_z = 0;
    int16_t XYZ[3];
    int8_t offset_x;
    int8_t offset_y;
    int8_t offset_z;

    accel_reg_write(POWER_CTL,0x00);
    
    accel_reg_read(OFSX,(uint8_t *)&offset_x);
    accel_reg_read(OFSY,(uint8_t *)&offset_y);
    accel_reg_read(OFSZ,(uint8_t *)&offset_z);

    uint8_t saved_bw;
    accel_reg_read(BW_RATE, &saved_bw);
    accel_reg_write(BW_RATE, 0x0a);

    uint8_t saved_dataF;
    accel_reg_read(DATA_FORMAT, &saved_dataF);
    accel_reg_write(DATA_FORMAT, 0x03 | 0x08);

    accel_reg_write(POWER_CTL,0x08);
    int i = 0;
    while(i < average_index){
        if (accel_isDataReady())
        {
            accel_readXYZ(XYZ);
            average_x += XYZ[0];
            average_y += XYZ[1];
            average_z += XYZ[2];
            i++;
        }  
    }
    average_x = average_x / average_index; //Mudar por rounded divisio
    average_y = average_y / average_index;
    average_z = average_z / average_index;

    accel_reg_write(POWER_CTL,0x00);

    printf("Average X=%d, Y=%d, Z=%d\n", average_x, average_y, average_z);

    offset_x += (0-average_x) / 4;
    offset_y += (0-average_y) / 4;
    offset_z += (256-average_z) / 4;
    
    accel_reg_write(OFSX,offset_x);
    accel_reg_write(OFSY,offset_y);
    accel_reg_write(OFSZ,offset_z);

    accel_reg_write(BW_RATE,saved_bw);
    accel_reg_write(DATA_FORMAT,saved_dataF);
    accel_reg_write(POWER_CTL,0x08);
}

bool accel_isDataReady(){
    bool bReady = false;
    uint8_t data8;

    accel_reg_read(INT_SOURCE,&data8);
     if (data8 & (0x80))
        bReady = true;
    
    return bReady;
}

bool accel_hadActivity(){
    bool bReady = false;
    uint8_t data8;

    accel_reg_read(INT_SOURCE,&data8);
     if (data8 & (0x10))
        bReady = true;
    
    return bReady;
}

void accel_reg_write(uint8_t address, uint8_t value){
    
    write_register(i2c0_regs,I2C0_DATA_CMD, address + 0x400);
    write_register(i2c0_regs,I2C0_DATA_CMD, value);
}

void accel_reg_read(uint8_t address, uint8_t *value){
    write_register(i2c0_regs,I2C0_DATA_CMD, address + 0x400);
    write_register(i2c0_regs,I2C0_DATA_CMD, 0x100);
    while (read_register(i2c0_regs,I2C0_RXFLR) == 0)
    {}
    *value = (uint8_t)read_register(i2c0_regs,I2C0_DATA_CMD);
}

void I2C_readXYZ(uint8_t address,uint8_t values[],uint8_t len){

    write_register(i2c0_regs,I2C0_DATA_CMD, address + 0x400); //Envia um sinal de start para o primeiro registrador

    int i = 0;

    for(i;i<len;i++){
        write_register(i2c0_regs,I2C0_DATA_CMD, 0x100); //Envia um sinal de leitura para os proximos 6 registradores
    }
    int byte_index = 0; //Cria uma variavel para percorrer o array guardado no buffer do I2C

    while (read_register(i2c0_regs,I2C0_RXFLR) < len){}

    while (len)
    {
        if (read_register(i2c0_regs,I2C0_RXFLR)>0) //Checka o buffer
        {
            values[byte_index] = (uint8_t)read_register(i2c0_regs,I2C0_DATA_CMD);
            byte_index++;
            len--;
        }
    }
}

void accel_readXYZ(int16_t XYZ_Data[3]) {
    uint8_t XYZ_8bits[6];
    I2C_readXYZ(DATA_X0, (uint8_t *)&XYZ_8bits, 6);

    XYZ_Data[0] = (XYZ_8bits[1] << 8) | XYZ_8bits[0]; 
    XYZ_Data[1] = (XYZ_8bits[3] << 8) | XYZ_8bits[2];
    XYZ_Data[2] = (XYZ_8bits[5] << 8) | XYZ_8bits[4];

}

int main(){
    int16_t value, XYZ_data[3];
    printf(".");
    int i = 0;
    int fd = open_and_mmap_dev_mem();
    if (fd == -1) {
	    return -1;
    }
    I2C0_init();
    accel_init();
    accel_calibrate(32);
    while (i < 100000)
        {   
            accel_readXYZ(XYZ_data);
            printf("x: %d - y: %d - z: %d\n", XYZ_data[0], XYZ_data[1], XYZ_data[2]);
	        i++;
        }
    close_and_unmap_dev_mem(fd);
    return 0;
}






