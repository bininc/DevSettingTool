#include "stdafx.h"
#include "Common.h"

#define MAX_KEY_LENGTH 255  
#define MAX_VALUE_NAME 16383  
DWORD dwType = REG_BINARY | REG_DWORD | REG_EXPAND_SZ | REG_MULTI_SZ | REG_NONE | REG_SZ;


void Common::GetComPorts(CMap<CString, LPCTSTR, DWORD, DWORD>& caports)
{
	Reg_Result regResult;
	LSTATUS code = Reg_Query(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), regResult);
	if (code == ERROR_SUCCESS)
	{
		if (regResult.Values.IsEmpty())
		{
			throw "未检测到串口设备插入电脑";
		}
		else
		{
			POSITION pos = regResult.Values.GetStartPosition();
			CString key;
			std::array<BYTE, 255> value;
			CString comStr;
			while (pos)
			{
				regResult.Values.GetNextAssoc(pos, key, value);
				comStr.Format(_T("%s"), value.data());
				CString comPort = comStr;
				comPort.Delete(0, 3);
				caports.SetAt(comStr, _ttoi(comPort));
			}
		}
	}
	else if (code == ERROR_FILE_NOT_FOUND) {
		throw "未检测到串口设备插入电脑";
	}
	else
	{
		throw "获取串口列表失败";
	}
}

LSTATUS Common::Reg_Query(HKEY rootKey, LPCTSTR path, Reg_Result& regResult)
{
	HKEY hKey;
	DWORD retCode = RegOpenKeyEx(rootKey, path, 0, KEY_READ, &hKey);
	if (retCode != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return retCode;
	}

	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name  
	DWORD    cbName;                   // size of name string   
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name   
	DWORD    cchClassName = MAX_PATH;  // size of class string   
	DWORD    cSubKeys = 0;               // number of subkeys   
	DWORD    cbMaxSubKey;              // longest subkey size   
	DWORD    cchMaxClass;              // longest class string   
	DWORD    cValues;              // number of values for key   
	DWORD    cchMaxValue;          // longest value name   
	DWORD    cbMaxValueData;       // longest value data   
	DWORD    cbSecurityDescriptor; // size of security descriptor   
	FILETIME ftLastWriteTime;      // last write time   

	DWORD i;

	TCHAR  achValue[MAX_VALUE_NAME];
	DWORD cchValue = MAX_VALUE_NAME;

	// Get the class name and the value count.   
	retCode = RegQueryInfoKey(
		hKey,                    // key handle   
		achClass,                // buffer for class name   
		&cchClassName,           // size of class string   
		NULL,                    // reserved   
		&cSubKeys,               // number of subkeys   
		&cbMaxSubKey,            // longest subkey size   
		&cchMaxClass,            // longest class string   
		&cValues,                // number of values for this key   
		&cchMaxValue,            // longest value name   
		&cbMaxValueData,         // longest value data   
		&cbSecurityDescriptor,   // security descriptor   
		&ftLastWriteTime);       // last write time   

	// Enumerate the subkeys, until RegEnumKeyEx fails.  
	if (cSubKeys)
	{
		for (i = 0; i < cSubKeys; i++)
		{
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, i,
				achKey,
				&cbName,
				NULL,
				NULL,
				NULL,
				&ftLastWriteTime);
			if (retCode == ERROR_SUCCESS)
			{
				//use achKey to build new path and input it into stack.
				CString newPath;
				newPath.Append(path);
				newPath.Append(_T("\\"));
				newPath.Append(achKey);
				regResult.Keys.push_back(newPath);
			}
		}
	}

	// Enumerate the key values.   
	if (cValues)
	{
		for (i = 0, retCode = ERROR_SUCCESS; i < cValues; i++)
		{
			cchValue = MAX_VALUE_NAME;
			achValue[0] = '\0';
			unsigned char vari[70];
			retCode = RegEnumValue(hKey, i,
				achValue,
				&cchValue,
				NULL,
				NULL,
				NULL,
				NULL);
			if (retCode == ERROR_SUCCESS)
			{
				BYTE szBuffer[255] = { 0 };
				DWORD dwNameLen = 255;
				DWORD rQ = RegQueryValueEx(hKey, achValue, 0, &dwType, szBuffer, &dwNameLen);
				if (rQ == ERROR_SUCCESS)
				{
					std::array<BYTE, 255> arr;
					for (auto j = 0; j < 255; j++)
					{
						arr[j] = szBuffer[j];
					}
					regResult.Values.SetAt(achValue, arr);
				}

			}
		}
	}
	//release.
	RegCloseKey(hKey);
	return retCode;
}

