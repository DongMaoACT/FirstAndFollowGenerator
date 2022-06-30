#pragma once
#include <set>

const std::set<char> Terminator{
	'a','b','c','d','e','f','g','h','i','g','k','l','%'
};

const std::set<char> UnTerminator{
	'A','B','C','D','E','F','G','H','I','G','K','L','S'
};

//判断该符号是否为终结符
bool JudgeTerminator(char symbolic);

//判断该符号是否为非终结符
bool JudgeUnTerminator(char symbolic);