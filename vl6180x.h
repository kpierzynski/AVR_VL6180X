#ifndef __VL6180X_H_
#define __VL6180X_H_

#include <avr/io.h>
#include "vl6180x_mem.h"
#include "i2c.h"

#define VL6180X_ADDRESS 0b101001

uint8_t vl6180x_read_reg(uint16_t reg);
void vl6180x_write_reg(uint16_t reg, uint8_t value);

void vl6180x_init();
uint8_t vl6180x_measure();

#endif