void Common::GetFileVerInfo(CString FileName, FileVerInfo& info)
{
	/* 获得版本信息大小 */
	UINT l_uiVersionInfoSize;//保存版本信息总体的大小
	TCHAR * l_ptcVersionInfo;
	l_uiVersionInfoSize = ::GetFileVersionInfoSize(FileName, 0);//获得大小 
	l_ptcVersionInfo = new TCHAR[l_uiVersionInfoSize];//申请空间  

	/* 该结构用于获得版本信息的语言信息 */
	struct VersionLanguage
	{
		WORD m_wLanguage;
		WORD m_wCcodePage;
	};

	VersionLanguage * l_ptVersionLanguage;
	UINT l_uiSize;

	if (::GetFileVersionInfo(FileName, 0, l_uiVersionInfoSize, l_ptcVersionInfo) != 0)//获取版本信息 
	{

		if (::VerQueryValue(l_ptcVersionInfo, _T("\\VarFileInfo\\Translation"), reinterpret_cast<LPVOID*>(&l_ptVersionLanguage), &l_uiSize))//查询语言信息并保存
		{
			/* 生成查询信息格式符 */
			CString l_cstrSubBlock;
			LPVOID * l_pvResult;

			l_cstrSubBlock.Format(_T("\\StringFileInfo\\%04x%04x\\CompanyName"), l_ptVersionLanguage->m_wLanguage, l_ptVersionLanguage->m_wCcodePage);
			/* 查询指定信息 */
			if (::VerQueryValue(static_cast<LPVOID>(l_ptcVersionInfo), l_cstrSubBlock.GetBuffer(), reinterpret_cast<LPVOID*>(&l_pvResult), &l_uiSize))
			{
				CString l_cstrvResult(reinterpret_cast<TCHAR *>(l_pvResult));//获得版本信息
				info.CompanyName = l_cstrvResult;
			}
			l_cstrSubBlock.Format(_T("\\StringFileInfo\\%04x%04x\\FileDescription"), l_ptVersionLanguage->m_wLanguage, l_ptVersionLanguage->m_wCcodePage);
			/* 查询指定信息 */
			if (::VerQueryValue(static_cast<LPVOID>(l_ptcVersionInfo), l_cstrSubBlock.GetBuffer(), reinterpret_cast<LPVOID*>(&l_pvResult), &l_uiSize))
			{
				CString l_cstrvResult(reinterpret_cast<TCHAR *>(l_pvResult));//获得版本信息
				info.FileDescription = l_cstrvResult;
			}
			l_cstrSubBlock.Format(_T("\\StringFileInfo\\%04x%04x\\FileVersion"), l_ptVersionLanguage->m_wLanguage, l_ptVersionLanguage->m_wCcodePage);
			/* 查询指定信息 */
			if (::VerQueryValue(static_cast<LPVOID>(l_ptcVersionInfo), l_cstrSubBlock.GetBuffer(), reinterpret_cast<LPVOID*>(&l_pvResult), &l_uiSize))
			{
				CString l_cstrvResult(reinterpret_cast<TCHAR *>(l_pvResult));//获得版本信息
				info.FileVersion = l_cstrvResult;
			}
			l_cstrSubBlock.Format(_T("\\StringFileInfo\\%04x%04x\\InternalName"), l_ptVersionLanguage->m_wLanguage, l_ptVersionLanguage->m_wCcodePage);
			/* 查询指定信息 */
			if (::VerQueryValue(static_cast<LPVOID>(l_ptcVersionInfo), l_cstrSubBlock.GetBuffer(), reinterpret_cast<LPVOID*>(&l_pvResult), &l_uiSize))
			{
				CString l_cstrvResult(reinterpret_cast<TCHAR *>(l_pvResult));//获得版本信息
				info.InternalName = l_cstrvResult;
			}
			l_cstrSubBlock.Format(_T("\\StringFileInfo\\%04x%04x\\LegalCopyright"), l_ptVersionLanguage->m_wLanguage, l_ptVersionLanguage->m_wCcodePage);
			/* 查询指定信息 */
			if (::VerQueryValue(static_cast<LPVOID>(l_ptcVersionInfo), l_cstrSubBlock.GetBuffer(), reinterpret_cast<LPVOID*>(&l_pvResult), &l_uiSize))
			{
				CString l_cstrvResult(reinterpret_cast<TCHAR *>(l_pvResult));//获得版本信息
				info.LegalCopyright = l_cstrvResult;
			}
			l_cstrSubBlock.Format(_T("\\StringFileInfo\\%04x%04x\\OriginalFilename"), l_ptVersionLanguage->m_wLanguage, l_ptVersionLanguage->m_wCcodePage);
			/* 查询指定信息 */
			if (::VerQueryValue(static_cast<LPVOID>(l_ptcVersionInfo), l_cstrSubBlock.GetBuffer(), reinterpret_cast<LPVOID*>(&l_pvResult), &l_uiSize))
			{
				CString l_cstrvResult(reinterpret_cast<TCHAR *>(l_pvResult));//获得版本信息
				info.OriginalFilename = l_cstrvResult;
			}
			l_cstrSubBlock.Format(_T("\\StringFileInfo\\%04x%04x\\ProductName"), l_ptVersionLanguage->m_wLanguage, l_ptVersionLanguage->m_wCcodePage);
			/* 查询指定信息 */
			if (::VerQueryValue(static_cast<LPVOID>(l_ptcVersionInfo), l_cstrSubBlock.GetBuffer(), reinterpret_cast<LPVOID*>(&l_pvResult), &l_uiSize))
			{
				CString l_cstrvResult(reinterpret_cast<TCHAR *>(l_pvResult));//获得版本信息
				info.ProductName = l_cstrvResult;
			}
			l_cstrSubBlock.Format(_T("\\StringFileInfo\\%04x%04x\\ProductVersion"), l_ptVersionLanguage->m_wLanguage, l_ptVersionLanguage->m_wCcodePage);
			/* 查询指定信息 */
			if (::VerQueryValue(static_cast<LPVOID>(l_ptcVersionInfo), l_cstrSubBlock.GetBuffer(), reinterpret_cast<LPVOID*>(&l_pvResult), &l_uiSize))
			{
				CString l_cstrvResult(reinterpret_cast<TCHAR *>(l_pvResult));//获得版本信息
				info.ProductVersion = l_cstrvResult;
			}
		}
	}

	delete[] l_ptcVersionInfo;
}

