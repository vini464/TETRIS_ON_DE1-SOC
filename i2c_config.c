#define SYSMGR_GENERALIO7 ((volatile unsigned int *) 0xFFD0849C)
#define SYSMGR_GENERALIO8 ((volatile unsigned int *) 0xFFD084A0)
#define SYSMGR_I2C0USEFPGA ((volatile unsigned int *) 0xFFD08704)

void configure_pinmux(){
*SYSMGR_I2C0USEFPGA = 0;
*SYSMGR_GENERALIO7 = 1;
*SYSMGR_GENERALIO8 = 1;
}