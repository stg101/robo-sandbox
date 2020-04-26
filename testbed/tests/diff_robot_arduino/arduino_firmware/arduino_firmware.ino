const int PINS_SIZE = 2;
int pins[PINS_SIZE] = {0, 1};

void setup()
{
    for (int i = 0; i < PINS_SIZE; i++)
    {
        pinMode(pins[i], OUTPUT);
    }
}

void loop()
{
    for (int i = 0; i < PINS_SIZE; i++)
    {
        digitalWrite(pins[i], HIGH);
        delay(2000);
        digitalWrite(pins[i], LOW);
        delay(2000);
    }
    delay(2000);
}