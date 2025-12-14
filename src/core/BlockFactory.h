#pragma once

/**
 * МОДУЛЬ: BlockFactory.h
 * ВІДПОВІДАЛЬНІСТЬ: Створення нових блоків
 * 
 * ПРИНЦИП SOLID:
 * - Single Responsibility: Тільки генерування нових блоків
 * - Open/Closed: Легко додати нові типи фігур
 * - Dependency Inversion: Залежить від IShapeProvider, а не від конкретної реалізації
 */

#include "Block.h"
#include "ShapeData.h"

class BlockFactory {
private:
    IShapeProvider &shapeProvider;
    const int screenWidth;

public:
    BlockFactory(IShapeProvider &provider, int screenW)
        : shapeProvider(provider), screenWidth(screenW) {}

    /**
     * Створити новий випадковий блок у верхній частині поля
     */
    Block createRandomBlock() {
        int figureHalfSize = 2;
        int centerX = screenWidth / 2 - figureHalfSize;

        // Випадково обрати один з 7 типів
        const char types[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
        char selectedType = types[random(7)];

        ShapePtr shape = shapeProvider.getShape(selectedType, 0);

        return Block(shape, centerX, 0, 0, selectedType);
    }
};
