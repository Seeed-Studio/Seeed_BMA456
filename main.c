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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include "rpi_bma456.h"

int main(int argc, const char* argv[]) {
	rpi_bma456_t rpi_bma[1];
	uint32_t step;
	double x, y, z;

	argc = argc;
	argv[0] = argv[0];

	rpi_bma456_init(rpi_bma,
			"/dev/i2c-1",
			BMA456_I2C_ADDR,
			RANGE_4G,
			ODR_1600_HZ,
			NORMAL_AVG4,
			CONTINUOUS);
	rpi_bma456_enable(rpi_bma, WRIST_CONFIG, 0, BMA4_ENABLE);

	for (;;) {
		step = rpi_bma456_get_counter(rpi_bma);
		printf("Steps:       %5u\n",    step);
		printf("Temperature: %5.2lf\n", rpi_bma456_get_temperature(rpi_bma));
		rpi_bma456_get_accel(rpi_bma, &x, &y, &z);
		printf(" X = %lf Y = %lf Z = %lf\n", x, y, z);
		rpi_bma->bma.delay(1000);
	}
	return 0;
}
