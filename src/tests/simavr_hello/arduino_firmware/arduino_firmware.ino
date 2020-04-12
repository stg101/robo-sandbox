const int PINS_SIZE = 8;
int pins[PINS_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7};

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
        delay(500);
        digitalWrite(pins[i], LOW);
        delay(500);
    }
}