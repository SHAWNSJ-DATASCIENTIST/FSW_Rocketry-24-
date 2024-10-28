#include <EEPROM.h>

const int EEPROM_SIZE = 1024;         // EEPROM size for Arduino Nano
const int VALUE_SIZE = sizeof(float); // Size of each logged value

void setup()
{
    Serial.begin(9600);

    Serial.println("Reading logged values from EEPROM:");
    for (int address = 0; address < EEPROM_SIZE; address += VALUE_SIZE)
    {
        float loggedValue;
        EEPROM.get(address, loggedValue);

        // Only print if the value is reasonable; you can set specific range checks as needed
        if (!isnan(loggedValue) && loggedValue != 0)
        {
            Serial.print("Address ");
            Serial.print(address);
            Serial.print(": ");
            Serial.println(loggedValue);
        }
    }
}

void loop()
{
    // No action required in loop
}
