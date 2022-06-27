#include <iostream>
#include <vector>
#include <unordered_map>
#include "GramarExtraction.h"
#include "../until/SymbolCollection.h"
#include "FirstGenerator.h"

// �������һ������ʽA����B�£���ôfirst���£��г���֮������з��Ŷ���follow��B���С�
// �ռ����з��Ϲ�����ķ��ţ�Ϊ��һ����first������׼��
static std::vector<char> JudgeRulesTow(char Symbolic, std::vector<char> Deduce) {
	std::vector<char> result;
	int length = Deduce.size();
	for (int i = 0; i < length; i++) {
		// ����÷��ź���ķ��Ų�Ϊ��|����÷��Ų������һ�����ͷ��Ϲ����

		if (Deduce.at(i) == Symbolic && i + 1 < length && Deduce.at(i + 1) != '|') {
			
				result.push_back(Deduce.at(i + 1));	
		}
	}
	return result;
}

// �ж�First�������Ƿ���ڿռ�
static bool JudgeNoneInFirst(char Symbolic,std::unordered_map<char, std::vector<char>> maps) {
	std::vector<char> result;
	bool flag = false;
	CreateFirst(Symbolic, maps, result);
	for (auto x : result)
		if (x == '%')
			!flag;
	return flag;
}

//�������еĲ���ʽ�Ҳ�,�ҳ����в���ʽ�в����������ķ���
static void TraversalAllGenerative(char Symbolic, std::unordered_map<char, std::vector<char>> maps, std::vector<char>& FirstList) {
	for (std::unordered_map<char, std::vector<char>>::iterator iter = maps.begin(); iter != maps.end(); iter++) {
		std::vector<char> temp;
		temp = JudgeRulesTow(Symbolic, iter->second);
		FirstList.insert(FirstList.end(), temp.begin(), temp.end());
		temp.clear();
	}
}

//First����ˮ��
static std::vector<char> FirstFlowline(std::unordered_map<char, std::vector<char>> maps, std::vector<char> FirstList) {
	std::vector<char> result;

	for (std::vector<char>::iterator iter = FirstList.begin(); iter != FirstList.end(); iter++) {
		// Ϊ����First�б���ÿһ��Ԫ����Fitst���ϲ����뼯����
		CreateFirst(*iter, maps, result);
	}
	return result;
}

// �������һ������ʽA����B������ڲ���ʽA����B����first���£������ţ���ôfollow��A���е����з��Ŷ���follow��B���С�
// �ҵ����ս�����ķ�����������������ķ��ţ�����һ���б�
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

//�ݹ鷽ʽѰ�Ҹ÷��ռ�����First���ϣ����������result��
static std::unordered_map<char, std::vector<char>> CreateFollow(std::unordered_map<char, std::vector<char>> maps) {
	// �������з��ռ���
	std::unordered_map<char, std::vector<char>> FollowStatus;
	for (std::unordered_map<char, std::vector<char>>::iterator iter = maps.begin(); iter != maps.end(); iter++) {
		std::vector<char> temp, tempsecond, tempthrid;
		//����һ���� $ �ŵ�follow��S���У�����S���ķ��Ŀ�ʼ���š�
		// ������ս��Ϊ��ʼ����E��#����
		if (iter->first == 'E') {
			temp.push_back('#');
		}
		std::vector<char> FirstList;
		std::vector<char> FollowList;

		// ���ɸ��ķ������������������������б�
		TraversalAllGenerative(iter->first, maps, FirstList);

		tempsecond = FirstFlowline(maps, FirstList);
		//���Ϲ�����ķ��ż����ù��������ˮ�߲���Follw��map��
		temp.insert(temp.end(), tempsecond.begin(), tempsecond.end());

		FollowList = JudegeResultThree(iter->first, maps);
		tempthrid = FollowFlowline(FollowStatus, FollowList);

		//���Ϲ������ķ��ż����ù���������ˮ�߲���Follw��map��
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