#include "FirstGenerator.h"


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



//递归方式寻找该非终极符的First集合，结果保存在result中
void CreateFirst(char Symbolic, std::unordered_map<char, std::vector<char>> maps, std::vector<char>& result) {
	//出口：左侧第一个符号为终结符
	if (JudgeTerminator(Symbolic)) {
		result.push_back(Symbolic);
		return ;
	}
	//找到此非终结符对应的所有左侧首字符集合
	std::vector<char> temp = FindAllFirstSymbolic(maps[Symbolic]);
	//递归调用将所有左侧第一个终结符加入result集合
	for (auto x : temp) {
		if(1 == maps.count(x)||JudgeTerminator(x))
			CreateFirst(x, maps, result);
		else {
			std::cout << "Error: 文法非法 存在非终结符无法推出终结符的情况'\n'";
			__debugbreak();
		}
	}
	return;
}

// 生成Firsit集合
void TraversalAllUnTerminator(char *data) {
	std::unordered_map<char, std::vector<char>> maps = StructMapper(data);
	std::vector<char> result;
	for (std::unordered_map<char, std::vector<char>>::const_iterator iter = maps.begin(); iter != maps.end(); ++iter) {
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
/*int main() {
	char test[40] = {'s','t','a','r','t','\n','E','-','>','a','c','|','%','|','A','\n','A','-','>','b','|','c','b','|','D','\n','D','-','>','%','\n','C','-','>','a','\n','e','n','d','\n'};
	
	Test Data
	*StructMapper(test);
	std::vector<char> data = { 's','a','|','b','a','|','d' };
	std::cout << FindAllFirstSymbolic(data).at(1) << std::endl;
	std::vector<char> result;
	CreateFirst('E', StructMapper(test), result);
	for (auto x : result) std::cout << x << std::endl;
	
	TraversalAllUnTerminator(test);
}
*/