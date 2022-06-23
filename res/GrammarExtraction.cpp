#include "../until/SymbolCollection.h"
#include "GramarExtraction.h"

/*
	�ķ�ģ��
	start\n
	E->Tb\n
	T->a\n
	end\n
	start\nE->Tb\nT->a\nend\n
*/
//�����Ƶ�ӳ���ϵ E��>TE' =>  ��E': {'T''E''}<'char','char[]'>
std::unordered_map<char, std::vector<char>>StructMapper(const char* Grammar) {
	std::unordered_map<char, std::vector<char>> maps;
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
	char end[3] = { 'e','n' ,'d' };
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
