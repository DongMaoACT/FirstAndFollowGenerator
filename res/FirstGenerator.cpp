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


void CreateChildFirst(char Symbolic, std::unordered_map<char, std::vector<char>> maps, std::vector<std::vector<char>> ChildData, std::vector<char>& result) {
	bool flag = 0;
	if (JudgeTerminator(Symbolic)) {
		result.push_back(Symbolic);
		return;
	}
	for (auto child : ChildData) {
		for (auto x : child) {
			if (1 == maps.count(x) || JudgeTerminator(x)) {
				CreateChildFirst(x,maps,GetAllDerivation(maps[x]), result);
				
				if (result.back() != '%' ) 
					break;
				else if(x != child.back())
					result.pop_back();
			}
		}
		
	}

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
		//CreateFirst(iter->first, maps, result);
		CreateChildFirst(iter->first, maps, GetAllDerivation(iter->second), result);
		//Vector 去重
		std::set<int>s(result.begin(), result.end());
		result.assign(s.begin(), s.end());
		std::cout << iter->first << ":";
		for (auto x : result) std::cout << x ;
		result.clear();
		std::cout << std::endl;
	}
}

std::unordered_map<char, std::vector<char>> CreateAllFirst(const char* InData) {
	std::unordered_map<char, std::vector<char>> result;
	std::unordered_map<char, std::vector<char>> maps = StructMapper(InData);
	for (std::unordered_map<char, std::vector<char>>::iterator iter = maps.begin(); iter != maps.end(); iter++) {
						std::cout << iter->first << "->";
						for (auto x : iter->second) {
							std::cout << x << " ";
						}
						std::cout << std::endl;
	}
	for (std::unordered_map<char, std::vector<char>>::const_iterator iter = maps.begin(); iter != maps.end(); ++iter) {
		std::vector<char> temp;
		CreateChildFirst(iter->first, maps, GetAllDerivation(iter->second), temp);
		std::set<char>s(temp.begin(), temp.end());
		temp.assign(s.begin(), s.end());
		result.insert(std::unordered_map<char, std::vector<char>>::value_type(iter->first, temp));
	}
	return result;
}

//由于使用的map结构为有序结构，故结果按A，B，C，D排序
//int main() {
//	char test[100] = { 's','t','a','r','t','\n','A','-','>','B','C','c','|','g','D','b','\n','B','-','>','b','C','D','E','|','%','\n','C','-','>','D','a','b','|','c','a','\n','D','-','>','d','D','|','%','\n','E','-','>','g','A','f','|','c','\n','e','n','d','\n'};
//	
//	/*Test Data
//	*StructMapper(test);
//	std::vector<char> data = { 's','a','|','b','a','|','d' };
//	std::cout << FindAllFirstSymbolic(data).at(1) << std::endl;
//	std::vector<char> result;
//	CreateFirst('E', StructMapper(test), result);
//	for (auto x : result) std::cout << x << std::endl;
//	*/
//	
//	CreateAllFirst(test);
//}
