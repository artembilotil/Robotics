#pragma once

/**
 * МОДУЛЬ: InputHandler.h
 * ВІДПОВІДАЛЬНІСТЬ: Обробка вхідних сигналів від джойстика і кнопок
 * 
 * ПРИНЦИП SOLID:
 * - Single Responsibility: Тільки читання вхідних даних
 * - Open/Closed: Можна легко додати нові типи вводу
 */

#include <Arduino.h>

class JoystickInput {
private:
    const int vrxPin;  // Аналоговий вхід X
    const int vryPin;  // Аналоговий вхід Y
    const int threshold;

public:
    JoystickInput(int rx, int ry) 
        : vrxPin(rx), vryPin(ry), threshold(100) {}

    /**
     * Отримати горизонтальне зміщення (-1, 0, +1)
     */
    int getHorizontalInput() {
        int ax = analogRead(vrxPin);
        if (ax < 400) return 1;      // Вправо
        if (ax > 600) return -1;     // Вліво
        return 0;                     // Без руху
    }

    /**
     * Отримати швидкість падіння блока на основі вертикального положення джойстика
     * Повертає значення від 0 до 690 (для віднімання від базової швидкості)
     */
    int getDropSpeedBoost() {
        int ay = analogRead(vryPin);
        int speed = map(ay, 512, 1023, 0, 690);
        return constrain(speed, 0, 690);
    }
};

class ButtonInput {
private:
    const int pinNumber;
    unsigned long lastDebounceTime;
    const unsigned long debounceDelay = 20;

public:
    ButtonInput(int pin) 
        : pinNumber(pin), lastDebounceTime(0) {
        pinMode(pin, INPUT_PULLUP);
    }

    /**
     * Перевірити чи була натиснута кнопка один раз
     * (з захистом від дребезгу контактів)
     * INPUT_PULLUP: LOW означає натиснута
     */
    bool wasPressed() {
        if (digitalRead(pinNumber) == LOW) {
            delay(debounceDelay);
            if (digitalRead(pinNumber) == LOW) {
                // Чекаємо доки кнопка буде відпущена
                while (digitalRead(pinNumber) == LOW) {
                    delay(1);
                }
                return true;
            }
        }
        return false;
    }

    /**
     * Чекати поки користувач натисне і відпустить кнопку
     */
    void waitForPress() {
        while (digitalRead(pinNumber) == HIGH) delay(10);  // Чекаємо натиску (LOW)
        while (digitalRead(pinNumber) == LOW) delay(10);   // Чекаємо відпуску (HIGH)
    }
};
