#include <iostream>
#include <vector>
#include <unordered_map>
#include "GramarExtraction.h"
#include "../until/SymbolCollection.h"
#include "FirstGenerator.h"

// 如果存在一个产生式A→αBβ，那么first（β）中除ε之外的所有符号都在follow（B）中。
// 收集所有符合规则二的符号，为下一步求first集合做准备
static std::vector<char> JudgeRulesTow(char Symbolic, std::vector<char> Deduce) {
	std::vector<char> result;
	int length = Deduce.size();
	for (int i = 0; i < length; i++) {
		// 如果该符号后面的符号不为‘|’或该符号不是最后一个，就符合规则二

		if (Deduce.at(i) == Symbolic && i + 1 < length && Deduce.at(i + 1) != '|') {
			
				result.push_back(Deduce.at(i + 1));	
		}
	}
	return result;
}

// 判断First集合中是否存在空集
static bool JudgeNoneInFirst(char Symbolic,std::unordered_map<char, std::vector<char>> maps) {
	std::vector<char> result;
	bool flag = false;
	CreateFirst(Symbolic, maps, result);
	for (auto x : result)
		if (x == '%')
			!flag;
	return flag;
}

//遍历所有的产生式右部,找出所有产生式有部满足规则二的符号
static void TraversalAllGenerative(char Symbolic, std::unordered_map<char, std::vector<char>> maps, std::vector<char>& FirstList) {
	for (std::unordered_map<char, std::vector<char>>::iterator iter = maps.begin(); iter != maps.end(); iter++) {
		std::vector<char> temp;
		temp = JudgeRulesTow(Symbolic, iter->second);
		FirstList.insert(FirstList.end(), temp.begin(), temp.end());
		temp.clear();
	}
}

//First集流水线
static std::vector<char> FirstFlowline(std::unordered_map<char, std::vector<char>> maps, std::vector<char> FirstList) {
	std::vector<char> result;

	for (std::vector<char>::iterator iter = FirstList.begin(); iter != FirstList.end(); iter++) {
		// 为待求First列表中每一个元素求Fitst集合并加入集合中
		CreateFirst(*iter, maps, result);
	}
	return result;
}

// 如果存在一个产生式A→αB，或存在产生式A→αBβ且first（β）包含ε，那么follow（A）中的所有符号都在follow（B）中。
// 找到该终结符在文法中所有满足规则三的符号，返回一个列表
static std::vector<char> JudegeResultThree(char Symbolic, std::unordered_map<char, std::vector<char>> maps) {
	std::vector<char> result;
	for (std::unordered_map<char, std::vector<char>>::iterator iter = maps.begin(); iter != maps.end(); iter++) {
		int length = iter->second.size();
		for (int i = 0; i < length; i++) {
			if (iter->second.at(i) == Symbolic) {
				if (i + 1 == length) {
					result.push_back(iter->first);
				}
				else if (iter->second.at(i + 1) == '|' || JudgeNoneInFirst(iter->second.at(i + 1), maps)) {
					result.push_back(iter->first);
				}
			}
		}
	}
	return result;
}

//Follow集流水线
// 入参{maps：当前所有已知Follow集合状态，FollowList：待求Follow子集列表}
static std::vector<char> FollowFlowline(std::unordered_map<char, std::vector<char>> maps, std::vector<char> FollowList) {
	std::vector<char> result;
	for (std::vector<char>::iterator iter = FollowList.begin(); iter != FollowList.end(); iter++) {
		// 将FollowList中的Follow集合都加入到result中作为Follow（代求集合）的子集
		result.insert(result.end(), maps[*iter].begin(), maps[*iter].end());
	}
	return result;
}

//递归方式寻找该非终极符的First集合，结果保存在result中
static std::unordered_map<char, std::vector<char>> CreateFollow(std::unordered_map<char, std::vector<char>> maps) {
	// 遍历所有非终极符
	std::unordered_map<char, std::vector<char>> FollowStatus;
	for (std::unordered_map<char, std::vector<char>>::iterator iter = maps.begin(); iter != maps.end(); iter++) {
		std::vector<char> temp, tempsecond, tempthrid;
		//规则一：将 $ 放到follow（S）中，其中S是文法的开始符号。
		// 如果非终结符为开始符号E则将#加入
		if (iter->first == 'E') {
			temp.push_back('#');
		}
		std::vector<char> FirstList;
		std::vector<char> FollowList;

		// 生成该文法符号所有满足规则二待处理列表
		TraversalAllGenerative(iter->first, maps, FirstList);

		tempsecond = FirstFlowline(maps, FirstList);
		//符合规则二的符号集合用规则二的流水线并入Follw集map中
		temp.insert(temp.end(), tempsecond.begin(), tempsecond.end());

		FollowList = JudegeResultThree(iter->first, maps);
		tempthrid = FollowFlowline(FollowStatus, FollowList);

		//符合规则三的符号集合用规则三的流水线并入Follw集map中
		temp.insert(temp.end(), tempthrid.begin(), tempthrid.end());
		FollowStatus.insert(std::unordered_map<char, std::vector<char>>::value_type(iter->first, temp));

		temp.clear();
	}
	return FollowStatus;
}

int main() {
	char test[50] = { 's','t','a','r','t','\n','E','-','>','a','c','|','%','|','A','c','|','A','|','D','\n','A','-','>','b','|','c','b','|','D','|','D','d','\n','D','-','>','%','\n','C','-','>','a','\n','e','n','d','\n'};
	/*
	Test Data
	*StructMapper(test);
	std::vector<char> data = { 's','a','|','b','a','|','d' };
	std::cout << FindAllFirstSymbolic(data).at(1) << std::endl;
	std::vector<char> result;
	CreateFirst('E', StructMapper(test), result);
	for (auto x : result) std::cout << x << std::endl;
	*/
	std::unordered_map<char, std::vector<char>> maps = StructMapper(test);
	std::unordered_map<char, std::vector<char>> result = CreateFollow(maps);

	
	
	std::cout << "1" << std::endl;
}