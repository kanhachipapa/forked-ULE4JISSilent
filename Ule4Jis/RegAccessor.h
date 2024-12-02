// RegAccessor.h : ヘッダー ファイル
//

#pragma once

#include <string>
#include <Windows.h>

class RegAccessor {
	std::string _subkey;
	std::string _valueName;
	HKEY _hKey;
public:
	RegAccessor(const std::string& subkey) :
		_subkey(subkey), _hKey(NULL), _valueName("restoreValue")
	{
		if (!Open())
			Create();
	}
	~RegAccessor()
	{
		if (_hKey)
			::RegCloseKey(_hKey);
	}
	bool Write(const int code)
	{
		if (!_hKey)
			return false;

		std::string codestr = std::to_string(code);
		DWORD dwResult = ::RegSetValueExA(_hKey , _valueName.c_str(), 0,
			REG_SZ, (const BYTE*)codestr.c_str(), (DWORD)(codestr.size() * sizeof(CHAR)));
		return (dwResult == ERROR_SUCCESS) ? true : false;
	}
	bool Read(int& code)
	{
		if (!_hKey)
			return false;
		CHAR waReadBuf[MAX_PATH];
		DWORD dwReadSize = sizeof(waReadBuf);
		DWORD dwType = 0;
		DWORD dwResult = ::RegQueryValueExA(_hKey, _valueName.c_str(), 0, &dwType, (LPBYTE)waReadBuf, &dwReadSize);
		if (dwResult == ERROR_SUCCESS) {
			std::string work = waReadBuf;
			code = std::stoi(work);
			return true;
		}
		else {
			DWORD hResult = ::HRESULT_FROM_WIN32(dwResult);
			return false;
		}
	}
protected:
	bool Open()
	{
		// reg open
		DWORD dwResult = ::RegOpenKeyExA(HKEY_CURRENT_USER, _subkey.c_str(), 0, KEY_SET_VALUE| KEY_QUERY_VALUE, &_hKey);
		return (dwResult == ERROR_SUCCESS) ? true : false;
	}
	bool Create()
	{
		DWORD dwDisposition = 0;
		DWORD dwResult = ::RegCreateKeyExA(HKEY_CURRENT_USER, _subkey.c_str(), 0,
			NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_64KEY, NULL, &_hKey, &dwDisposition);
		return (dwResult == ERROR_SUCCESS) ? true : false;
	}
};
