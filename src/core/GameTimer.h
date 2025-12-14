#pragma once

/**
 * МОДУЛЬ: GameTimer.h
 * ВІДПОВІДАЛЬНІСТЬ: Управління часовими інтервалами в грі
 * 
 * ПРИНЦИП SOLID:
 * - Single Responsibility: Тільки часові інтервали
 * - Open/Closed: Готово до розширення новими типами таймерів
 */

class GameTimer {
private:
    unsigned long lastTime;
    unsigned long interval;  // Тепер не const, щоб дозволити setInterval

public:
    GameTimer(unsigned long timerInterval)
        : lastTime(0), interval(timerInterval) {}

    /**
     * Перевірити чи пройшов достатній час
     * і оновити таймер якщо так
     */
    bool isReady() {
        unsigned long now = millis();
        if (now - lastTime >= interval) {
            lastTime = now;
            return true;
        }
        return false;
    }

    /**
     * Встановити нов значення інтервалу
     */
    void setInterval(unsigned long newInterval) {
        interval = newInterval;
    }

    /**
     * Отримати час останньої активації
     */
    unsigned long getLastTime() const {
        return lastTime;
    }

    /**
     * Переустановити таймер
     */
    void reset() {
        lastTime = millis();
    }
};

/**
 * Контейнер для управління всіма таймерами гри
 */
class GameTimerManager {
public:
    GameTimer moveTimer{200};     // Інтервал між поточними рухами
    GameTimer dropTimer{500};     // Інтервал падіння блока
    GameTimer refreshTimer{33};   // Інтервал оновлення екрану (~30 FPS)

    void reset() {
        moveTimer.reset();
        dropTimer.reset();
        refreshTimer.reset();
    }
};
