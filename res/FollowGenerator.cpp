#include "FollowGenerator.h"

// �������һ������ʽA����B�£���ôfirst���£��г���֮������з��Ŷ���follow��B���С�
// �ռ����з��Ϲ�����ķ��ţ�Ϊ��һ����first������׼��
static std::vector<std::vector<char>> JudgeRulesTow(char Symbolic, std::vector<char> Deduce) {
	std::vector<std::vector<char>> result;
	int length = Deduce.size();
	for (int i = 0; i < length; i++) {
		// ����÷��ź���ķ��Ų�Ϊ��|����÷��Ų������һ�����ͷ��Ϲ����
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

// ��������������Ŀռ�
// �ж�First�������Ƿ���ڿռ�
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

//�������еĲ���ʽ�Ҳ�,�ҳ����в���ʽ�в����������ķ���
static void TraversalAllGenerative(char Symbolic, std::unordered_map<char, std::vector<char>> maps, std::vector<std::vector<char>>& FirstList) {
	for (std::unordered_map<char, std::vector<char>>::iterator iter = maps.begin(); iter != maps.end(); iter++) {
		std::vector<std::vector<char>> temp;
		temp = JudgeRulesTow(Symbolic, iter->second);
		FirstList.insert(FirstList.end(), temp.begin(), temp.end());
		temp.clear();
	}
}

//First����ˮ��
static std::vector<char> FirstFlowline(std::unordered_map<char, std::vector<char>> maps, std::vector<std::vector<char>> FirstList) {
	std::vector<char> result;
	CreateChildFirst('E', maps, FirstList, result);
	return result;
}

// �������һ������ʽA����B������ڲ���ʽA����B����first���£������ţ���ôfollow��A���е����з��Ŷ���follow��B���С�
// �ҵ����ս�����ķ�����������������ķ��ţ�����һ���б�
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

//Follow����ˮ��
// ���{maps����ǰ������֪Follow����״̬��FollowList������Follow�Ӽ��б�}
static std::vector<char> FollowFlowline(std::unordered_map<char, std::vector<char>> maps, std::vector<char> FollowList) {
	std::vector<char> result;
	for (std::vector<char>::iterator iter = FollowList.begin(); iter != FollowList.end(); iter++) {
		// ��FollowList�е�Follow���϶����뵽result����ΪFollow�����󼯺ϣ����Ӽ�
		result.insert(result.end(), maps[*iter].begin(), maps[*iter].end());
	}
	return result;
}

//����һ�͹����
static std::unordered_map<char, std::vector<char>> CreateAllRuleOneAndTwo(std::unordered_map<char, std::vector<char>> maps) {
	// �������з��ռ���
	std::unordered_map<char, std::vector<char>> FollowTemp;
	for (std::unordered_map<char, std::vector<char>>::iterator iter = maps.begin(); iter != maps.end(); iter++) {
		std::vector<char> temp, tempsecond, tempthrid;
		//����һ���� $ �ŵ�follow��S���У�����S���ķ��Ŀ�ʼ���š�
		// ������ս��Ϊ��ʼ����E��#����
		if ((*iter).first == 'S') {
			temp.push_back('#');
		}
		std::vector<std::vector<char>> FirstList;
		std::vector<char> FollowList;

		// ���ɸ��ķ������������������������б�
		TraversalAllGenerative(iter->first, maps, FirstList);

		tempsecond = FirstFlowline(maps, FirstList);
		//���Ϲ�����ķ��ż����ù��������ˮ�߲���Follw��map��
		temp.insert(temp.end(), tempsecond.begin(), tempsecond.end());

		FollowTemp.insert(std::unordered_map<char, std::vector<char>>::value_type(iter->first, temp));

		temp.clear();
	}
	return FollowTemp;
}
// ������
static std::unordered_map<char, std::vector<char>> CreateRuleThree(std::unordered_map<char, std::vector<char>> maps, std::unordered_map<char, std::vector<char>>FollowTemp) {
	// �������з��ռ���
	std::unordered_map<char, std::vector<char>> FollowStatus = FollowTemp;
	
	for (std::unordered_map<char, std::vector<char>>::iterator iter = maps.begin(); iter != maps.end(); iter++) {
		std::vector<char> FollowList, tempthrid, temp;
		FollowList = JudegeResultThree(iter->first, maps);
		if (FollowList.size()) {
			tempthrid = FollowFlowline(FollowStatus, FollowList);
		}
		//���Ϲ������ķ��ż����ù���������ˮ�߲���Follw��map��
		temp.insert(temp.end(), tempthrid.begin(), tempthrid.end());
		FollowStatus[iter->first].insert(FollowStatus[iter->first].end(),temp.begin(),temp.end());
	}
	for (std::unordered_map<char, std::vector<char>>::iterator iter = FollowStatus.begin(); iter != FollowStatus.end(); iter++) {
		//ȥ��
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

//����ȫ��follow����
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