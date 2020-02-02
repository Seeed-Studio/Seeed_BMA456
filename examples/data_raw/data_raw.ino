
#include "arduino_bma456.h"

float x = 0, y = 0, z = 0;
int32_t temp = 0;

void setup(void) {
    Serial.begin(115200);
    Serial.println("BMA456 Raw Data");

    bma456.initialize();
}

void loop(void) {
    bma456.getAcceleration(&x, &y, &z);
    temp = bma456.getTemperature();

    Serial.print("X: ");
    Serial.print(x);
    Serial.print(", Y: ");
    Serial.print(y);
    Serial.print(", Z: ");
    Serial.print(z);
    Serial.print(", T: ");
    Serial.println(temp);

    delay(10);
}
