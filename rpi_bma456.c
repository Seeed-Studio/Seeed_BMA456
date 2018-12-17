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
#define _DEBUG	0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include "rpi_bma456.h"

static int rpi_i2c_fd = -1;

static int rpi_i2c_init(const char* dev_path) {
	int fd;

	if ((fd = rpi_i2c_fd) >= 0) {
		return fd;
	}

	if ((fd = open(dev_path, O_RDWR)) < 0) {
		printf("Failed to open i2c bus %s, error = %d\n", dev_path, fd);
	} else {
		rpi_i2c_fd = fd;
	}
	return fd;
}

// return none-zero == FAIL
//        zero      == OK
#define RPI_I2C_OK	0
#define RPI_I2C_FAIL	1
static uint16_t rpi_i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len) {
	uint8_t* buf = malloc(len + 1);
	int rt;

	if (buf == NULL || rpi_i2c_fd < 0) {
		return RPI_I2C_FAIL;
	}

	if ((rt = ioctl(rpi_i2c_fd, I2C_SLAVE, dev_addr) < 0)) {
		printf("Failed to talk to slave %02X, error = %d.\n", dev_addr, rt);
		return RPI_I2C_FAIL;
	}

	buf[0] = reg_addr;
	memcpy(buf + 1, data, len);

	if ((rt = write(rpi_i2c_fd, buf, len + 1)) != len + 1) {
		printf("Failed to write i2c bus with error = %d.\n", rt);
		rt = RPI_I2C_FAIL;
	} else {
		rt = RPI_I2C_OK;
	}

	free(buf);
	return rt;
}

// return none-zero == FAIL
//        zero      == OK
static uint16_t rpi_i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len) {
	int rt;

	if (rpi_i2c_fd < 0) {
		return RPI_I2C_FAIL;
	}

	if ((rt = ioctl(rpi_i2c_fd, I2C_SLAVE, dev_addr) < 0)) {
		printf("Failed to talk to slave %02X, error = %d.\n", dev_addr, rt);
		return RPI_I2C_FAIL;
	}

	if ((rt = write(rpi_i2c_fd, &reg_addr, 1)) != 1) {
		printf("Failed to write i2c bus with error = %d.\n", rt);
		return RPI_I2C_FAIL;
	}

	if ((rt = read(rpi_i2c_fd, data, len)) != len) {
		printf("Failed to read from i2c bus with error = %d.\n", rt);
		return RPI_I2C_FAIL;
	}
	return RPI_I2C_OK;
}

static void rpi_delay_ms(uint32_t millis) {
	usleep(millis * 1000UL);
	return;
}

void* rpi_bma456_alloc(void) {
	return malloc(sizeof(rpi_bma456_t));
}

int rpi_bma456_free(rpi_bma456_t* dev) {
	free(dev);
	return 0;
}

int rpi_bma456_init(
	rpi_bma456_t* dev,
	const char* i2c_dev,
	int bma_addr,
	BMA456_RANGE range,
	BMA456_ODR odr,
	BMA456_BW bw,
	BMA456_PERF_MODE mode
) {
	/* Declare an instance of the BMA4xy device */
	int rt = BMA4_OK;

	#if _DEBUG
	printf("%s() +++\n", __func__);
	#endif

	if ((rt = rpi_i2c_init(i2c_dev)) < 0) {
		return BMA4_E_FAIL;
	}

	/* fill device parameters */
	dev->bma.dev_addr 	= bma_addr;
	dev->bma.interface 	= BMA4_I2C_INTERFACE;
	dev->bma.bus_read 	= rpi_i2c_read;
	dev->bma.bus_write 	= rpi_i2c_write;
	dev->bma.delay 		= rpi_delay_ms;
	dev->bma.read_write_len	= 32;

	/* a. Reading the chip id. */
	rt = bma456_init(&dev->bma);

	/* reset device */
	bma4_set_command_register(0xB6, &dev->bma);
	dev->bma.delay(10);

	#if _DEBUG
	printf("%s() L%d\n", __func__, __LINE__);
	#endif
	bma456_write_config_file(&dev->bma);
	#if _DEBUG
	printf("%s() L%d\n", __func__, __LINE__);
	#endif

	dev->acc_conf.odr       = (uint8_t)odr;
	dev->acc_conf.range     = (uint8_t)range;
	dev->acc_conf.bandwidth = (uint8_t)bw;
	dev->acc_conf.perf_mode = (uint8_t)mode;
	bma4_set_accel_config(&dev->acc_conf, &dev->bma);

	switch (range) {
	case RANGE_2G:
		dev->range = 2000.0; break;
	case RANGE_4G:
		dev->range = 4000.0; break;
	case RANGE_8G:
		dev->range = 8000.0; break;
	case RANGE_16G:
	default:
		dev->range = 16000.0;break;
	}
	bma4_set_accel_enable(BMA4_ENABLE, &dev->bma);

	#if _DEBUG
	printf("%s() ---\n", __func__);
	#endif
	return BMA4_OK;
}

int rpi_bma456_enable(
	rpi_bma456_t* dev,
	BMA456_PLATFORM_CONF plat,
	BMA456_PATTERN_t pattern,
	int cmd
) {
	bma456_reset_step_counter(&dev->bma);
	bma456_select_platform(plat, &dev->bma);
	bma456_feature_enable(BMA456_STEP_CNTR | pattern, cmd, &dev->bma);
	return BMA4_OK;
}

int rpi_bma456_get_accel(
	rpi_bma456_t* dev,
	double* x, double* y, double* z
) {
	struct bma4_accel d;
    
	bma4_read_accel_xyz(&d, &dev->bma);
    
	*x = d.x * dev->range / 32768;
	*y = d.y * dev->range / 32768;
	*z = d.z * dev->range / 32768;
	return BMA4_OK;
}

double rpi_bma456_get_temperature(
	rpi_bma456_t* dev
) {
	int32_t t = 0;

	bma4_get_temperature(&t, BMA4_DEG, &dev->bma);

	return t / 1000.0;
}

uint32_t rpi_bma456_get_counter(
	rpi_bma456_t* dev
) {
	uint32_t step = 0;

	bma456_step_counter_output(&step, &dev->bma);
	return step;
}

#ifdef _HAS_MAIN
#include "main.c"
#endif
