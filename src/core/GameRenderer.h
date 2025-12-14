#pragma once

/**
 * МОДУЛЬ: GameRenderer.h
 * ВІДПОВІДАЛЬНІСТЬ: Відображення гри на дисплеї (MAX7219)
 * 
 * ПРИНЦИП SOLID:
 * - Single Responsibility: Тільки відображення на екран
 * - Open/Closed: Можна замінити на інший дисплей без змін ігрової логіки
 */

#include <Arduino.h>
#include <LedControl.h>
#include "GameField.h"
#include "Block.h"

// Шрифти для текстового відображення
static const uint8_t FONT_G[8] = {0x3C, 0x42, 0x40, 0x4E, 0x42, 0x42, 0x3C, 0x00};
static const uint8_t FONT_A[8] = {0x18, 0x24, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x00};
static const uint8_t FONT_M[8] = {0x42, 0x66, 0x5A, 0x5A, 0x42, 0x42, 0x42, 0x00};
static const uint8_t FONT_E[8] = {0x7E, 0x40, 0x5C, 0x40, 0x40, 0x40, 0x7E, 0x00};
static const uint8_t FONT_O[8] = {0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00};
static const uint8_t FONT_V[8] = {0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00};
static const uint8_t FONT_R[8] = {0x7C, 0x42, 0x42, 0x7C, 0x48, 0x44, 0x42, 0x00};
static const uint8_t FONT_P[8] = {0x7C, 0x42, 0x42, 0x7C, 0x40, 0x40, 0x40, 0x00};
static const uint8_t FONT_L[8] = {0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7E, 0x00};
static const uint8_t FONT_Y[8] = {0x42, 0x42, 0x42, 0x24, 0x18, 0x08, 0x08, 0x00};

class GameRenderer {
private:
    LedControl &ledControl;
    const int numModules;
    const int screenWidth;
    const int screenHeight;
    const int modulesPerRow;

    // Допоміжна функція: отримати позицію модуля для координати Y
    int getModuleForY(int fieldY) const {
        return numModules - 1 - (fieldY / screenWidth);
    }

    // Допоміжна функція: отримати рядок у модулі для координати Y
    int getRowInModule(int fieldY) const {
        return 7 - (fieldY % screenWidth);
    }

    // Отримати шрифт для символу
    const uint8_t* charToFont(char c) const {
        switch (c) {
            case 'G': return FONT_G;
            case 'A': return FONT_A;
            case 'M': return FONT_M;
            case 'E': return FONT_E;
            case 'O': return FONT_O;
            case 'V': return FONT_V;
            case 'R': return FONT_R;
            case 'P': return FONT_P;
            case 'L': return FONT_L;
            case 'Y': return FONT_Y;
            default: return FONT_E;
        }
    }

    // Вивести текст на екран з ротацією 90° за годинниковою стрілкою
    void displayText(const char *text) {
        for (int m = 0; m < numModules && m < (int)strlen(text); m++) {
            const uint8_t *font = charToFont(text[m]);
            uint8_t rotatedChar[screenWidth] = {0};
            
            // Rotate 90° CW: (x,y) → (7-y, x)
            for (int y = 0; y < screenWidth; y++) {
                for (int x = 0; x < screenWidth; x++) {
                    if (bitRead(font[y], x)) {
                        int newX = 7 - y;
                        int newY = x;
                        bitSet(rotatedChar[newY], newX);
                    }
                }
            }
            
            int module = numModules - 1 - m;
            for (int row = 0; row < screenWidth; row++) {
                ledControl.setRow(module, row, rotatedChar[row]);
            }
        }
    }

