#include "SymbolCollection.h"

//�жϸ÷����Ƿ�Ϊ�ս��
bool JudgeTerminator(char symbolic) {
	if (Terminator.find(symbolic) != Terminator.end())
		return true;
	else
		return false;
}

//�жϸ÷����Ƿ�Ϊ���ս��
bool JudgeUnTerminator(char symbolic) {
	if (UnTerminator.find(symbolic) != UnTerminator.end())
		return true;
	else
		return false;
}