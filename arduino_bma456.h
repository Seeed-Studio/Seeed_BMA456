/*
    A library for Grove - Step Counter(BMA456)

    Copyright (c 2018 seeed technology co., ltd.
    Author      : Wayen Weng
    Create Time : June 2018
    Change Log  :

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software", to deal
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

#ifndef __BOSCH_BMA456_H__
#define __BOSCH_BMA456_H__

#include <Arduino.h>
#include <Wire.h>
#include "bma456.h"

typedef enum {
    CIC_AVG = 0,
    CONTINUOUS,
} MA456_PERF_MODE;

typedef enum {
    OSR4_AVG1 = 0,
    OSR2_AVG2,
    NORMAL_AVG4,
    CIC_AVG8,
    RES_AVG16,
    RES_AVG32,
    RES_AVG64,
    RES_AVG128,
} MA456_BW;

typedef enum {
    ODR_0_78_HZ = 1,
    ODR_1_5_HZ,
    ODR_3_1_HZ,
    ODR_6_25_HZ,
    ODR_12_5_HZ,
    ODR_25_HZ,
    ODR_50_HZ,
    ODR_100_HZ,
    ODR_200_HZ,
    ODR_400_HZ,
    ODR_800_HZ,
    ODR_1600_HZ,
} MBA456_ODR;

typedef enum {
    RANGE_2G = 0,
    RANGE_4G,
    RANGE_8G,
    RANGE_16G,
} MA456_RANGE;

typedef enum {
    PHONE_CONFIG = 0,
    WRIST_CONFIG = 1,
} MA456_PLATFORM_CONF;

class BMA456 {
  public:

    BMA456(void) {}

    void initialize(MA456_RANGE range = RANGE_4G, MBA456_ODR odr = ODR_100_HZ,
                    MA456_BW bw = NORMAL_AVG4, MA456_PERF_MODE mode = CIC_AVG);

    void stepCounterEnable(MA456_PLATFORM_CONF conf = WRIST_CONFIG, bool cmd = true);

    void getAcceleration(float* x, float* y, float* z);

    int32_t getTemperature(void);

    uint32_t getStepCounterOutput(void);

  private:

    float devRange;
    struct bma4_dev accel;
    struct bma4_accel_config accel_conf;

};

extern BMA456 bma456;

#endif