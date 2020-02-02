/*
    A library for Grove - Step Counter(BMA456)

    Copyright (c) 2018 seeed technology co., ltd.
    Author      : Wayen Weng
    Create Time : June 2018
    Change Log  :

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include "arduino_bma456.h"

#ifdef __AVR__

uint8_t config_file[8] = { 0 };

#endif

static uint16_t bma_i2c_write(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t len) {
    Wire.beginTransmission(addr);
    Wire.write(reg);
    for (uint16_t i = 0; i < len; i++) {
        Wire.write(data[i]);
    }
    Wire.endTransmission();

    return 0;
}

static uint16_t bma_i2c_read(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t len) {
    uint16_t i = 0;

    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom((int16_t)addr, len);
    while (Wire.available()) {
        data[i++] = Wire.read();
    }

    return 0;
}

static void bma_delay_ms(uint32_t ms) {
    delay(ms);
}

void BMA456::initialize(MA456_RANGE range, MBA456_ODR odr, MA456_BW bw, MA456_PERF_MODE mode) {
    Wire.begin();

    accel.dev_addr        = BMA4_I2C_ADDR_SECONDARY;
    accel.interface       = BMA4_I2C_INTERFACE;
    accel.bus_read        = bma_i2c_read;
    accel.bus_write       = bma_i2c_write;
    accel.delay           = bma_delay_ms;
    accel.read_write_len  = 8;
    accel.resolution      = 16;
    accel.feature_len     = BMA456_FEATURE_SIZE;

    bma456_init(&accel);

    bma4_set_command_register(0xB6, &accel); // reset device

    delay(500); // wait for POR finish

    bma456_write_config_file(&accel);

    accel_conf.odr = (uint8_t)odr;
    accel_conf.range = (uint8_t)range;
    accel_conf.bandwidth = (uint8_t)bw;
    accel_conf.perf_mode = (uint8_t)mode;

    bma4_set_accel_config(&accel_conf, &accel);

    if (range == RANGE_2G) {
        devRange = 2000;
    } else if (range == RANGE_4G) {
        devRange = 4000;
    } else if (range == RANGE_8G) {
        devRange = 8000;
    } else if (range == RANGE_16G) {
        devRange = 16000;
    }

    bma4_set_accel_enable(BMA4_ENABLE, &accel);
}

void BMA456::stepCounterEnable(MA456_PLATFORM_CONF conf, bool cmd) {
    bma456_reset_step_counter(&accel);
    bma456_select_platform(conf, &accel);
    bma456_feature_enable(BMA456_STEP_CNTR, cmd, &accel);
}

void BMA456::getAcceleration(float* x, float* y, float* z) {
    struct bma4_accel sens_data;

    bma4_read_accel_xyz(&sens_data, &accel);

    *x = (float)sens_data.x * devRange / 32768;
    *y = (float)sens_data.y * devRange / 32768;
    *z = (float)sens_data.z * devRange / 32768;
}

int32_t BMA456::getTemperature(void) {
    int32_t temp = 0;

    bma4_get_temperature(&temp, BMA4_DEG, &accel);

    return (temp / 1000);
}

uint32_t BMA456::getStepCounterOutput(void) {
    uint32_t step = 0;

    bma456_step_counter_output(&step, &accel);

    return step;
}

BMA456 bma456;
