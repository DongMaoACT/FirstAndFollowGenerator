#include <iostream>
#include <vector>
#include <map>
#include "../config/SymbolCollection.h"

//判断该符号是否为非终结符
static bool JudgeTerminator(char symbolic) {
	if (Terminator.find(symbolic) != Terminator.end())
		return true;
	else
		return false;
}

//判断该符号是否为终结符
static bool JudgeUnTerminator(char symbolic) {
	if (UnTerminator.find(symbolic) != UnTerminator.end())
		return true;
	else
		return false;
}

//检查单句文法中是否存在 “|”,并输出文法中所以首字符
static std::vector<char> FindAllFirstSymbolic(std::vector<char> &Data) {
	std::vector<char> symbolic;
	std::vector<char>::iterator Grammar = Data.begin();
	//将文法中第一个字符加入容器中
	if (Grammar != Data.end()) {
		symbolic.push_back(*Grammar);
		Grammar += 1;
	}
	else {
		std::cout << "Error: 文法错误“->”后无任何符号";
		__debugbreak();
	}
	//将文法中每个“|”后的首字符加入容器中
	while (Grammar != Data.end()) {
		if (*Grammar == '|') {
			if ((Grammar + 1) != Data.end()) {
				symbolic.push_back(*(Grammar + 1));
				Grammar += 2;
			}
			else {
				std::cout << "Error: 文法错误“|”后未置放符号";
				__debugbreak();
			}
		}else {
			Grammar += 1;
		}
	}
	return symbolic;
}

/*
	文法模型
	start\n
	E->Tb\n
	T->a\n
	end\n
	start\nE->Tb\nT->a\nend\n
*/
//构造推到映射关系 E—>TE' =>  ‘E': {'T''E''}<'char','char[]'>
static std::map<char, std::vector<char>>StructMapper(const char* Grammar) {
	std::map<char, std::vector<char>> maps;
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
	while (index < size && *(Grammar+1) != 'e') {
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
				maps.insert(std::map<char, std::vector<char>>::value_type(UnTerminator, temp));
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
	char end[3] = { 'e','n' ,'d'};
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

//递归方式寻找该非终极符的First集合，结果保存在result中
static void CreateFirst(char Symbolic, std::map<char, std::vector<char>> maps, std::vector<char>& result) {
	//出口：左侧第一个符号为终结符
	if (JudgeTerminator(Symbolic)) {
		result.push_back(Symbolic);
		return ;
	}
	//找到此非终结符对应的所有左侧首字符集合
	std::vector<char> temp = FindAllFirstSymbolic(maps[Symbolic]);
	//递归调用将所有左侧第一个终结符加入result集合
	for (auto x : temp) {
		if(1 == maps.count(x))
			CreateFirst(x, maps, result);
		else {
			std::cout << "Error: 文法非法 存在非终结符无法推出终结符的情况'\n'";
			__debugbreak();
		}
	}
	return;
}

void TraversalAllUnTerminator(char *data) {
	std::map<char, std::vector<char>> maps = StructMapper(data);
	std::vector<char> result;
	for (std::map<char, std::vector<char>>::const_iterator iter = maps.begin(); iter != maps.end(); ++iter) {
		CreateFirst(iter->first, maps, result);
		//Vector 去重
		std::set<int>s(result.begin(), result.end());
		result.assign(s.begin(), s.end());
		std::cout << iter->first << ":";
		for (auto x : result) std::cout << x ;
		result.clear();
		std::cout << std::endl;
	}

}


//由于使用的map结构为有序结构，故结果按A，B，C，D排序
int main() {
	char test[40] = {'s','t','a','r','t','\n','E','-','>','a','c','|','%','|','A','\n','A','-','>','b','|','c','b','|','D','\n','D','-','>','%','\n','C','-','>','F','\n','e','n','d','\n'};
	/*
	Test Data
	*StructMapper(test);
	std::vector<char> data = { 's','a','|','b','a','|','d' };
	std::cout << FindAllFirstSymbolic(data).at(1) << std::endl;
	std::vector<char> result;
	CreateFirst('E', StructMapper(test), result);
	for (auto x : result) std::cout << x << std::endl;
	*/
	TraversalAllUnTerminator(test);
}