#include "FirstGenerator.h"


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


//�ݹ鷽ʽѰ�Ҹ÷��ռ�����First���ϣ����������result��
void CreateFirst(char Symbolic, std::unordered_map<char, std::vector<char>> maps, std::vector<char>& result) {
	//���ڣ�����һ������Ϊ�ս��
	if (JudgeTerminator(Symbolic)) {
		result.push_back(Symbolic);
		return ;
	}
	//�ҵ��˷��ս����Ӧ������������ַ�����
	std::vector<char> temp = FindAllFirstSymbolic(maps[Symbolic]);
	//�ݹ���ý���������һ���ս������result����
	for (auto x : temp) {
		
		if(1 == maps.count(x)||JudgeTerminator(x))
			CreateFirst(x, maps, result);
		else {
			std::cout << "Error: �ķ��Ƿ� ���ڷ��ս���޷��Ƴ��ս�������'\n'";
			__debugbreak();
		}
	}
	return;
}

// ����Firsit����
void TraversalAllUnTerminator(char *data) {
	std::unordered_map<char, std::vector<char>> maps = StructMapper(data);
	std::vector<char> result;
	for (std::unordered_map<char, std::vector<char>>::const_iterator iter = maps.begin(); iter != maps.end(); ++iter) {
		//CreateFirst(iter->first, maps, result);
		CreateChildFirst(iter->first, maps, GetAllDerivation(iter->second), result);
		//Vector ȥ��
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

//����ʹ�õ�map�ṹΪ����ṹ���ʽ����A��B��C��D����
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
