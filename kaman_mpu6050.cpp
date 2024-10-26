#include <Wire.h>
#include <MPU6050.h>
#include <SimpleKalmanFilter.h> // Import the Kalman library

MPU6050 gyro;
SimpleKalmanFilter kalmanFilter(0.01, 0.1, 1); // Initialize the Kalman filter with Q, R, and initial P values

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    gyro.initialize();
}

void loop()
{
    int16_t ax, ay, az;
    gyro.getAcceleration(&ax, &ay, &az);

    float ax_g = ax / 16384.0;
    float filtered_x = kalmanFilter.updateEstimate(ax_g); // Pass the raw measurement

    Serial.print(ax_g);
    Serial.print(", ");
    Serial.println(filtered_x);

    delay(500);
}
