#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include "GrammarExtraction.h"
#include "../until/SymbolCollection.h"

static std::vector<char> FindAllFirstSymbolic(std::vector<char>& Data);

void CreateFirst(char Symbolic, std::unordered_map<char, std::vector<char>> maps, std::vector<char>& result);

std::vector<std::vector<char>> GetAllDerivation(const std::vector<char>& Data);

void CreateChildFirst(char Symbolic, std::unordered_map<char, std::vector<char>> maps, std::vector<std::vector<char>> ChildData, std::vector<char>& result);

void TraversalAllUnTerminator(char* data);

std::unordered_map<char, std::vector<char>> CreateAllFirst(const char* InData);