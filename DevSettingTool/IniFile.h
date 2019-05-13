#pragma once

class CIniFile
{
public:
	CString GetClassVersion();
	DWORD IniFile_GetKeyNames(CString strSectionName, CStringArray &strArray);

	UINT GetMaxSize_A_Section();
	UINT GetMaxSize_All_SectionNames();

	DWORD IniFile_GetKeyNamesValues(CString strSectionName,CStringArray &strArray);
	BOOL  IniFile_GetStruct(LPCTSTR lpSectionName,LPCTSTR lpKeyName,LPVOID lpStruct,UINT uSizeStruct);
	DWORD IniFile_GetSectionNames(CStringArray &strArray );


	BOOL  IniFile_DeleteStruct(LPCTSTR lpSectionName, LPCTSTR lpKeyName);
	BOOL  IniFile_DeleteSection(CString strSectionName);
	BOOL  IniFile_DeleteString(CString strSectionName,CString strKeyName);

	BOOL  IniFile_ModifyString(LPCTSTR lpSectionName,LPCTSTR lpKeyName,LPCTSTR lpString);
	BOOL  IniFile_ModifyInt(LPCTSTR lpSectionName, LPCTSTR lpKeyName, int nValue);


	BOOL  IniFile_WriteStruct(LPCTSTR lpSectionName,LPCTSTR lpKeyName,LPVOID lpStruct,UINT uSizeStruct);
	BOOL  IniFile_WriteSection(LPCTSTR lpSectionName, LPCTSTR lpString);
	BOOL  IniFile_WriteLongString(LPCTSTR lpValue,LPCTSTR lpSectionName,LPCTSTR lpKeyName,  ...);

	BOOL  IniFile_WriteString(LPCTSTR lpSectionName,LPCTSTR lpKeyName,LPCTSTR lpString);
	BOOL  IniFile_WriteInt(LPCTSTR lpSectionName, LPCTSTR lpKeyName, int nValue);

	DWORD IniFile_GetString(CString strSectionName,CString strKeyName,CString strDefault,CString &strReturnedString);
	DWORD IniFile_GetString(LPCTSTR lpSectionName,LPCTSTR lpKeyName,LPCTSTR lpDefault,LPTSTR lpReturnedString,DWORD nSize);
	UINT  IniFile_GetInt(LPCTSTR lpSectionName,LPCTSTR lpKeyName,INT nDefault);



	BOOL    EnsureIniFileExist();

	CString GetFilePathName();
	void    SetDefaultFilePathName();
	BOOL    SetFileName(LPCTSTR lpFileName,LPCTSTR lpFileExt = _T(".ini"));
	BOOL    SetFileAllName(LPCTSTR lpFileName,LPCTSTR lpPathName);
	void    SetMaxSize_All_SectionNames(UINT nSize);
	void    SetMaxSize_A_Section(UINT nSize );

	CIniFile();
	virtual ~CIniFile();
	BOOL    IsIniFileExist();
protected:
	


	DWORD   IniFile_GetSectionNames(LPTSTR lpszReturnBuffer, DWORD nSize);
	DWORD   IniFile_GetSection(LPCTSTR lpSectionName,LPTSTR lpReturnedString, DWORD nSize);


private:
	CString m_strFileFullName;
	UINT    nMaxSize_All_SectionNames;
	UINT    nMaxSize_A_Section;


};
