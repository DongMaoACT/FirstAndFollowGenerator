#include "../until/SymbolCollection.h"
#include "GrammarExtraction.h"

//将所有右推导分块A->Ba|D 分为Ba,D放到两(n)个vector中
std::vector<std::vector<char>> GetAllDerivation(const std::vector<char>& Data) {
	std::vector<std::vector<char>> result;
	result.push_back({});
	for (auto x : Data) {
		if (x == '|') {
			result.push_back({});
			continue;
		}
		result.back().push_back(x);
	}
	return result;
}

static char FindUnUseUnTerminator(std::unordered_map<char, std::vector<char>> maps) {
	for (auto x : UnTerminator) {
		if (maps.count(x) == 1) {
			continue;
		}
		else {
			return x;
		}
	}
	return '@';
}

std::unordered_map<char, std::vector<char>> DeleteLeftRecursion(std::unordered_map<char, std::vector<char>> maps) {
	for (std::unordered_map<char, std::vector<char>>::iterator iter = maps.begin(); iter != maps.end(); iter++) {
		std::vector<std::vector<char>> DataChild = GetAllDerivation(iter->second);
		bool flag = false;
		char Ctemp = FindUnUseUnTerminator(maps);
		std::vector<char> temp;
		std::vector<std::vector<char>>::iterator vter = DataChild.begin();
		while (vter != DataChild.end()) {
			if ((*vter).at(0) == iter->first) {
				(*vter).push_back(Ctemp);
				(*vter).erase((*vter).begin());
				temp.insert(temp.end(), (*vter).begin(), (*vter).end());
				temp.push_back('|');
				vter = DataChild.erase(vter);
				flag = true;
			}
			else {
				++vter;
			}
		}
		if (flag) {
			temp.pop_back();
			maps.insert(std::unordered_map<char, std::vector<char>>::value_type(Ctemp, temp));
			temp.clear();
			for (auto x : DataChild) {
				x.push_back(Ctemp);
				temp.insert(temp.end(), x.begin(), x.end());
				temp.push_back('|');
			}
			temp.pop_back();
			maps[iter->first] = temp;
		}
	}
	for (std::unordered_map<char, std::vector<char>>::iterator iter = maps.begin(); iter != maps.end(); iter++) {
		bool flag = false;
		std::vector<char> temp;
		std::vector<std::vector<char>> tempresult;
		char Ctemp = FindUnUseUnTerminator(maps);
		std::vector<std::vector<char>> DataChild = GetAllDerivation(iter->second);
		std::vector<std::vector<char>>::iterator vter = DataChild.begin();
		while (vter != DataChild.end()) {
			if (JudgeUnTerminator((*vter).at(0)) && (*vter).at(0) != iter->first) {
				char tempchar = (*vter).at(0);
				(*vter).erase((*vter).begin());
				std::vector<std::vector<char>> tempChild = GetAllDerivation(maps[tempchar]);
				temp = *vter;
				for (auto x : tempChild) {
					if (x.at(0) != '%') {
						x.insert(x.end(), temp.begin(), temp.end());
						tempresult.push_back(x);
					}
				}
				vter = DataChild.erase(vter);
				DataChild.insert(DataChild.end(), tempresult.begin(), tempresult.end());
				tempresult.clear();
				vter = DataChild.begin();
				flag = true;
			}
			else {
				vter++;
			}
		}
		if (flag) {
			temp.clear();
			for (auto x : DataChild) {
				temp.insert(temp.end(), x.begin(), x.end());
				temp.push_back('|');
			}
			temp.pop_back();
			vter = DataChild.begin();
			temp.clear();
			while (vter != DataChild.end()) {
				if ((*vter).at(0) == iter->first) {
					(*vter).push_back(Ctemp);
					(*vter).erase((*vter).begin());
					temp.insert(temp.end(), (*vter).begin(), (*vter).end());
					temp.push_back('|');
					vter = DataChild.erase(vter);
					flag = false;
				}
				else {
					++vter;
				}
			}
			if (!flag) {
				temp.pop_back();
				maps.insert(std::unordered_map<char, std::vector<char>>::value_type(Ctemp, temp));
				temp.clear();
				for (auto x : DataChild) {
					x.push_back(Ctemp);
					temp.insert(temp.end(), x.begin(), x.end());
					temp.push_back('|');
				}
				temp.pop_back();
				maps[iter->first] = temp;
			}

		}
	}

	return maps;
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

	return DeleteLeftRecursion(maps);
}





//S:Sa|b -> S:bX X->aX|%

//int main() {
//
//	char test1[100] = { 's','t','a','r','t','\n','S','-','>','S','a','|','S','b','|','c','|','A','\n','A','-','>','B','d','\n','B','-','>','S','d','|','%','\n','e','n','d','\n' };
//	char test[100] = { 's','t','a','r','t','\n','S','-','>','A','a','|','S','b','|','c','\n','A','-','>','B','C','c','|','g','D','B','\n','B','-','>','S','b','C','D','E','|','%','\n','C','-','>','D','a','B','|','c','a','\n','D','-','>','d','D','|','%','\n','E','-','>','g','A','f','|','c','\n','e','n','d','\n'};
//	std::unordered_map<char, std::vector<char>> maps = StructMapper(test1);
//	std::unordered_map<char, std::vector<char>> temp = DeleteLeftRecursion(maps);
//	int i;
//}