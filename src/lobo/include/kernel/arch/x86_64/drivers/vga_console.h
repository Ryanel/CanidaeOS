#pragma once
#include "kernel/log.h"
#include <stdint.h>

class VGAConsoleDevice : public kernel::IKernelLogTerminalOutDevice {
   public:
    void Init(int curX, int curY);
    void PrintChar(const char c);
    void Clear();
    void FormatSetLeftColumn(int column);
   private:
    void AttemptScroll();

   private:
    uint64_t bufferAddr;
    const int width = 80;
    const int height = 25;
    int x = 0;
    int y = 0;
    int leftColumn = 0;
    unsigned char attribute = 0x0F;
};