#include "OpenFile.h"
std::string XX() {
	HANDLE hOpenFile = (HANDLE)CreateFile(_T("read.txt"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hOpenFile == INVALID_HANDLE_VALUE)
	{
		hOpenFile = NULL;
		MessageBoxA(NULL, "Can not open the file", "Playwav", MB_OK);
	}
	int fileSize = GetFileSize(hOpenFile, NULL);
	char* pBuffer = (char*)malloc(fileSize);
	std::string rBuffer = "start\\n";
	DWORD RSize = 0;
	std::deque<char> first;
	std::deque<char> second;
	char xx[] = "end\\n";
	char yy[] = "start\\n";
	ReadFile(hOpenFile, pBuffer, fileSize, &RSize, NULL);
	/*for (int i = 0; i < sizeof(yy); i++)
	{
		rBuffer[i] = p
	}*/
	for (int i = 1 + sizeof(yy); i < RSize; i++)
	{
		if (*(&pBuffer[i]) == *xx)
		{
			break;
		}
		if (pBuffer[i + 2] == '\r')
		{
			if (pBuffer[i + 4] == first.front())
			{
				i = i + 6;
				rBuffer += "|";
			}
			else
			{
				first.clear();
				rBuffer += "\\n";
				i = i + 3;
			}

		}
		else {
			first.push_back(pBuffer[i]);
			rBuffer += pBuffer[i];
		}
	}
	rBuffer += "end\\n";
	free(pBuffer);
	CloseHandle(hOpenFile);
	return rBuffer;
}