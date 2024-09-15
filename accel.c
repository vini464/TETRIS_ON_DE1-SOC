#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include "accel_register_map.h"

#define I2C0_BASE_ADDR 0xFFC04000  // Endereço base do controlador I2C0
#define I2C0_REG_SIZE  0x1000      // Tamanho da região mapeada (4KB)

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

int test_communication() {
    // Abre e mapeia /dev/mem
    int fd = open_and_mmap_dev_mem();
    if (fd == -1) {
        return -1;
    }

    // Inicializa o I2C0
    I2C0_init();

    // Lê o ID do acelerômetro (registro 0x00)
    uint8_t device_id;
    accel_reg_read(DEVID, &device_id);

    // Exibe o valor lido
    printf("Device ID: 0x%02X\n", device_id);

    // Fecha e desmapeia /dev/mem
    close_and_unmap_dev_mem(fd);

    return 0;
}

void accel_init() {
    accel_reg_write(DATA_FORMAT, 0x03 | 0x08); //Coloca o formato de dados em 0x03 (+-16g) e resolução completa
    accel_reg_write(BW_RATE, 0x0B); //Coloca o fluxo de saida de dados em 200Hz
    accel_reg_write(THRESH_ACT,0x06); // Calibra a detecção de movimento(62,5 mg por unidade em g (gravidade da Terra))

    accel_reg_write(POWER_CTL,0x00); //Seta o power para 0, parando
    accel_reg_write(POWER_CTL,0x08); //Inicia a medição.

}

bool accel_activity_update(){
    bool bReady = false;
    uint8_t data8;

    accel_reg_read(INT_SOURCE,&data8)
     if (data8 & (1 << 4))
        bReady = true;
    
    return bReady;
}

void accel_read_one_axies(uint8_t address, uint8_t *value){
    uint8_t data8;
    accel_reg_read(address,data8);
    *value = data8;

}

int main(){
    *uint8_t value;
    if (test_communication == 0){
        while (1)
        {
        
            accel_read_one_axies(DATA_X0,*value);
            printf(value);
        }
    }
}






