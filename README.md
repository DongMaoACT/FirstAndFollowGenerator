# FirstAndFollowGenerator
# 提取文法G的FIRST集合、FOLLOW集合
## 自定义文法格式
```C++
start\n
A->BCc|gDB\n   
B->bCDE|%\n
C->DaB|ca\n
D->dD|%\n
E->gAf|c\n
end\n
```
## 字符串提取
1、将**自定义文法格式**转换为**Char类型数组**
```C++
char data[100] = { 's','t','a','r','t','\n','A','-','>','B','C','c','|','g','D','B','\n','B','-','>','b','C','D','E','|','%','\n','C','-','>','D','a','B','|','c','a','\n','D','-','>','d','D','|','%','\n','E','-','>','g','A','f','|','c','\n','e','n','d','\n' };
```
2、提取**Char数组**内容为**unordered_map类型**
```
//提取所有A->a为key：value的格式
```
```C++
unordered_map<char,vector<char>> map('A',{'a','b','|','B','d'}
```
3、**消除左递归**

## FIRST集合提取
1、**按‘|’拆分：**E->ab|cd 分别存入两个vector中****
```C++
for (auto x : Data) {
		if (x == '|') {
			result.push_back({});
			continue;
		} 
		result.back().push_back(x);
	}
```
2、**递归获取单个非终结符的FIRST集合[核心方法]**
```
如果有B→ε,那么FIRST(A)={……}∪(FIRST(B)-{ε})∪FIRST(CDEFG),接着分别求FIRST(B)-{ε}、FIRST(CDEFG)
 一 如果无B→ε,那么FIRST(A)={……}∪FIRST(B),接着求FIRST(B)
 二 如果有B→ε,那么FIRST(A)={……}∪(FIRST(B)-{ε})∪FIRST(CDEFG),接着分别求FIRST(B)-{ε}、FIRST(CDEFG)
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
				// 当该次递归返回的是非%，结束递归（因为找到非空非终极符了）
				if (result.back() != '%' ) 
					break;
				else if(x != child.back())
					result.pop_back();
			}
		}
	}
}
```
**3、遍历所有map->first 集合，得出所有FIRST(map->first)集合**

## FOLLOW集合提取
**1、规则一：将 $ 放到follow（S）中，其中S是文法的开始符号。**
```C++
if (iter == maps.begin()) {
	temp.push_back('#');
}
```
**2、**规则二：如果存在一个产生式A→αBβ，那么first（β）中除ε之外的所有符号都在follow（B）中。****
```
//返回一个二维的vector，如果该符号后面的符号不为‘|’或该符号不是最后一个，就符合规则二
//将此符号之后，’|‘或vector.end()之前所有的内容加入到一个一维向量中
//多个这样的一维向量组成二维的vector
```
```C++
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
```
**3、规则三：如果存在一个产生式A→αB，或存在产生式A→αBβ且first（β）包含ε，那么follow（A）中的所有符号都在follow（B）中。**
```
//解决了该符号后面为vector.end()情况
//解决了该符号后面为’|”情况
//解决了该符号后FIRST(该符号后，‘|’或vector.end()前所有符号）存在空集的情况
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
**思路：先求规则一和规则二然后求规则三**
```
// 核心思想
//如果该符号得到的规则三集合为空就直接加入FOLLOW集合
//如果不为空就在当前FOLLOW集合中寻找以推导出的FOLLOW集合拼接
```
```C++
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
```

```C++
//规则一和规则二
std::unordered_map<char, std::vector<char>> temp = CreateAllRuleOneAndTwo(maps);
//采用规则一和规则二的中间结果生成规则三
std::unordered_map<char, std::vector<char>> result = CreateRuleThree(maps, temp);
```


