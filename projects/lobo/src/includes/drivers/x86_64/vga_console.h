#pragma once
#include "kernel/log.h"

class VGAConsoleDevice : public IKernelLogTerminalOutDevice {
   public:
    void Init(int curX, int curY);
    void PrintChar(const char c);
    void Clear();

   private:
    void AttemptScroll();

   private:
    const int width = 80;
    const int height = 25;
    int x = 0;
    int y = 0;
    unsigned char attribute = 0x0F;
};