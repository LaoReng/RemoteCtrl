#pragma once
class CLTools
{
public:
	static void ErrorOut(const CString& errInfo, const CString& filePath, const int& line) {
		CString str;
		str.Format(_T("%s(%d): %s\r\n"), filePath, line, errInfo);
		OutputDebugString(str);
	}
};

