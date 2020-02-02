
#include "arduino_bma456.h"

uint32_t step = 0;

void setup(void) {
    Serial.begin(115200);
    Serial.println("BMA456 Step Counter");

    bma456.initialize(RANGE_4G, ODR_1600_HZ, NORMAL_AVG4, CONTINUOUS);
    bma456.stepCounterEnable();
}

void loop(void) {
    step = bma456.getStepCounterOutput();

    Serial.print("Step: ");
    Serial.println(step);

    delay(1000);
}
