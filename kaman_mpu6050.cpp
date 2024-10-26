#include <Wire.h>
#include <MPU6050.h>

MPU6050 gyro;

float kalmanFilterX(float measurement);

float Q = 0.01;
float R = 0.1;
float P_x = 1;
float x_hat_x = 0;

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
    float filtered_x = kalmanFilterX(ax_g);

    Serial.print(ax_g);
    Serial.print(", ");
    Serial.println(filtered_x);

    delay(500);
}

float kalmanFilterX(float measurement)
{
    float K = P_x / (P_x + R);
    x_hat_x = x_hat_x + K * (measurement - x_hat_x);
    P_x = (1 - K) * P_x + Q;
    return x_hat_x;
}
