#include "FollowGenerator.h"

// 如果存在一个产生式A→αBβ，那么first（β）中除ε之外的所有符号都在follow（B）中。
// 收集所有符合规则二的符号，为下一步求first集合做准备
static std::vector<std::vector<char>> JudgeRulesTow(char Symbolic, std::vector<char> Deduce) {
	std::vector<std::vector<char>> result;
	int length = Deduce.size();
	for (int i = 0; i < length; i++) {
		// 如果该符号后面的符号不为‘|’或该符号不是最后一个，就符合规则二
		if (Deduce.at(i) == Symbolic && i + 1 < length && Deduce.at(i + 1) != '|') {
			result.push_back({});
			for (int j = i +1 ; j < length; j++) {
				if (Deduce.at(j) != '|')
					result.back().push_back(Deduce.at(j));
				else break;
			}
		}
	}
	return result;
}

// 解决有两个连续的空集
// 判断First集合中是否存在空集
static bool JudgeNoneInFirst(std::vector<char>  Tdata , std::unordered_map<char, std::vector<char>> maps) {
	std::vector<char> result;
	std::vector<std::vector<char>> ChildData;
	ChildData.push_back({});
	ChildData.back() = Tdata;
	bool flag = false;
	CreateChildFirst('A', maps, ChildData, result);
	for (auto x : result)
		if (x == '%')
			flag = true;
	return flag;
}

//遍历所有的产生式右部,找出所有产生式有部满足规则二的符号
static void TraversalAllGenerative(char Symbolic, std::unordered_map<char, std::vector<char>> maps, std::vector<std::vector<char>>& FirstList) {
	for (std::unordered_map<char, std::vector<char>>::iterator iter = maps.begin(); iter != maps.end(); iter++) {
		std::vector<std::vector<char>> temp;
		temp = JudgeRulesTow(Symbolic, iter->second);
		FirstList.insert(FirstList.end(), temp.begin(), temp.end());
		temp.clear();
	}
}

//First集流水线
static std::vector<char> FirstFlowline(std::unordered_map<char, std::vector<char>> maps, std::vector<std::vector<char>> FirstList) {
	std::vector<char> result;
	CreateChildFirst('E', maps, FirstList, result);
	return result;
}

// 如果存在一个产生式A→αB，或存在产生式A→αBβ且first（β）包含ε，那么follow（A）中的所有符号都在follow（B）中。
// 找到该终结符在文法中所有满足规则三的符号，返回一个列表
static std::vector<char> JudegeResultThree(char Symbolic, std::unordered_map<char, std::vector<char>> maps) {
	std::vector<char> result;
	for (std::unordered_map<char, std::vector<char>>::iterator iter = maps.begin(); iter != maps.end(); iter++) {
		int length = iter->second.size();
		std::vector<char> temp;
		for (int i = 0; i < length; i++) {
			if (iter->second.at(i) == Symbolic) {
				if (i + 1 == length) {
					result.push_back(iter->first);
				}
				else if (iter->second.at(i + 1) == '|' ) {
					result.push_back(iter->first);
				}
				else {
					for (int j = i + 1; j < length; j++) {
						if (iter->second.at(j) != '|')
							temp.push_back(iter->second.at(j));
						else break;
					}
				}
			}
		}
		if(JudgeNoneInFirst(temp, maps))
			result.push_back(iter->first);
		temp.clear();
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

//规则一和规则二
static std::unordered_map<char, std::vector<char>> CreateAllRuleOneAndTwo(std::unordered_map<char, std::vector<char>> maps) {
	// 遍历所有非终极符
	std::unordered_map<char, std::vector<char>> FollowTemp;
	for (std::unordered_map<char, std::vector<char>>::iterator iter = maps.begin(); iter != maps.end(); iter++) {
		std::vector<char> temp, tempsecond, tempthrid;
		//规则一：将 $ 放到follow（S）中，其中S是文法的开始符号。
		// 如果非终结符为开始符号E则将#加入
		if ((*iter).first == 'S') {
			temp.push_back('#');
		}
		std::vector<std::vector<char>> FirstList;
		std::vector<char> FollowList;

		// 生成该文法符号所有满足规则二待处理列表
		TraversalAllGenerative(iter->first, maps, FirstList);

		tempsecond = FirstFlowline(maps, FirstList);
		//符合规则二的符号集合用规则二的流水线并入Follw集map中
		temp.insert(temp.end(), tempsecond.begin(), tempsecond.end());

		FollowTemp.insert(std::unordered_map<char, std::vector<char>>::value_type(iter->first, temp));

		temp.clear();
	}
	return FollowTemp;
}
// 规则三
static std::unordered_map<char, std::vector<char>> CreateRuleThree(std::unordered_map<char, std::vector<char>> maps, std::unordered_map<char, std::vector<char>>FollowTemp) {
	// 遍历所有非终极符
	std::unordered_map<char, std::vector<char>> FollowStatus = FollowTemp;
	
	for (std::unordered_map<char, std::vector<char>>::iterator iter = maps.begin(); iter != maps.end(); iter++) {
		std::vector<char> FollowList, tempthrid, temp;
		FollowList = JudegeResultThree(iter->first, maps);
		if (FollowList.size()) {
			tempthrid = FollowFlowline(FollowStatus, FollowList);
		}
		//符合规则三的符号集合用规则三的流水线并入Follw集map中
		temp.insert(temp.end(), tempthrid.begin(), tempthrid.end());
		FollowStatus[iter->first].insert(FollowStatus[iter->first].end(),temp.begin(),temp.end());
	}
	for (std::unordered_map<char, std::vector<char>>::iterator iter = FollowStatus.begin(); iter != FollowStatus.end(); iter++) {
		//去重
		std::vector<char> temp;
		std::set<char>s(iter->second.begin(), iter->second.end());
		temp.assign(s.begin(), s.end());
		for (std::vector<char>::iterator iter = temp.begin(); iter != temp.end(); iter++) {
			if (*iter == '%')
				iter = temp.erase(iter);
		}
		iter->second = temp;
	}
	return FollowStatus;
}

//生成全部follow集合
std::unordered_map<char, std::vector<char>> CreateAllFollow(const char* InData) {
	std::unordered_map<char, std::vector<char>> maps = StructMapper(InData);
	std::unordered_map<char, std::vector<char>> temp = CreateAllRuleOneAndTwo(maps);
	std::unordered_map<char, std::vector<char>> result = CreateRuleThree(maps, temp);
	return result;
}


//int main() {
//			char test[100] = { 's','t','a','r','t','\n','A','-','>','B','C','c','|','g','D','B','\n','B','-','>','b','C','D','E','|','%','\n','C','-','>','D','a','B','|','c','a','\n','D','-','>','d','D','|','%','\n','E','-','>','g','A','f','|','c','\n','e','n','d','\n' };
//			std::unordered_map<char, std::vector<char>> result = CreateAllFollow(test);
//			for (std::unordered_map<char, std::vector<char>>::iterator iter = result.begin(); iter != result.end();iter++) {
//				std::cout << iter->first << " : { ";
//				for (auto x : iter->second) {
//					std::cout << x << " ";
//				}
//				std::cout << "}" << std::endl;
//			}
//			result = CreateAllFirst(test);
//			for (std::unordered_map<char, std::vector<char>>::iterator iter = result.begin(); iter != result.end(); iter++) {
//				std::cout << iter->first << " : { ";
//				for (auto x : iter->second) {
//					std::cout << x << " ";
//				}
//				std::cout << "}" << std::endl;
//			}
//	
//}