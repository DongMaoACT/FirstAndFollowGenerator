#include "../until/SymbolCollection.h"
#include "GramarExtraction.h"

/*
	文法模型
	start\n
	E->Tb\n
	T->a\n
	end\n
	start\nE->Tb\nT->a\nend\n
*/
//构造推到映射关系 E—>TE' =>  ‘E': {'T''E''}<'char','char[]'>
std::unordered_map<char, std::vector<char>>StructMapper(const char* Grammar) {
	std::unordered_map<char, std::vector<char>> maps;
	//设置文法最大长度
	int index = 0;
	int size = 100;
	//find 'start'
	char start[5] = { 's','t' ,'a','r','t' };
	while (index < size) {
		int i = 0;
		// 因为start中无重复字符s，故每次匹配失败都从当前index重新开始
		while (*Grammar == start[i] && i < 5) {
			Grammar += 1;
			index += 1;
			i += 1;
		}
		if (i != 5) {
			Grammar += 1;
			index += 1;
			continue;
		}
		else break;
	}
	if (*Grammar != '\n') {
		std::cout << "Error: 文法非法 文法后 无'\n'";
		__debugbreak();
	}
	if (index > 95) {
		std::cout << "Error: 文法非法 start 后无文法";
		__debugbreak();
	}
	//mapping grammar
	while (index < size && *(Grammar + 1) != 'e') {
		std::vector<char> temp;
		while (*Grammar == '\n' && *(Grammar + 2) == '-' && *(Grammar + 3) == '>') {
			if (JudgeUnTerminator(*(Grammar + 1))) {
				char UnTerminator = *(Grammar + 1);
				Grammar += 4;
				index += 4;
				while (*Grammar != '\n') {
					temp.push_back(*Grammar);
					Grammar += 1;
					index += 1;
				}
				maps.insert(std::unordered_map<char, std::vector<char>>::value_type(UnTerminator, temp));
			}
			else {
				std::cout << "Error: 文法错误 -> 前放置为非终结符 或 文法格式错误";
				__debugbreak();
			}
			temp.clear();
		}
		break;
	}
	if (*Grammar != '\n') {
		std::cout << "Error: 文法非法 文法后 无'\n'";
		__debugbreak();
	}
	if (index > 96) {
		std::cout << "Error: 文法长度超出限制";
		__debugbreak();
	}
	//find 'end'
	char end[3] = { 'e','n' ,'d' };
	while (index < size) {
		int i = 0;
		// 因为start中无重复字符s，故每次匹配失败都从当前index重新开始
		while (*Grammar == end[i] && i < 3) {
			Grammar += 1;
			index += 1;
			i += 1;
		}
		if (i != 3) {
			Grammar += 1;
			index += 1;
			continue;
		}
		else break;
	}
	if (*Grammar != '\n') {
		std::cout << "Error: 文法非法 文法后 无'\n'";
		__debugbreak();
	}
	return maps;
}
