//
// Created by kxg220013 on 4/30/2024.
//

#include <iostream>
#include "debugger.h"

Debugger::Debugger() {
    m_debug = false;
}

void Debugger::setDebug(bool debug) {
    m_debug = debug;
}

void Debugger::debug (const std::string &message) const {
    if (m_debug) {
        std::cout << message;
    }
}