#pragma once
#include <set>

const std::set<char> Terminator{
	'a','b','c','d','e','%','g','f'
};

const std::set<char> UnTerminator{
	'A','B','C','D','E','F'
};

//�жϸ÷����Ƿ�Ϊ�ս��
bool JudgeTerminator(char symbolic);

//�жϸ÷����Ƿ�Ϊ���ս��
bool JudgeUnTerminator(char symbolic);