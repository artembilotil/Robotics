#include <Arduino.h>

class DebButton
{
private:
    int pin;

public:
    DebButton(int pin)
    {
        this->pin = pin;
    }
    ~DebButton();

    void attach()
    {
        pinMode(pin, INPUT_PULLUP);
    }

    bool isClickedOnce()
    {
        if (digitalRead(pin) == HIGH)
        {
            delay(20);
            if (digitalRead(pin) == HIGH)
            {
                while (digitalRead(pin) == HIGH)
                {
                    delay(1);
                }
                return true;
            }
        }
        return false;
    }
    
    void waitForButtonPress()
    {        
        while (digitalRead(pin) != LOW)
            delay(10);
        while (digitalRead(pin) == LOW)
            delay(10);
    }
};

DebButton::~DebButton()
{
}
