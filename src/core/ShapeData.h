#pragma once

/**
 * МОДУЛЬ: ShapeData.h
 * ВІДПОВІДАЛЬНІСТЬ: Зберігання та управління даними фігур Tetris
 * 
 * ПРИНЦИП SOLID:
 * - Single Responsibility: Містить тільки визначення фігур
 * - Open/Closed: Легко додати нові фігури без змін існуючого коду
 */

// Константні масиви для кожної фігури Tetris
// Формат: [версія повороту][4 пікселі][x, y]

const int I_SHAPE[2][4][2] = {
    {{0, 0}, {0, 1}, {0, 2}, {0, 3}},  // Вертикальна
    {{-1, 1}, {0, 1}, {1, 1}, {2, 1}}  // Горизонтальна
};

const int O_SHAPE[1][4][2] = {
    {{0, 0}, {1, 0}, {0, 1}, {1, 1}}   // Квадрат (без повороту)
};

const int T_SHAPE[4][4][2] = {
    {{1, 0}, {0, 1}, {1, 1}, {2, 1}},
    {{1, 0}, {1, 1}, {1, 2}, {0, 1}},
    {{0, 1}, {1, 1}, {2, 1}, {1, 2}},
    {{1, 0}, {1, 1}, {1, 2}, {2, 1}}
};

const int L_SHAPE[4][4][2] = {
    {{0, 0}, {0, 1}, {0, 2}, {1, 2}},
    {{0, 0}, {1, 0}, {2, 0}, {0, 1}},
    {{0, 0}, {1, 0}, {1, 1}, {1, 2}},
    {{2, 0}, {0, 1}, {1, 1}, {2, 1}}
};

const int J_SHAPE[4][4][2] = {
    {{1, 0}, {1, 1}, {1, 2}, {0, 2}},
    {{0, 0}, {0, 1}, {1, 1}, {2, 1}},
    {{0, 0}, {1, 0}, {0, 1}, {0, 2}},
    {{0, 0}, {1, 0}, {2, 0}, {2, 1}}
};

const int S_SHAPE[2][4][2] = {
    {{1, 0}, {2, 0}, {0, 1}, {1, 1}},
    {{1, 0}, {1, 1}, {2, 1}, {2, 2}}
};

const int Z_SHAPE[2][4][2] = {
    {{0, 0}, {1, 0}, {1, 1}, {2, 1}},
    {{2, 0}, {1, 1}, {2, 1}, {1, 2}}
};

/**
 * Інтерфейс для управління даними фігур
 * (Interface Segregation: мінімальні методи для отримання даних)
 */

#include "Block.h"  // Містить typedef ShapePtr

class IShapeProvider {
public:
    virtual ~IShapeProvider() = default;
    virtual ShapePtr getShape(char type, int rotation) = 0;
    virtual int getRotationCount(char type) = 0;
};

/**
 * Конкретна реалізація провайдера фігур
 * (Dependency Inversion: клієнти залежать від інтерфейсу, а не від класу)
 */
class ShapeProvider : public IShapeProvider {
public:
    ShapePtr getShape(char type, int rotation) override {
        switch (type) {
            case 'I': return I_SHAPE[rotation];
            case 'O': return O_SHAPE[0];
            case 'T': return T_SHAPE[rotation];
            case 'L': return L_SHAPE[rotation];
            case 'J': return J_SHAPE[rotation];
            case 'S': return S_SHAPE[rotation];
            case 'Z': return Z_SHAPE[rotation];
            default: return nullptr;
        }
    }

    int getRotationCount(char type) override {
        switch (type) {
            case 'I':
            case 'S':
            case 'Z':
                return 2;
            case 'O':
                return 1;
            default:
                return 4;
        }
    }
};
