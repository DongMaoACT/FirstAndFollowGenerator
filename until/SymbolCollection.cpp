#include "SymbolCollection.h"

//判断该符号是否为终结符
bool JudgeTerminator(char symbolic) {
	if (Terminator.find(symbolic) != Terminator.end())
		return true;
	else
		return false;
}

//判断该符号是否为非终结符
bool JudgeUnTerminator(char symbolic) {
	if (UnTerminator.find(symbolic) != UnTerminator.end())
		return true;
	else
		return false;
}