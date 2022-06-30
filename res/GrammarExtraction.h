#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include "../until/OpenFile.h"

std::unordered_map<char, std::vector<char>>StructMapper(const char* Grammar);
std::vector<std::vector<char>> GetAllDerivation(const std::vector<char>& Data);
