#include "zgate.h"
#include <vector>

std::vector<ZGate> gates;

void GATES_Config(void)
{
    gates.push_back(ZGate(PF1, PA0, PF0, GATE_TYPE_AND));
    gates.push_back(ZGate(PC14, PC15, PC13, GATE_TYPE_AND));

    gates.push_back(ZGate(PA1, PA2, PA3, GATE_TYPE_NAND));
    gates.push_back(ZGate(PA4, PA5, PA6, GATE_TYPE_NAND));

    gates.push_back(ZGate(PB8, PB9, PB7, GATE_TYPE_OR));
    gates.push_back(ZGate(PA15, PB3, PF7, GATE_TYPE_OR));

    gates.push_back(ZGate(PB11, PB10, PB2, GATE_TYPE_NOR));
    gates.push_back(ZGate(PB1, PB0, PA7, GATE_TYPE_NOR));

    gates.push_back(ZGate(PB5, PB5, PB4, GATE_TYPE_NOT));
    gates.push_back(ZGate(PA12, PA12, PA11, GATE_TYPE_NOT));

    gates.push_back(ZGate(PB14, PB13, PB12, GATE_TYPE_XOR));
    gates.push_back(ZGate(PA8, PF6, PB15, GATE_TYPE_XOR));
}

void GATES_Task(void)
{
    for(auto& gate : gates) {
        gate.update();
    }
    // 先更新所有门的状态, 再更新输出引脚
    for(auto& gate : gates) {
        gate.output();
    }
}
