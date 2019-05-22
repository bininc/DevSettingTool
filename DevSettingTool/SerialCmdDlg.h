#pragma once

#include "CnComm.h"

#define WM_MY_MESSAGE (WM_USER+100)
#define	ON_COM_OPEN			WM_MY_MESSAGE + 1	
#define	ON_COM_CLOSE		WM_MY_MESSAGE + 2	
#define ON_APPEND_MSG		WM_MY_MESSAGE + 3
#define ON_RECEIVE_DATA		WM_MY_MESSAGE + 4
// SerialCmdDlg 对话框

class SerialCmdDlg : public CDialog
{
	DECLARE_DYNAMIC(SerialCmdDlg)

public:
	SerialCmdDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SerialCmdDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SerialCmdDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg	void OnCheckRxHex();
	afx_msg void OnCheckTxHex();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnsend();
	afx_msg LRESULT OnReceive(WPARAM, LPARAM);
	afx_msg LRESULT OnRLSD(WPARAM, LPARAM);
	afx_msg LRESULT OnBreak(WPARAM, LPARAM);
	afx_msg LRESULT OnRing(WPARAM, LPARAM);
	afx_msg LRESULT OnCTS(WPARAM, LPARAM);
	afx_msg LRESULT OnDSR(WPARAM, LPARAM);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCheckStopmove();
	afx_msg void OnEnKillfocusSenddata();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnBnClickedBtnclaer();
	DECLARE_MESSAGE_MAP()

protected:
	HICON m_hIcon;
	CnComm Comm_;
	CnComm::BlockBuffer Rx_;
	DWORD ComPort = NULL;
	DWORD BaudRate = 115200;
	BYTE Parity = NOPARITY;
	BYTE ByteSize = 8;
	BYTE StopBits = ONESTOPBIT;
	bool m_changed = false;
	virtual BOOL OnInitDialog();
	void AppendMsg(CString msg);
	bool openCloseing = false;	//打开操作标记
public:
	CString m_sRx;
	CString	m_sTx;
	CEdit m_editSendData;
	CEdit m_editReceiveData;
	BOOL m_bRxHex;
	BOOL m_bTxHex;
	BOOL m_bStopMove;
	bool OpenSerial();
	void CloseSerial(DWORD dwWaitTime = 500);
	bool IsOpen();
	void SetComPort(DWORD dwComPort);
	DWORD GetComPort();
	void SetBaudRate(DWORD dwBaudRate);
	DWORD GetBaudRate();
	void SetParity(BYTE bParity);
	BYTE GetParity();
	void SetByteSize(BYTE bByteSize);
	BYTE GetByteSize();
	void SetStopBits(BYTE bStopBits);
	BYTE GetStopBits();
	DWORD ReadData(LPVOID lpBuf, DWORD dwSize);
	void NotifyMainWnd(UINT uMsg, LPARAM wParam = 0, LPARAM lParam = 0);
	void AppendMsgMainWnd(CString msg);
	void WriteMsg(CString msg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnChangeReceivedata();
};

