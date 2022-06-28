# FirstAndFollowGenerator
# ��ȡ�ķ�G��FIRST���ϡ�FOLLOW����
## �Զ����ķ���ʽ
```C++
start\n
A->BCc|gDB\n   
B->bCDE|%\n
C->DaB|ca\n
D->dD|%\n
E->gAf|c\n
end\n
```
## �ַ�����ȡ
1����**�Զ����ķ���ʽ**ת��Ϊ**Char��������**
```C++
char data[100] = { 's','t','a','r','t','\n','A','-','>','B','C','c','|','g','D','B','\n','B','-','>','b','C','D','E','|','%','\n','C','-','>','D','a','B','|','c','a','\n','D','-','>','d','D','|','%','\n','E','-','>','g','A','f','|','c','\n','e','n','d','\n' };
```
2����ȡ**Char����**����Ϊ**unordered_map����**
```
//��ȡ����A->aΪkey��value�ĸ�ʽ
```
```C++
unordered_map<char,vector<char>> map('A',{'a','b','|','B','d'}
```
3��**������ݹ�**

## FIRST������ȡ
1��**����|����֣�**E->ab|cd �ֱ��������vector��****
```C++
for (auto x : Data) {
		if (x == '|') {
			result.push_back({});
			continue;
		} 
		result.back().push_back(x);
	}
```
2��**�ݹ��ȡ�������ս����FIRST����[���ķ���]**
```
�����B����,��ôFIRST(A)={����}��(FIRST(B)-{��})��FIRST(CDEFG),���ŷֱ���FIRST(B)-{��}��FIRST(CDEFG)
 һ �����B����,��ôFIRST(A)={����}��FIRST(B),������FIRST(B)
 �� �����B����,��ôFIRST(A)={����}��(FIRST(B)-{��})��FIRST(CDEFG),���ŷֱ���FIRST(B)-{��}��FIRST(CDEFG)
```
```C++
void CreateChildFirst(char Symbolic, std::unordered_map<char, std::vector<char>> maps, std::vector<std::vector<char>> ChildData, std::vector<char>& result) {
	if (JudgeTerminator(Symbolic)) {
		result.push_back(Symbolic);
		return;
	}
	for (auto child : ChildData) {
		for (auto x : child) {
			if (1 == maps.count(x) || JudgeTerminator(x)) {
				CreateChildFirst(x,maps,GetAllDerivation(maps[x]), result);
				// ���ôεݹ鷵�ص��Ƿ�%�������ݹ飨��Ϊ�ҵ��ǿշ��ռ����ˣ�
				if (result.back() != '%' ) 
					break;
				else if(x != child.back())
					result.pop_back();
			}
		}
	}
}
```
**3����������map->first ���ϣ��ó�����FIRST(map->first)����**

## FOLLOW������ȡ
**1������һ���� $ �ŵ�follow��S���У�����S���ķ��Ŀ�ʼ���š�**
```C++
if (iter == maps.begin()) {
	temp.push_back('#');
}
```
**2��**��������������һ������ʽA����B�£���ôfirst���£��г���֮������з��Ŷ���follow��B���С�****
```
//����һ����ά��vector������÷��ź���ķ��Ų�Ϊ��|����÷��Ų������һ�����ͷ��Ϲ����
//���˷���֮�󣬡�|����vector.end()֮ǰ���е����ݼ��뵽һ��һά������
//���������һά������ɶ�ά��vector
```
```C++
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
```
**3�����������������һ������ʽA����B������ڲ���ʽA����B����first���£������ţ���ôfollow��A���е����з��Ŷ���follow��B���С�**
```
//����˸÷��ź���Ϊvector.end()���
//����˸÷��ź���Ϊ��|�����
//����˸÷��ź�FIRST(�÷��ź󣬡�|����vector.end()ǰ���з��ţ����ڿռ������
```
```C++
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
```
**˼·���������һ�͹����Ȼ���������**
```
// ����˼��
//����÷��ŵõ��Ĺ���������Ϊ�վ�ֱ�Ӽ���FOLLOW����
//�����Ϊ�վ��ڵ�ǰFOLLOW������Ѱ�����Ƶ�����FOLLOW����ƴ��
```
```C++
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
```

```C++
//����һ�͹����
std::unordered_map<char, std::vector<char>> temp = CreateAllRuleOneAndTwo(maps);
//���ù���һ�͹�������м������ɹ�����
std::unordered_map<char, std::vector<char>> result = CreateRuleThree(maps, temp);
```


