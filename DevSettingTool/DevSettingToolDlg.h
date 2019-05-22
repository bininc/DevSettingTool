
// DevSettingToolDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "PL_CYHK.h"
#include "SerialCmdDlg.h"
#include "IniFile.h"

#define INI_DEFAULTFILENAME _T("Default")

// CDevSettingToolDlg �Ի���
class CDevSettingToolDlg : public CDialog
{
	// ����
public:
	CDevSettingToolDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CDevSettingToolDlg();
	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEVSETTINGTOOL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnClose();
	afx_msg void OnExit();
	afx_msg void OnAbout();
	afx_msg void OnCbnSelchangeSelcom();
	afx_msg void OnCbnSelchangeSelbaudrate();
	afx_msg void OnBnClickedChkbaudrate();
	afx_msg void OnCbnSelchangeSelparity();
	afx_msg void OnBnClickedOpencmd();
	afx_msg void OnEnKillfocusEditbaudrate();
	afx_msg void OnCbnSelchangeSelbytesize();
	afx_msg void OnCbnSelchangeSelstopbits();
	afx_msg void OnBnClickedOpenclose();
	afx_msg LRESULT OnComOpen(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnComClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAppendMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveData(WPARAM wParam, LPARAM lParam);
protected:
	CComboBox m_selCom;
	CComboBox m_selBaudRate;
	CEdit m_editBaudRate;
	CComboBox m_selParity;
	CComboBox m_selByteSize;
	CComboBox m_selStopBits;
	PL_CYHK m_cyhk;	
	CStatic m_gbMain;
	SerialCmdDlg m_cmdDlg;
	BOOL m_isCostomBaudRate;
	CString m_lb_msg;
	CButton m_btnOpenClose;
private:
	void InitCtrl();
	void ReLoadCOM(bool bReset);
	void FillCmbBaudRate();
	void FillCmbParity();
	void FillCmbByteSize();
	void FillCmbStopBits();

public:
	void LoadCfg(LPCTSTR cfgFileName = INI_DEFAULTFILENAME);
	void SaveIntCfg(LPCTSTR lpKeyName, int nvalue);
	void LoadCYHKCfg(CYHK_INFO* info, LPCTSTR cfgFileName = INI_DEFAULTFILENAME);
	void SaveCYHKCfg(CYHK_INFO* info, LPCTSTR lpFileName = INI_DEFAULTFILENAME);
	void SaveCfg(CYHK_INFO* info = NULL, LPCTSTR lpFileName = INI_DEFAULTFILENAME);
	void SetCComboBoxItemData(CComboBox& combo_box, DWORD_PTR data);
	void AppendMsg(CString* msg);
	afx_msg void OnOpencfgfile();
	afx_msg void OnSavecfgfile();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