void Common::GetCurrentAppVerInfo(FileVerInfo & info)
{
	CString fileName = GetCurrentAppDirectory();
	GetFileVerInfo(fileName, info);
}

CString Common::GetCurrentAppDirectory()
{
	CString szFile;
	GetModuleFileName(NULL, szFile.GetBuffer(MAX_PATH), MAX_PATH);
	return szFile;
}

CString Common::GetModifyTime(CString appname)
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFile(appname, &ffd);
	SYSTEMTIME stUTC, stLocal;
	FileTimeToSystemTime(&(ffd.ftLastWriteTime), &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	CString strTmp;
	strTmp.Format(_T("%d-%d-%d,%d:%d"), stLocal.wYear, stLocal.wMonth, stLocal.wDay, stLocal.wHour, stLocal.wMinute);
	//--	
	return strTmp;
}

void Common::SplitStr(CString strSrc, CString strGap, CStringArray &strResult)
{
	int nPos = strSrc.Find(strGap);
	CString strLeft = _T("");

	while (0 <= nPos)
	{
		strLeft = strSrc.Left(nPos);
		if (!strLeft.IsEmpty())
		{
			strResult.Add(strLeft);
		}

		strSrc = strSrc.Right(strSrc.GetLength() - nPos - strGap.GetLength());
		nPos = strSrc.Find(strGap);
	}

	if (!strSrc.IsEmpty())
	{
		strResult.Add(strSrc);
	}
}