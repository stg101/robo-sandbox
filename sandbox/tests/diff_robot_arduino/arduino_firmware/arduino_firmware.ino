const int PINS_SIZE = 2;
int pins[PINS_SIZE] = {0, 1};

void setup()
{
    for (int i = 0; i < PINS_SIZE; i++)
    {
        pinMode(pins[i], OUTPUT);
    }

    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
}

void loop()
{

    int read0 = digitalRead(A0);
    int read1 = digitalRead(A1);

    if (read0 && read1)
    {
        digitalWrite(0, LOW);
        digitalWrite(1, LOW);
        delay(1);
    }

    if (read0 && !read1)
    {
        digitalWrite(0, HIGH);
        digitalWrite(1, LOW);
        delay(1);
    }

    if (read1 && !read0)
    {
        digitalWrite(0, LOW);
        digitalWrite(1, HIGH);
        delay(1);
    }

    if (!read0 && !read1)
    {
        digitalWrite(0, HIGH);
        digitalWrite(1, HIGH);
        delay(1);
    }
}