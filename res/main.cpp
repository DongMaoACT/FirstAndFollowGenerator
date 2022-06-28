#include <iostream>
#include <vector>
#include <unordered_map>
#include "FirstGenerator.h"
#include "FollowGenerator.h"
#include "atlstr.h"

CString TransfromMapToCString(std::unordered_map<char, std::vector<char>> &tempResult) {
	CString result;
	for (std::unordered_map<char, std::vector<char>>::iterator iter = tempResult.begin(); iter != tempResult.end(); iter++) {
		if (iter != tempResult.begin()) {
			result.AppendChar('\r');
			result.AppendChar('\n');
		}
		result.AppendChar(iter->first);
		result.AppendChar(':');
		for (auto x : iter->second) {
			result.AppendChar(x);
		}
	}
	return result;
}
CString GetData(const char* InData) {
	CString result;
	std::unordered_map<char, std::vector<char>> tempResult = CreateAllFirst(InData);
	result.Append(TransfromMapToCString(tempResult));
	result.AppendChar('\r');
	result.AppendChar('\n');
	tempResult = CreateAllFollow(InData);
	result.Append(TransfromMapToCString(tempResult));
	return result;
}
int main() {
	char test[100] = { 's','t','a','r','t','\n','A','-','>','B','C','c','|','g','D','B','\n','B','-','>','b','C','D','E','|','%','\n','C','-','>','D','a','B','|','c','a','\n','D','-','>','d','D','|','%','\n','E','-','>','g','A','f','|','c','\n','e','n','d','\n' };
	//cout CString 一堆十六进制
	std::cout << GetData(test);
	
}
