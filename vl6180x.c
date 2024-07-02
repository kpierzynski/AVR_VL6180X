#include "vl6180x.h"

uint8_t vl6180x_read_reg(uint16_t reg)
{
    uint8_t result;

    i2c_start();
    i2c_write((VL6180X_ADDRESS << 1) | I2C_WRITE);
    i2c_write(reg >> 8);
    i2c_write(reg & 0xFF);
    i2c_start();
    i2c_write((VL6180X_ADDRESS << 1) | I2C_READ);
    result = i2c_read(I2C_NACK);
    i2c_stop();

    return result;
}

void vl6180x_write_reg(uint16_t reg, uint8_t value)
{
    i2c_start();
    i2c_write((VL6180X_ADDRESS << 1) | I2C_WRITE);
    i2c_write(reg >> 8);
    i2c_write(reg & 0xFF);
    i2c_write(value);
    i2c_stop();
}

void vl6180x_init()
{
    uint8_t id = vl6180x_read_reg(VL6180X_REG_IDENTIFICATION_MODEL_ID);
    if (id != 0xB4)
        while (1) // If value of ID register is not 0xB4, then error occur
            ;

    // Those are some magic values and magic registers,
    // because ST don't want to share them for some reason.
    vl6180x_write_reg(0x207, 0x01);
    vl6180x_write_reg(0x208, 0x01);
    vl6180x_write_reg(0x096, 0x00);
    vl6180x_write_reg(0x097, 0xFD);
    vl6180x_write_reg(0x0E3, 0x01);
    vl6180x_write_reg(0x0E4, 0x03);
    vl6180x_write_reg(0x0E5, 0x02);
    vl6180x_write_reg(0x0E6, 0x01);
    vl6180x_write_reg(0x0E7, 0x03);
    vl6180x_write_reg(0x0F5, 0x02);
    vl6180x_write_reg(0x0D9, 0x05);
    vl6180x_write_reg(0x0DB, 0xCE);
    vl6180x_write_reg(0x0DC, 0x03);
    vl6180x_write_reg(0x0DD, 0xF8);
    vl6180x_write_reg(0x09F, 0x00);
    vl6180x_write_reg(0x0A3, 0x3C);
    vl6180x_write_reg(0x0B7, 0x00);
    vl6180x_write_reg(0x0BB, 0x3C);
    vl6180x_write_reg(0x0B2, 0x09);
    vl6180x_write_reg(0x0CA, 0x09);
    vl6180x_write_reg(0x198, 0x01);
    vl6180x_write_reg(0x1B0, 0x17);
    vl6180x_write_reg(0x1AD, 0x00);
    vl6180x_write_reg(0x0FF, 0x05);
    vl6180x_write_reg(0x100, 0x05);
    vl6180x_write_reg(0x199, 0x05);
    vl6180x_write_reg(0x1A6, 0x1B);
    vl6180x_write_reg(0x1AC, 0x3E);
    vl6180x_write_reg(0x1A7, 0x1F);
    vl6180x_write_reg(0x030, 0x00);

    vl6180x_write_reg(VL6180X_REG_SYSRANGE_INTERMEASUREMENT_PERIOD, 0x09);
    vl6180x_write_reg(VL6180X_REG_SYSRANGE_VHV_REPEAT_RATE, 0xFF);
    vl6180x_write_reg(VL6180X_REG_SYSRANGE_VHV_RECALIBRATE, 0x01);

    vl6180x_write_reg(VL6180X_REG_SYSRANGE_MAX_CONVERGENCE_TIME, 0x31);
    vl6180x_write_reg(VL6180X_REG_SYSRANGE_RANGE_CHECK_ENABLES, 0x10 | 0x01);

    vl6180x_write_reg(VL6180X_REG_INTERLEAVED_MODE_ENABLE, 0x00);
    vl6180x_write_reg(VL6180X_REG_READOUT_AVERAGING_SAMPLE_PERIOD, 0x30);

    // Turn interrupt on
    vl6180x_write_reg(VL6180X_REG_SYSTEM_INTERRUPT_CONFIG_GPIO, 4);

    vl6180x_write_reg(VL6180X_REG_SYSTEM_FRESH_OUT_OF_RESET, 0);
}

uint8_t vl6180x_measure()
{
    while (!(vl6180x_read_reg(VL6180X_REG_RESULT_RANGE_STATUS) & 0x01))
        ;

    vl6180x_write_reg(VL6180X_REG_SYSRANGE_START, 0b1);

    uint8_t status;
    while (!(status = vl6180x_read_reg(VL6180X_REG_RESULT_INTERRUPT_STATUS_GPIO) & 0x04))
        ;

    uint8_t range = vl6180x_read_reg(VL6180X_REG_RESULT_RANGE_VAL);

    vl6180x_write_reg(VL6180X_REG_SYSTEM_INTERRUPT_CLEAR, 0x07);

    return range;
}