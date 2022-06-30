#include <iostream>
#include <vector>
#include <unordered_map>
#include "..\until\OpenFile.h"
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
//CString GetData(const char* InData) {
//	CString result;
//	std::unordered_map<char, std::vector<char>> tempResult = CreateAllFirst(InData);
//	result.Append(TransfromMapToCString(tempResult));
//	result.AppendChar('\r');
//	result.AppendChar('\n');
//	tempResult = CreateAllFollow(InData);
//	result.Append(TransfromMapToCString(tempResult));
//	return result;
//}
std::string GetData(const char* InData) {
	std::string result;
	std::unordered_map<char, std::vector<char>> tempResult = CreateAllFirst(InData);
	std::cout << "-----------First集------------\r\n";
	for (auto singal : tempResult)
	{
		std::cout << singal.first << "的First集为：";
		for (auto terminate : singal.second)
		{
			std::cout << terminate;
		}
		std::cout << std::endl;
	}
	/*result.Append(TransfromMapToCString(tempResult));
	result.AppendChar('\r');
	result.AppendChar('\n');*/
	tempResult = CreateAllFollow(InData);
	std::cout << "-----------Fllow集------------\r\n";
	for (auto singal : tempResult)
	{
		std::cout << singal.first << "的Fllow集为：";
		for (auto terminate : singal.second)
		{
			std::cout << terminate;
		}
		std::cout << std::endl;
	}
	/*result.Append(TransfromMapToCString(tempResult));*/
	return result;
}

int main() {
	std::string xx = XX();
	/*char test[100] = { 's','t','a','r','t','\n','L','-','>','A','\n','A','-','>','B','C','c','|','g','D','B','\n','B','-','>','b','C','D','E','|','%','\n','C','-','>','D','a','B','|','c','a','\n','D','-','>','d','D','|','%','\n','E','-','>','g','A','f','|','c','\n','e','n','d','\n' };
	*/
	std::cout << "文法为：" << std::endl;
	std::cout << xx;
	char test[100] = {0};
	strcpy(test, xx.c_str());
	//cout CString 一堆十六进制
	GetData(test);
	
}
