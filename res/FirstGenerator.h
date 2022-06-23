#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include "GramarExtraction.h"
#include "../until/SymbolCollection.h"

static std::vector<char> FindAllFirstSymbolic(std::vector<char>& Data);

void CreateFirst(char Symbolic, std::unordered_map<char, std::vector<char>> maps, std::vector<char>& result);

void TraversalAllUnTerminator(char* data);