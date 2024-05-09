//
// Created by kxg220013 on 4/30/2024.
//

#ifndef CHIP_8_DEBUGGER_H
#define CHIP_8_DEBUGGER_H

#include <string>

class Debugger {
private:
    bool m_debug;
public:
    Debugger();
    void setDebug(bool debug);
    void debug (const std::string &message) const;
};


#endif //CHIP_8_DEBUGGER_H
