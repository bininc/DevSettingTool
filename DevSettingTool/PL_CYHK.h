#pragma once
#include "afxwin.h"
#include "SerialCmdDlg.h"

#define STRING_LEN 1024
#define CMD_BUFFER_MAX_LEN 4096
#define CMD_CMDLEN 4
#define PACK_END '$'

#define SETTING_HEAD			"$SET$"
#define SETTING_TAIL			"$END$"
#define SETTING_CMD_SET			"SET"
#define SETTING_CMD_READ		"READ"
#define APN_ITEM_NAME			"APN"
#define SERVER_ITEM_NAME		"SERVER"
#define OEM_CODE_ITEM_NAME		"OEMCODE"
#define VEID_ITEM_NAME			"VEID"
#define VENDOR_ID_ITEM_NAME		"VENDORID"
#define SUPER_NO_ITEM_NAME		"SUPERNUMBER"

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned long   u32;

struct CYHK_INFO
{
	int PROTOCOL = -1;
	CString APN;
	CString SVIP;
	CString SVPORT;
	CString VEID;
	CString OEM_CODE;
	CString MFRS_CODE;
	CString PHONES[4];
};

// PL_CYHK 对话框

class PL_CYHK : public CDialog
{
	DECLARE_DYNAMIC(PL_CYHK)

public:
	PL_CYHK(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PL_CYHK();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CYHK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedReadv();
	afx_msg void OnBnClickedSetting();
	afx_msg	LRESULT OnReceiveData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedAntoset();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
private:
	CEdit m_dc_oem_code;
	CEdit m_dc_cur_oem_code;
	CComboBox m_dc_cur_mfrs_code;
	CComboBox m_dc_mfrs_code;
	CString GetEditText(int nIdc);
	void SetEditText(int nIdc, CString text);
	int GetCheckedRadioButton();
	void SetRadioButtonChecked(int nIdc);
	CWinThread* hThreadReadData;
	UINT_PTR m_timerid_1s = 1;
public:
	SerialCmdDlg* m_cmd_dlg;
	CYHK_INFO GetInfo();
	void SetInfo(CYHK_INFO* m_info);
	CYHK_INFO GetCurInfo();
	void SetCurInfo(CYHK_INFO* m_info);
	void Init(SerialCmdDlg* p_cmd_dlg);
	u8 m_RecvBuffer[CMD_BUFFER_MAX_LEN];
	void ReadData(char* p_data, int len);
	BOOL reading = FALSE;
	BOOL setting = FALSE;
	void PrintMsg(LPCTSTR msg);
	void PrintReadSuccessMsg();
	void DecodeCYHK_INFO(CString dataStr, CYHK_INFO* info);
	bool IsReadedSetting(bool showAlert = true);
	LPCTSTR GetProtocolStr(int protocol);
	afx_msg void OnBnClickedSavecfgfile();
	afx_msg void OnBnClickedOpencfgfile();
};
