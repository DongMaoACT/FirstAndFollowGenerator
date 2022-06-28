#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include "GrammarExtraction.h"
#include "../until/SymbolCollection.h"
#include "FirstGenerator.h"

std::unordered_map<char, std::vector<char>> CreateAllFollow(const char* InData);