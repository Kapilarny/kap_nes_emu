//
// Created by user on 06.07.2024.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <cstdio>

#define LOG(str, ...) printf(str, ##__VA_ARGS__); printf("\n");
#endif //LOGGER_H
