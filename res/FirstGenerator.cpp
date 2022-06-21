#include <iostream>
#include <vector>
#include <map>
#include "../config/SymbolCollection.h"

//�жϸ÷����Ƿ�Ϊ���ս��
static bool JudgeTerminator(char symbolic) {
	if (Terminator.find(symbolic) != Terminator.end())
		return true;
	else
		return false;
}

//�жϸ÷����Ƿ�Ϊ�ս��
static bool JudgeUnTerminator(char symbolic) {
	if (UnTerminator.find(symbolic) != UnTerminator.end())
		return true;
	else
		return false;
}

//��鵥���ķ����Ƿ���� ��|��,������ķ����������ַ�
static std::vector<char> FindAllFirstSymbolic(std::vector<char> &Data) {
	std::vector<char> symbolic;
	std::vector<char>::iterator Grammar = Data.begin();
	//���ķ��е�һ���ַ�����������
	if (Grammar != Data.end()) {
		symbolic.push_back(*Grammar);
		Grammar += 1;
	}
	else {
		std::cout << "Error: �ķ�����->�������κη���";
		__debugbreak();
	}
	//���ķ���ÿ����|��������ַ�����������
	while (Grammar != Data.end()) {
		if (*Grammar == '|') {
			if ((Grammar + 1) != Data.end()) {
				symbolic.push_back(*(Grammar + 1));
				Grammar += 2;
			}
			else {
				std::cout << "Error: �ķ�����|����δ�÷ŷ���";
				__debugbreak();
			}
		}else {
			Grammar += 1;
		}
	}
	return symbolic;
}

/*
	�ķ�ģ��
	start\n
	E->Tb\n
	T->a\n
	end\n
	start\nE->Tb\nT->a\nend\n
*/
//�����Ƶ�ӳ���ϵ E��>TE' =>  ��E': {'T''E''}<'char','char[]'>
static std::map<char, std::vector<char>>StructMapper(const char* Grammar) {
	std::map<char, std::vector<char>> maps;
	//�����ķ���󳤶�
	int index = 0;
	int size = 100;
	//find 'start'
	char start[5] = { 's','t' ,'a','r','t' };
	while (index < size) {
		int i = 0;
		// ��Ϊstart�����ظ��ַ�s����ÿ��ƥ��ʧ�ܶ��ӵ�ǰindex���¿�ʼ
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
		std::cout << "Error: �ķ��Ƿ� �ķ��� ��'\n'";
		__debugbreak();
	}
	if (index > 95) {
		std::cout << "Error: �ķ��Ƿ� start �����ķ�";
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
				std::cout << "Error: �ķ����� -> ǰ����Ϊ���ս�� �� �ķ���ʽ����";
				__debugbreak();
			}
			temp.clear();
		}
		break;
	}
	if (*Grammar != '\n') {
		std::cout << "Error: �ķ��Ƿ� �ķ��� ��'\n'";
		__debugbreak();
	}
	if (index > 96) {
		std::cout << "Error: �ķ����ȳ�������";
		__debugbreak();
	}
	//find 'end'
	char end[3] = { 'e','n' ,'d'};
	while (index < size) {
		int i = 0;
		// ��Ϊstart�����ظ��ַ�s����ÿ��ƥ��ʧ�ܶ��ӵ�ǰindex���¿�ʼ
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
		std::cout << "Error: �ķ��Ƿ� �ķ��� ��'\n'";
		__debugbreak();
	}
	return maps;
}

//�ݹ鷽ʽѰ�Ҹ÷��ռ�����First���ϣ����������result��
static void CreateFirst(char Symbolic, std::map<char, std::vector<char>> maps, std::vector<char>& result) {
	//���ڣ�����һ������Ϊ�ս��
	if (JudgeTerminator(Symbolic)) {
		result.push_back(Symbolic);
		return ;
	}
	//�ҵ��˷��ս����Ӧ������������ַ�����
	std::vector<char> temp = FindAllFirstSymbolic(maps[Symbolic]);
	//�ݹ���ý���������һ���ս������result����
	for (auto x : temp) {
		if(1 == maps.count(x))
			CreateFirst(x, maps, result);
		else {
			std::cout << "Error: �ķ��Ƿ� ���ڷ��ս���޷��Ƴ��ս�������'\n'";
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
		//Vector ȥ��
		std::set<int>s(result.begin(), result.end());
		result.assign(s.begin(), s.end());
		std::cout << iter->first << ":";
		for (auto x : result) std::cout << x ;
		result.clear();
		std::cout << std::endl;
	}

}


//����ʹ�õ�map�ṹΪ����ṹ���ʽ����A��B��C��D����
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