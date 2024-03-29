//
// Created by kxg220013 on 3/27/2024.
//

#ifndef CHIP_8_COMMON_H
#define CHIP_8_COMMON_H
#ifdef DEBUG
#define DEBUG_STDERR(x) (std::cerr << (x))
#define DEBUG_STDOUT(x) (std::cout << (x))
#else
#define DEBUG_STDERR(x)
#define DEBUG_STDOUT(x)
#endif
#endif //CHIP_8_COMMON_H
