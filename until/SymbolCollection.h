#pragma once
#include <set>

const std::set<char> Terminator{
	'a','b','c','d','e','f','g','h','i','g','k','l','%'
};

const std::set<char> UnTerminator{
	'A','B','C','D','E','F','G','H','I','G','K','L','S'
};

//�жϸ÷����Ƿ�Ϊ�ս��
bool JudgeTerminator(char symbolic);

//�жϸ÷����Ƿ�Ϊ���ս��
bool JudgeUnTerminator(char symbolic);