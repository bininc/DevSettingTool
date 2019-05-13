#pragma once
#pragma comment(lib,"Version.lib")

#include <vector>
#include <array>

struct Reg_Result
{
	std::vector<CString> Keys;
	CMap<CString, LPCTSTR, std::array<BYTE, 255>, std::array<BYTE, 255>> Values;
};

struct FileVerInfo
{
	CString CompanyName;
	CString FileDescription;
	CString FileVersion;
	CString InternalName;
	CString LegalCopyright;
	CString OriginalFilename;
	CString ProductName;
	CString ProductVersion;
};

class Common
{
public:
	static void GetComPorts(CMap<CString, LPCTSTR, DWORD, DWORD>& caports);
	static LSTATUS Reg_Query(HKEY rootKey, LPCTSTR path, Reg_Result& regResult);
	static void GetFileVerInfo(CString FileName,FileVerInfo& info);
	static void GetCurrentAppVerInfo(FileVerInfo& info);
	static CString GetCurrentAppDirectory();
	static CString GetModifyTime(CString appname);
	static void SplitStr(CString strSrc, CString strGap, CStringArray & strResult);
};

template <class T>
int getArrayLen(T&array)
{
	return sizeof(array) / sizeof(array[0]);
}