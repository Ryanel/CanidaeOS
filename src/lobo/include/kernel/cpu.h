#pragma once
namespace kernel {
namespace cpu {

/// Put the CPU into an idle loop
void IdleLoop();

/// Halt the CPU
void HaltCPU();

}
}