#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <linux/i2c-dev.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "bme680_pi.h"

int get_i2c_fd(uint8_t dev_id);
int _fd;
bool isInit;

void user_delay_ms(uint32_t period)
{
    usleep(period * 1000);
}

int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    /*
     * Data on the bus should be like
     * |------------+---------------------|
     * | I2C action | Data                |
     * |------------+---------------------|
     * | Start      | -                   |
     * | Write      | (reg_addr)          |
     * | Stop       | -                   |
     * | Start      | -                   |
     * | Read       | (reg_data[0])       |
     * | Read       | (....)              |
     * | Read       | (reg_data[len - 1]) |
     * | Stop       | -                   |
     * |------------+---------------------|
     */
    // __s32 i2c_smbus_read_byte_data(int file, __u8 command);
    //
    for(uint16_t pos = 0; pos < len; pos++) {
        reg_data[pos] = i2c_smbus_read_byte_data(get_i2c_fd(dev_id), reg_addr + pos);
    }

    return BME680_OK;
}

int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    /*
     * Data on the bus should be like
     * |------------+---------------------|
     * | I2C action | Data                |
     * |------------+---------------------|
     * | Start      | -                   |
     * | Write      | (reg_addr)          |
     * | Write      | (reg_data[0])       |
     * | Write      | (....)              |
     * | Write      | (reg_data[len - 1]) |
     * | Stop       | -                   |
     * |------------+---------------------|
     */
    // write
    // __s32 i2c_smbus_write_byte_data(int file, __u8 command, __u8 value);
    uint8_t addr = reg_addr;
    uint8_t val = reg_data[0];
    if(i2c_smbus_write_byte_data(get_i2c_fd(dev_id), reg_addr, val) != 0) {
    printf("fail to write data\n");
        exit(1);
    }

    for(uint16_t pos = 1; pos < len;) {
        addr = reg_data[pos++];
        val = reg_data[pos++];
        if(i2c_smbus_write_byte_data(get_i2c_fd(dev_id), addr, val) != 0) {
        printf("fail to write data\n");
            exit(1);
        }
    }

    return BME680_OK;
}

int get_i2c_fd(uint8_t dev_id) 
{
    if(isInit) {
        return _fd;
    }
    // open device
    char filename[20] = "/dev/i2c-1";
    _fd = open(filename, O_RDWR);
    if (_fd < 0) {
        printf("fail to open %s\n",filename);
        exit(1);
    }

    // set slave address
    if (ioctl(_fd, I2C_SLAVE, dev_id) < 0) {
        printf("fail to set device: %x\n", dev_id);
        exit(1);
    }

    isInit = true;
    return _fd;
}
