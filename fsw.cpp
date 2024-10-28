#include <Wire.h>
#include <MPU6050.h>
#include <EEPROM.h>

MPU6050 gyro;

float kalmanFilterX(float measurement);

float Q = 0.01;
float R = 0.1;
float P_x = 1;
float x_hat_x = 0;

const int EEPROM_SIZE = 1024; // EEPROM size for Arduino Nano
int currentAddress = 0;       // Start address for EEPROM writing

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    gyro.initialize();

    pinMode(13, OUTPUT);
    if (!gyro.testConnection())
    {
        Serial.println("MPU6050 connection failed!");
        while (1)
            ;
    }

    Serial.println("MPU6050 connected successfully");
}

void morph()
{
    analogWrite(6, 255); // Set LED to maximum brightness
    delay(300);          // Wait for 300 ms
    analogWrite(6, 0);   // Turn the LED off
    delay(300);
}

void loop()
{
    int16_t ax, ay, az;
    gyro.getAcceleration(&ax, &ay, &az);

    // Convert raw acceleration data to g
    float ax_g = ax / 16384.0;
    float filtered_x = kalmanFilterX(ax_g);

    // Print the raw and filtered values
    Serial.print(ax_g);

    // Write filtered X value to EEPROM
    EEPROM.put(currentAddress, ax_g);

    // Increment the EEPROM address
    currentAddress += sizeof(ax_g);

    // Wrap around if EEPROM is full
    if (currentAddress >= EEPROM_SIZE)
    {
        currentAddress = 0;
    }
    morph();
    delay(100); // Loop delay
}

float kalmanFilterX(float measurement)
{
    float K = P_x / (P_x + R);
    x_hat_x = x_hat_x + K * (measurement - x_hat_x);
    P_x = (1 - K) * P_x + Q;
    return x_hat_x;
}
