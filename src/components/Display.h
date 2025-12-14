#include "Arduino.h"
#include <LedControl.h>

//використати https://dotmatrixtool.com/#
static const uint8_t G[8] = {0x3C, 0x42, 0x40, 0x4E, 0x42, 0x42, 0x3C, 0x00};
static const uint8_t A[8] = {0x18, 0x24, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x00};
static const uint8_t M[8] = {0x42, 0x66, 0x5A, 0x5A, 0x42, 0x42, 0x42, 0x00};
static const uint8_t E[8] = {0x7E, 0x40, 0x5C, 0x40, 0x40, 0x40, 0x7E, 0x00};
static const uint8_t O[8] = {0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00};
static const uint8_t V[8] = {0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00};
static const uint8_t R[8] = {0x7C, 0x42, 0x42, 0x7C, 0x48, 0x44, 0x42, 0x00};

class TetrisDisplay
{
private:
    LedControl& lc;
    const int numModules;
    const int screeWidth;
    const uint8_t *charToLedLetter(char c);
public:
    TetrisDisplay(LedControl &lc, int numModules, int screeWidth);
    ~TetrisDisplay();

    void setup();
    void clearDisplay();
    void gameOverAnimation();
    void PrintText(const char *w1);
};

TetrisDisplay::TetrisDisplay(LedControl &lc, int numModules,  int screeWidth)
    : lc(lc), numModules(numModules), screeWidth(screeWidth)
{
}

TetrisDisplay::~TetrisDisplay()
{
}

void TetrisDisplay::setup()
{
    for (int i = 0; i < numModules; i++)
    {
        lc.shutdown(i, false);
        lc.setIntensity(i, 1);
        lc.clearDisplay(i);
    }
}

void TetrisDisplay::clearDisplay()
{
    for (int i = 0; i < numModules; i++)
    {
        lc.clearDisplay(i);
    }
}

const uint8_t* TetrisDisplay::charToLedLetter(char c)
{
  switch (c)
  {
  case 'G':
    return G;
  case 'A':
    return A;
  case 'M':
    return M;
  case 'E':
    return E;
  case 'O':
    return O;
  case 'V':
    return V;
  case 'R':
    return R;
  default:
    return E;
  }
}

void TetrisDisplay::PrintText(const char *w1)
{
  for (int m = 0; m < numModules; m++)
  {
    const uint8_t *letter = charToLedLetter(w1[m]);
    uint8_t rotatedLetter[screeWidth] = {0};
    // Rotate 90° CW: (x,y) → (7-y, x)
    for (int y = 0; y < screeWidth; y++)
    {
      for (int x = 0; x < screeWidth; x++)
      {
        // якщо піксель увімкнено (перевірка біта по масці)
        if (bitRead(letter[y], x))
        {
          // повертаємо x,y місцями
          // https://en.wikipedia.org/wiki/Rotation_matrix
          // згадуємо чому рівний cos, sin для 90°
          int newX = 7 - y;
          int newY = x;
          // встановлюємо відповідний біт у новому масиві
          bitSet(rotatedLetter[newY], newX);
        }
      }
    }
    int module = numModules - 1 - m;
    for (int row = 0; row < screeWidth; row++)
    {
      lc.setRow(module, row, rotatedLetter[row]);
    }
  }
}

void TetrisDisplay::gameOverAnimation()
{
  // мигаємо екраном 3 рази
  for (int i = 0; i < 3; i++)
  {
    clearDisplay();
    delay(500);
    for (int m = 0; m < numModules; m++)
      {
        for (int r = 0; r < screeWidth; r++)
        {
          // 0xFF тут 255, всі біти в байті заповнені одиницями
          lc.setRow(m, r, 0xFF);
        }
      }
    delay(500);
  }
    
  const char *w1 = "GAME";
  PrintText(w1);
  delay(1000); // Wait 1 second before showing OVER

  // 3) Display "OVER" rotated 90° CW
  const char *w2 = "OVER";
  PrintText(w2);
  delay(1000); // Hold OVER for 1 second
}