    /**
     * Анімація падаючої ялинки на весь екран
     */
    void playFallingTreeAnimation() {
        // Падаюча ялинка - почти як Тетрис блок, що падає
        // Ялинка формується поступово та потім падає вниз
        for (int drop = 0; drop < 3; drop++) {
            // Будуємо ялинку лінія за лінією (зверху вниз)
            for (int stage = 0; stage < 8; stage++) {
                clear();
                
                // Вершина ялинки (трикутник)
                uint8_t treePattern[8] = {0};
                
                // Рисуємо ялинку залежно від стадії падіння
                // Верхня частина - зірочка
                if (stage >= 0) treePattern[0] = 0x18;  // ..**...
                if (stage >= 1) treePattern[1] = 0x3C;  // ..***..
                if (stage >= 2) treePattern[2] = 0x7E;  // .******.
                if (stage >= 3) treePattern[3] = 0x3C;  // ..***..
                if (stage >= 4) treePattern[4] = 0x18;  // ..**...
                if (stage >= 5) treePattern[5] = 0x18;  // Стовбур
                if (stage >= 6) treePattern[6] = 0x18;  // Стовбур
                if (stage >= 7) treePattern[7] = 0x00;  // Порожньо
                
                // Виводимо ялинку на всі модулі з ефектом "падіння"
                for (int m = 0; m < numModules; m++) {
                    // Зміщуємо ялинку вниз залежно від номера модуля
                    int offset = (drop * 2 + m) % 8;
                    for (int row = 0; row < screenWidth; row++) {
                        int treeRow = (row - offset + 16) % 8;
                        if (treeRow >= 0 && treeRow < 8) {
                            ledControl.setRow(m, row, treePattern[treeRow]);
                        } else {
                            ledControl.setRow(m, row, 0x00);
                        }
                    }
                }
                
                delay(150);
            }
        }
        
        clear();
    }

public:
    GameRenderer(LedControl &lc, int modules, int width, int height)
        : ledControl(lc), numModules(modules), screenWidth(width), 
          screenHeight(height), modulesPerRow(width) {
        initialize();
    }

    /**
     * Ініціалізувати дисплей
     */
    void initialize() {
        for (int i = 0; i < numModules; i++) {
            ledControl.shutdown(i, false);
            ledControl.setIntensity(i, 1);
            ledControl.clearDisplay(i);
        }
    }

    /**
     * Очистити весь дисплей
     */
    void clear() {
        for (int i = 0; i < numModules; i++) {
            ledControl.clearDisplay(i);
        }
    }

    /**
     * Основний метод відображення стану гри
     * Малює: поле + падаючий блок
     */
    void render(const GameField &field, const Block &currentBlock) {
        // Локальний буфер для кожного модуля
        uint8_t buf[numModules][screenWidth];
        memset(buf, 0, sizeof(buf));

        // Малюємо зафіксовані блоки з поля
        for (int y = 0; y < screenHeight; y++) {
            uint8_t row = field.getRow(y);
            if (!row) continue;  // Пропустити пусті рядки

            int moduleIdx = getModuleForY(y);
            int bitPos = getRowInModule(y);

            for (int x = 0; x < screenWidth; x++) {
                if (bitRead(row, x)) {
                    bitSet(buf[moduleIdx][x], bitPos);
                }
            }
        }

        // Малюємо поточний падаючий блок
        for (int i = 0; i < 4; i++) {
            int x = currentBlock.x + currentBlock.shape[i][0];
            int y = currentBlock.y + currentBlock.shape[i][1];

            if (x < 0 || x >= screenWidth || y < 0 || y >= screenHeight) continue;

            int moduleIdx = getModuleForY(y);
            int bitPos = getRowInModule(y);
            bitSet(buf[moduleIdx][x], bitPos);
        }

        // Записати на дисплей
        for (int module = 0; module < numModules; module++) {
            for (int row = 0; row < screenWidth; row++) {
                ledControl.setRow(module, row, buf[module][row]);
            }
        }
    }

    /**
     * Анімація "Game Over" - падаюча ялинка + текст (без миготління)
     */
    void playGameOverAnimation() {
        // 1. Падаюча ялинка анімація
        playFallingTreeAnimation();
        
        delay(500);
        
        // 2. Вивести "GAME"
        displayText("GAME");
        delay(1500);
        
        // 3. Вивести "OVER"
        displayText("OVER");
        delay(1500);
        
        clear();
    }
    
    /**
     * Допоміжна функція: включити всі LED
     */
    void fillAllLeds() {
        for (int m = 0; m < numModules; m++) {
            for (int r = 0; r < screenWidth; r++) {
                ledControl.setRow(m, r, 0xFF);
            }
        }
    }

    /**
     * Вивести напис "PLAY" з миготінням під час очікування натиску кнопки
     */
    void displayPlayText() {
        // Миготимо "PLAY" кілька разів поки чекаємо натиску
        displayText("PLAY");
    }
};
