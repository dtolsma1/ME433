#ifndef MCP23017_H__
#define MCP23017_H__

#include "i2c_master_noint.h"

#define MCP_ADDRESS 0b01000000  // 7bit address of mcp23017, blank R/W bit at end
#define IODIRA_register 0x00
#define IODIRB_register 0x01
#define OLATA_register 0x14
#define GPIOB_register 0x13

//const unsigned char MCP_ADDRESS = 0b01000000;  // 7bit address of mcp23017, blank R/W bit at end
//const unsigned char IODIRA_register = 0x00;
//const unsigned char IODIRB_register = 0x01;
//const unsigned char OLATA_register = 0x14;
//const unsigned char GPIOB_register = 0x13;

void setPin(unsigned char address, unsigned char reg, unsigned char value); // write fcn
unsigned char readPin(unsigned char address, unsigned char reg); //read fcn
void clearPin(unsigned char address, unsigned char reg, int pin);
void mcp_init_forHW3(void);

#endif