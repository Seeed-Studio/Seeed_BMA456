/*    
 * A library for Grove - Step Counter(BMA456)
 *   
 * Copyright (c 2018 seeed technology co., ltd.  
 * Author      : Peter Yang
 * Create Time : Dec 2018
 * Change Log  :
 *     11:09 2018/12/14 Initial version
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software", to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef __RPI_BMA456_H__
#define __RPI_BMA456_H__

#include "bma456.h"

#define BMA456_I2C_ADDR		0x19

typedef enum
{
	CIC_AVG = 0,
	CONTINUOUS,
} BMA456_PERF_MODE;

typedef enum
{
	OSR4_AVG1 = 0,
	OSR2_AVG2,
	NORMAL_AVG4,
	CIC_AVG8,
	RES_AVG16,
	RES_AVG32,
	RES_AVG64,
	RES_AVG128,
} BMA456_BW;

typedef enum
{
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
} BMA456_ODR;

typedef enum
{
	RANGE_2G = 0,
	RANGE_4G,
	RANGE_8G,
	RANGE_16G,
} BMA456_RANGE;

typedef enum
{
	PHONE_CONFIG = 0,
	WRIST_CONFIG = 1,
} BMA456_PLATFORM_CONF;

typedef enum
{
	PATTERN_ANY_MOTION = BMA456_ANY_MOTION,
	PATTERN_NO_MOTION  = BMA456_NO_MOTION,
	PATTERN_ACTIVITY   = BMA456_ACTIVITY,
	PATTERN_WRIST_TILT = BMA456_WRIST_TILT,
	PATTERN_WAKEUP     = BMA456_WAKEUP,
} BMA456_PATTERN_t;

typedef struct {
	struct bma4_dev bma;
	struct bma4_accel_config acc_conf;
	double range;
} rpi_bma456_t;

void* rpi_bma456_alloc(void);
int rpi_bma456_free(rpi_bma456_t* dev);

extern int rpi_bma456_init(
	rpi_bma456_t* dev,
	const char* i2c_dev,
	int bma_addr,
	BMA456_RANGE range,
	BMA456_ODR odr,
	BMA456_BW bw,
	BMA456_PERF_MODE mode
);

extern int rpi_bma456_enable(
	rpi_bma456_t* dev,
	BMA456_PLATFORM_CONF plat,
	BMA456_PATTERN_t pattern,
	int cmd
);

extern int rpi_bma456_get_accel(
	rpi_bma456_t* dev,
	double* x, double* y, double* z
);

extern double rpi_bma456_get_temperature(
	rpi_bma456_t* dev
);

extern uint32_t rpi_bma456_get_counter(
	rpi_bma456_t* dev
);

#endif//__RPI_BMA456_H__
