
// DevSettingToolDlg.h : ͷ�ļ�
//

#pragma once


// CDevSettingToolDlg �Ի���
class CDevSettingToolDlg : public CDialog
{
// ����
public:
	void InitCtrl(void);
	void ReLoadCOM(bool bReset);
	CDevSettingToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEVSETTINGTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CComboBox m_selCom;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnExit();
	afx_msg void OnAbout();
};
