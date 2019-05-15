
// DevSettingToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DevSettingTool.h"
#include "DevSettingToolDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "CnComm.h"
#include "Common.h"
#include <tchar.h>
#include <Dbt.h>
#include <string>
#include "afxwin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define INI_SECTION_NAME _T("SETTING")

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_lblVer;
	//	afx_msg void OnPaint();
	CStatic m_lblVer2;
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VER, m_lblVer);
	DDX_Control(pDX, IDC_STATIC_VER2, m_lblVer2);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	FileVerInfo info;
	Common::GetCurrentAppVerInfo(info);
	m_lblVer.SetWindowText(info.ProductName + " V" + info.ProductVersion);
	m_lblVer2.SetWindowText(info.FileDescription);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)

END_MESSAGE_MAP()


// CDevSettingToolDlg 对话框



CDevSettingToolDlg::CDevSettingToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DEVSETTINGTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_isCostomBaudRate = FALSE;
}

void CDevSettingToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SELCOM, m_selCom);
	DDX_Control(pDX, IDC_SELBaudRate, m_selBaudRate);
	DDX_Check(pDX, IDC_CHKBaudRate, m_isCostomBaudRate);
	DDX_Control(pDX, IDC_EDITBaudRate, m_editBaudRate);
	DDX_Control(pDX, IDC_SELParity, m_selParity);
	DDX_Control(pDX, IDC_SELByteSize, m_selByteSize);
	DDX_Control(pDX, IDC_SELStopBits, m_selStopBits);
	DDX_Control(pDX, IDC_GBMAIN, m_gbMain);
	DDX_Control(pDX, IDC_CMDSTATE, m_lb_msg);
	DDX_Control(pDX, IDC_OPENCLOSE, m_btnOpenClose);
}

BEGIN_MESSAGE_MAP(CDevSettingToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_COMMAND(IDM_EXIT, &CDevSettingToolDlg::OnExit)
	ON_COMMAND(IDM_About, &CDevSettingToolDlg::OnAbout)
	ON_WM_DEVICECHANGE(&CDevSettingToolDlg::OnDeviceChange)
	ON_CBN_SELCHANGE(IDC_SELCOM, &CDevSettingToolDlg::OnCbnSelchangeSelcom)
	ON_CBN_SELCHANGE(IDC_SELBaudRate, &CDevSettingToolDlg::OnCbnSelchangeSelbaudrate)
	ON_BN_CLICKED(IDC_CHKBaudRate, &CDevSettingToolDlg::OnBnClickedChkbaudrate)
	ON_CBN_SELCHANGE(IDC_SELParity, &CDevSettingToolDlg::OnCbnSelchangeSelparity)
	ON_BN_CLICKED(IDC_OPENCMD, &CDevSettingToolDlg::OnBnClickedOpencmd)
	ON_CBN_SELCHANGE(IDC_SELByteSize, &CDevSettingToolDlg::OnCbnSelchangeSelbytesize)
	ON_CBN_SELCHANGE(IDC_SELStopBits, &CDevSettingToolDlg::OnCbnSelchangeSelstopbits)
	ON_EN_KILLFOCUS(IDC_EDITBaudRate, &CDevSettingToolDlg::OnEnKillfocusEditbaudrate)
	ON_MESSAGE(ON_COM_OPEN, &CDevSettingToolDlg::OnComOpen)
	ON_MESSAGE(ON_COM_CLOSE, &CDevSettingToolDlg::OnComClose)
	ON_MESSAGE(ON_APPEND_MSG, &CDevSettingToolDlg::OnAppendMsg)
	ON_MESSAGE(ON_RECEIVE_DATA, &CDevSettingToolDlg::OnReceiveData)
	ON_BN_CLICKED(IDC_OPENCLOSE, &CDevSettingToolDlg::OnBnClickedOpenclose)
	ON_COMMAND(IDM_OpenCfgFile, &CDevSettingToolDlg::OnOpencfgfile)
	ON_COMMAND(IDM_SaveCfgFile, &CDevSettingToolDlg::OnSavecfgfile)
END_MESSAGE_MAP()


// CDevSettingToolDlg 消息处理程序

BOOL CDevSettingToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	FileVerInfo info;
	Common::GetCurrentAppVerInfo(info);
	SetWindowText(info.ProductName + " V" + info.ProductVersion);
	InitCtrl();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDevSettingToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDevSettingToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDevSettingToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDevSettingToolDlg::OnClose()
{
	CYHK_INFO info = m_cyhk.GetInfo();
	info.PROTOCOL = -1;	//默认不保存协议类型
	SaveCYHKCfg(&info);

	m_cyhk.SendMessage(WM_CLOSE);
	m_cmdDlg.SendMessage(WM_CLOSE);
	CDialog::OnClose();
}

void CDevSettingToolDlg::OnExit()
{
	AfxGetMainWnd()->SendMessage(WM_CLOSE); //退出程序
}


void CDevSettingToolDlg::OnAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CDevSettingToolDlg::InitCtrl()
{
	ReLoadCOM(TRUE);
	FillCmbBaudRate();
	FillCmbParity();
	FillCmbByteSize();
	FillCmbStopBits();

	CRect rect;
	m_gbMain.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.top = rect.top + 8;
	rect.left = rect.left + 15;
	rect.right = rect.right - 18;
	rect.bottom = rect.bottom - 10;
	m_cyhk.Create(IDD_CYHK, this);
	m_cmdDlg.Create(IDD_SerialCmdDlg, this);
	m_cyhk.Init(&m_cmdDlg);
	m_cyhk.MoveWindow(&rect);
	m_cyhk.ShowWindow(SW_SHOW);

	LoadCfg();
}


BOOL CDevSettingToolDlg::OnDeviceChange(UINT nEventType, DWORD dwData)
{
	//DEV_BROADCAST_DEVICEINTERFACE* dbd = (DEV_BROADCAST_DEVICEINTERFACE*) dwData;  
	switch (nEventType)
	{
	case DBT_DEVICEREMOVECOMPLETE://移除设备
		ReLoadCOM(false);
		break;
	case DBT_DEVICEARRIVAL://添加设备
		ReLoadCOM(false);
		break;
	default:
		break;
	}
	return TRUE;
}

void CDevSettingToolDlg::LoadCfg(LPCTSTR cfgFileName)
{
	CIniFile v_IniFile;
	v_IniFile.SetFileName(cfgFileName);
	if (cfgFileName != INI_DEFAULTFILENAME) {
		if (!v_IniFile.IsIniFileExist()) return;
	}
	else
		v_IniFile.EnsureIniFileExist();

	UINT uiVal = v_IniFile.IniFile_GetInt(INI_SECTION_NAME, _T("COM"), 0);
	SetCComboBoxItemData(m_selCom, uiVal);
	if (m_selCom.GetCurSel() != -1)
		m_cmdDlg.SetComPort(uiVal);

	uiVal = v_IniFile.IniFile_GetInt(INI_SECTION_NAME, _T("BAUDRATE"), 0);
	SetCComboBoxItemData(m_selBaudRate, uiVal);
	if (m_selBaudRate.GetCurSel() == -1)
		SetCComboBoxItemData(m_selBaudRate, m_cmdDlg.GetBaudRate());

	CString strVal;
	uiVal = v_IniFile.IniFile_GetString(INI_SECTION_NAME, _T("CUSTOM_BAUDRATE"), _T(""), strVal);
	m_editBaudRate.SetWindowText(strVal);

	uiVal = v_IniFile.IniFile_GetInt(INI_SECTION_NAME, _T("IS_CUSTOM_BAUDRATE"), 0);
	m_isCostomBaudRate = uiVal;
	UpdateData(FALSE);
	if (m_isCostomBaudRate)
	{
		m_editBaudRate.SetReadOnly(false);
		m_selBaudRate.EnableWindow(false);
		OnEnKillfocusEditbaudrate();
	}
	else
	{
		m_editBaudRate.SetReadOnly(true);
		m_selBaudRate.EnableWindow(true);
		OnCbnSelchangeSelbaudrate();
	}

	uiVal = v_IniFile.IniFile_GetInt(INI_SECTION_NAME, _T("PARITY"), -1);
	SetCComboBoxItemData(m_selParity, uiVal);
	if (m_selParity.GetCurSel() == -1)
	{
		SetCComboBoxItemData(m_selParity, m_cmdDlg.GetParity());
		OnCbnSelchangeSelparity();
	}
	else
		m_cmdDlg.SetParity(uiVal);

	uiVal = v_IniFile.IniFile_GetInt(INI_SECTION_NAME, _T("BYTESIZE"), 0);
	SetCComboBoxItemData(m_selByteSize, uiVal);
	if (m_selByteSize.GetCurSel() == -1)
	{
		SetCComboBoxItemData(m_selByteSize, m_cmdDlg.GetByteSize());
		OnCbnSelchangeSelbytesize();
	}
	else
		m_cmdDlg.SetByteSize(uiVal);

	uiVal = v_IniFile.IniFile_GetInt(INI_SECTION_NAME, _T("STOPBITS"), -1);
	SetCComboBoxItemData(m_selStopBits, uiVal);
	if (m_selStopBits.GetCurSel() == -1)
	{
		SetCComboBoxItemData(m_selStopBits, m_cmdDlg.GetStopBits());
		OnCbnSelchangeSelstopbits();
	}
	else
		m_cmdDlg.SetStopBits(uiVal);

	CYHK_INFO info;
	LoadCYHKCfg(&info, cfgFileName);
	m_cyhk.SetInfo(&info);
}

void CDevSettingToolDlg::SaveIntCfg(LPCTSTR lpKeyName, int nvalue)
{
	CIniFile v_IniFile;
	v_IniFile.SetFileName(INI_DEFAULTFILENAME);
	v_IniFile.EnsureIniFileExist();
	v_IniFile.IniFile_WriteInt(INI_SECTION_NAME, lpKeyName, nvalue);
}

void CDevSettingToolDlg::LoadCYHKCfg(CYHK_INFO* info, LPCTSTR cfgFileName)
{
	CIniFile v_IniFile;
	v_IniFile.SetFileName(cfgFileName);
	if (!v_IniFile.IsIniFileExist()) return;

	info->PROTOCOL = v_IniFile.IniFile_GetInt(INI_SECTION_NAME, _T("PROTOCOL"), 0);
	v_IniFile.IniFile_GetString(INI_SECTION_NAME, _T("APN"), _T(""), info->APN);
	v_IniFile.IniFile_GetString(INI_SECTION_NAME, _T("SVIP"), _T(""), info->SVIP);
	v_IniFile.IniFile_GetString(INI_SECTION_NAME, _T("SVPORT"), _T(""), info->SVPORT);
	v_IniFile.IniFile_GetString(INI_SECTION_NAME, _T("VEID"), _T(""), info->VEID);
	v_IniFile.IniFile_GetString(INI_SECTION_NAME, _T("OEM_CODE"), _T(""), info->OEM_CODE);
	v_IniFile.IniFile_GetString(INI_SECTION_NAME, _T("MFRS_CODE"), _T(""), info->MFRS_CODE);
	v_IniFile.IniFile_GetString(INI_SECTION_NAME, _T("PHONE1"), _T(""), info->PHONES[0]);
	v_IniFile.IniFile_GetString(INI_SECTION_NAME, _T("PHONE2"), _T(""), info->PHONES[1]);
	v_IniFile.IniFile_GetString(INI_SECTION_NAME, _T("PHONE3"), _T(""), info->PHONES[2]);
	v_IniFile.IniFile_GetString(INI_SECTION_NAME, _T("PHONE4"), _T(""), info->PHONES[3]);
}

void CDevSettingToolDlg::SaveCYHKCfg(CYHK_INFO* info, LPCTSTR lpFileName)
{
	CIniFile v_IniFile;
	v_IniFile.SetFileName(lpFileName);
	v_IniFile.EnsureIniFileExist();
	v_IniFile.IniFile_WriteInt(INI_SECTION_NAME, _T("PROTOCOL"), info->PROTOCOL);
	v_IniFile.IniFile_WriteString(INI_SECTION_NAME, _T("APN"), info->APN);
	v_IniFile.IniFile_WriteString(INI_SECTION_NAME, _T("SVIP"), info->SVIP);
	v_IniFile.IniFile_WriteString(INI_SECTION_NAME, _T("SVPORT"), info->SVPORT);
	v_IniFile.IniFile_WriteString(INI_SECTION_NAME, _T("VEID"), info->VEID);
	v_IniFile.IniFile_WriteString(INI_SECTION_NAME, _T("OEM_CODE"), info->OEM_CODE);
	v_IniFile.IniFile_WriteString(INI_SECTION_NAME, _T("MFRS_CODE"), info->MFRS_CODE);
	v_IniFile.IniFile_WriteString(INI_SECTION_NAME, _T("PHONE1"), info->PHONES[0]);
	v_IniFile.IniFile_WriteString(INI_SECTION_NAME, _T("PHONE2"), info->PHONES[1]);
	v_IniFile.IniFile_WriteString(INI_SECTION_NAME, _T("PHONE3"), info->PHONES[2]);
	v_IniFile.IniFile_WriteString(INI_SECTION_NAME, _T("PHONE4"), info->PHONES[3]);
}

void CDevSettingToolDlg::SaveCfg(CYHK_INFO* info, LPCTSTR lpFileName)
{
	CIniFile v_IniFile;
	v_IniFile.SetFileName(lpFileName);
	v_IniFile.EnsureIniFileExist();

	UINT uiVal = v_IniFile.IniFile_WriteInt(INI_SECTION_NAME, _T("COM"), m_cmdDlg.GetComPort());
	if (!m_isCostomBaudRate) {
		uiVal = v_IniFile.IniFile_WriteInt(INI_SECTION_NAME, _T("BAUDRATE"), m_cmdDlg.GetBaudRate());
	}
	else
	{
		uiVal = v_IniFile.IniFile_WriteInt(INI_SECTION_NAME, _T("CUSTOM_BAUDRATE"), m_cmdDlg.GetBaudRate());
	}
	uiVal = v_IniFile.IniFile_WriteInt(INI_SECTION_NAME, _T("IS_CUSTOM_BAUDRATE"), m_isCostomBaudRate);
	uiVal = v_IniFile.IniFile_WriteInt(INI_SECTION_NAME, _T("PARITY"), m_cmdDlg.GetParity());
	uiVal = v_IniFile.IniFile_WriteInt(INI_SECTION_NAME, _T("BYTESIZE"), m_cmdDlg.GetByteSize());
	uiVal = v_IniFile.IniFile_WriteInt(INI_SECTION_NAME, _T("STOPBITS"), m_cmdDlg.GetStopBits());
	if (info == NULL)
		info = &m_cyhk.GetCurInfo();
	SaveCYHKCfg(info, lpFileName);
}

void CDevSettingToolDlg::SetCComboBoxItemData(CComboBox& combo_box, DWORD_PTR data)
{
	int len = combo_box.GetCount();
	for (int i = 0; i < len; i++)
	{
		if (combo_box.GetItemData(i) == data)
		{
			combo_box.SetCurSel(i);
			break;
		}
	}
}

void CDevSettingToolDlg::AppendMsg(CString* msg)
{
	if (msg->IsEmpty()) return;
	CTime now = CTime::GetCurrentTime();
	CString timeStr = now.Format("%X");
	timeStr.AppendFormat(_T(" %s"), msg->GetString());
	m_lb_msg.AddString(timeStr);
	m_lb_msg.SetCurSel(m_lb_msg.GetCount() - 1);
}

void CDevSettingToolDlg::ReLoadCOM(bool bReset)
{
	CString vl_str;
	if (bReset)
		m_selCom.ResetContent();
	CMap<CString, LPCTSTR, DWORD, DWORD> caports;
	try {
		Common::GetComPorts(caports); //获取串口号列表
		if (!caports.IsEmpty())
		{
			/*_ttoi(comPort)				CString comPort = comStr;
				comPort.Delete(0, 3);*/
			POSITION pos;
			CString comStr;
			DWORD comPort;
			if (bReset)
			{
				pos = caports.GetStartPosition();
				while (pos)
				{
					caports.GetNextAssoc(pos, comStr, comPort);
					const DWORD iindex = m_selCom.GetCount();
					m_selCom.InsertString(iindex, comStr);
					m_selCom.SetItemData(iindex, comPort);
				}
			}
			else
			{
				CString str1;	//移除不存在的串口
				int n;
				DWORD v1;
				for (int i = 0; i < m_selCom.GetCount(); i++)
				{
					n = m_selCom.GetLBTextLen(i);
					m_selCom.GetLBText(i, str1.GetBuffer(n));
					if (caports.Lookup(str1, v1) == 0)
						m_selCom.DeleteString(i--);
					str1.ReleaseBuffer();
				}

				pos = caports.GetStartPosition();	//添加新的串口
				while (pos)
				{
					caports.GetNextAssoc(pos, comStr, comPort);
					if (m_selCom.FindString(-1, comStr) == CB_ERR)
					{
						const DWORD iindex = m_selCom.GetCount();
						m_selCom.InsertString(iindex, comStr);
						m_selCom.SetItemData(iindex, comPort);
					}
				}
			}

			if (m_selCom.GetCurSel() == CB_ERR) {
				const int nLast = m_selCom.GetCount() - 1;
				if (nLast >= 0) {
					m_selCom.SetCurSel(nLast);
					comPort = m_selCom.GetItemData(nLast);
					if (!bReset)
						m_cmdDlg.SetComPort(comPort);
					SaveIntCfg(_T("COM"), comPort);
				}
			}
		}
	}
	catch (char* errMsg)
	{
		if (m_cmdDlg.IsOpen())
			m_cmdDlg.CloseSerial();
		m_selCom.ResetContent();
		int num = MultiByteToWideChar(0, 0, errMsg, -1, nullptr, 0);
		wchar_t *wide = new wchar_t[num];
		MultiByteToWideChar(0, 0, errMsg, -1, wide, num);
		MessageBox(wide, _T("温馨提示"), MB_TOPMOST | MB_OK | MB_ICONWARNING);
	}

}


void CDevSettingToolDlg::OnCbnSelchangeSelcom()
{
	DWORD port = m_selCom.GetCurSel();
	if (port != -1) {
		port = m_selCom.GetItemData(port);
		m_cmdDlg.SetComPort(port);
		SaveIntCfg(_T("COM"), port);
	}
}

void CDevSettingToolDlg::FillCmbBaudRate()
{
	const int BAUDRATERANGE[]{ CBR_110,CBR_300,CBR_600,CBR_1200,CBR_2400,CBR_4800,CBR_9600,CBR_14400,CBR_19200,CBR_38400,CBR_56000,CBR_57600,CBR_115200,CBR_128000,CBR_256000 };
	int len = getArrayLen(BAUDRATERANGE);
	for (int i = 0; i < len; i++)
	{
		CString str;
		str.Format(_T("%d"), BAUDRATERANGE[i]);
		m_selBaudRate.InsertString(i, str);
		m_selBaudRate.SetItemData(i, BAUDRATERANGE[i]);
	}
}

void CDevSettingToolDlg::OnCbnSelchangeSelbaudrate()
{
	if (m_isCostomBaudRate) return;
	DWORD selVal = m_selBaudRate.GetCurSel();
	if (selVal != -1) {
		selVal = m_selBaudRate.GetItemData(selVal);
		m_cmdDlg.SetBaudRate(selVal);
		SaveIntCfg(_T("BAUDRATE"), selVal);
	}
}


void CDevSettingToolDlg::OnEnKillfocusEditbaudrate()
{
	if (!m_isCostomBaudRate) return;
	CString cBaudRateStr;
	m_editBaudRate.GetWindowText(cBaudRateStr);
	DWORD cBaudRate = _ttoi(cBaudRateStr);
	if (cBaudRate > 0)
	{
		m_cmdDlg.SetBaudRate(cBaudRate);
		SaveIntCfg(_T("CUSTOM_BAUDRATE"), cBaudRate);
	}
}

void CDevSettingToolDlg::OnBnClickedChkbaudrate()
{
	UpdateData();

	if (m_isCostomBaudRate)
	{
		m_editBaudRate.SetReadOnly(false);
		m_selBaudRate.EnableWindow(false);
		OnEnKillfocusEditbaudrate();
	}
	else
	{
		m_editBaudRate.SetReadOnly(true);
		m_selBaudRate.EnableWindow(true);
		OnCbnSelchangeSelbaudrate();
	}
	SaveIntCfg(_T("IS_CUSTOM_BAUDRATE"), m_isCostomBaudRate);
}


void CDevSettingToolDlg::FillCmbParity()
{
	const int parityRange[]{ NOPARITY,ODDPARITY,EVENPARITY,MARKPARITY,SPACEPARITY };
	int len = getArrayLen(parityRange);
	for (int i = 0; i < len; i++)
	{
		if (parityRange[i] == NOPARITY)
		{
			m_selParity.InsertString(i, _T("无校验(NO)"));
		}
		else if (parityRange[i] == ODDPARITY)
		{
			m_selParity.InsertString(i, _T("奇校验(ODD)"));
		}
		else if (parityRange[i] == EVENPARITY)
		{
			m_selParity.InsertString(i, _T("偶校验(EVEN)"));
		}
		else if (parityRange[i] == MARKPARITY)
		{
			m_selParity.InsertString(i, _T("1校验(MARK)"));
		}
		else if (parityRange[i] == SPACEPARITY)
		{
			m_selParity.InsertString(i, _T("0校验(SPACE)"));
		}
		m_selParity.SetItemData(i, parityRange[i]);
	}
}

void CDevSettingToolDlg::OnCbnSelchangeSelparity()
{
	DWORD selVal = m_selParity.GetCurSel();
	if (selVal != -1) {
		selVal = m_selParity.GetItemData(selVal);
		m_cmdDlg.SetParity(selVal);
		SaveIntCfg(_T("PARITY"), selVal);
	}
}

void CDevSettingToolDlg::FillCmbByteSize()
{
	for (int i = 4; i <= 8; i++)
	{
		CString lbl;
		lbl.Format(_T("%d位"), i);
		m_selByteSize.InsertString(i - 4, lbl);
		m_selByteSize.SetItemData(i - 4, i);
	}
}

void CDevSettingToolDlg::OnCbnSelchangeSelbytesize()
{
	DWORD selVal = m_selByteSize.GetCurSel();
	if (selVal != -1) {
		selVal = m_selByteSize.GetItemData(selVal);
		m_cmdDlg.SetByteSize(selVal);
		SaveIntCfg(_T("BYTESIZE"), selVal);
	}
}

void CDevSettingToolDlg::FillCmbStopBits()
{
	const int stopBitsRange[]{ ONESTOPBIT,ONE5STOPBITS,TWOSTOPBITS };
	int len = getArrayLen(stopBitsRange);
	for (int i = 0; i < len; i++)
	{
		if (stopBitsRange[i] == ONESTOPBIT)
		{
			m_selStopBits.InsertString(i, _T("1位"));
		}
		else if (stopBitsRange[i] == ONE5STOPBITS)
		{
			m_selStopBits.InsertString(i, _T("1.5位"));
		}
		else if (stopBitsRange[i] == TWOSTOPBITS)
		{
			m_selStopBits.InsertString(i, _T("2位"));
		}

		m_selStopBits.SetItemData(i, stopBitsRange[i]);
	}
}

void CDevSettingToolDlg::OnCbnSelchangeSelstopbits()
{
	DWORD selVal = m_selStopBits.GetCurSel();
	if (selVal != -1) {
		selVal = m_selStopBits.GetItemData(selVal);
		m_cmdDlg.SetStopBits(selVal);
		SaveIntCfg(_T("STOPBITS"), selVal);
	}
}

LRESULT CDevSettingToolDlg::OnComOpen(WPARAM wParam, LPARAM lParam)
{
	CString msg = _T("串口");
	if (wParam == -1)
	{
		msg.Append(_T("打开失败"));
	}
	else
	{
		m_btnOpenClose.SetWindowText(_T("关闭串口"));
		msg.AppendFormat(_T("COM%d打开成功"), wParam);
	}
	AppendMsg(&msg);
	return 0;
}

LRESULT CDevSettingToolDlg::OnComClose(WPARAM wParam, LPARAM lParam)
{
	m_btnOpenClose.SetWindowText(_T("打开串口"));
	AppendMsg(&CString("串口关闭"));
	return 0;
}

LRESULT CDevSettingToolDlg::OnAppendMsg(WPARAM wParam, LPARAM lParam)
{
	CString* msg = (CString*)lParam;
	AppendMsg(msg);
	return 0;
}

LRESULT CDevSettingToolDlg::OnReceiveData(WPARAM wParam, LPARAM lParam)
{
	m_cyhk.SendMessage(ON_RECEIVE_DATA, wParam, lParam);
	return 0;
}

void CDevSettingToolDlg::OnBnClickedOpenclose()
{
	if (!m_cmdDlg.IsOpen())
	{
		m_cmdDlg.OpenSerial();
	}
	else
	{
		m_cmdDlg.CloseSerial();
	}
}


void CDevSettingToolDlg::OnBnClickedOpencmd()
{
	if (!m_cmdDlg.IsWindowVisible())
		m_cmdDlg.ShowWindow(SW_SHOW);
	if (m_cmdDlg.IsIconic())
		m_cmdDlg.ShowWindow(SW_SHOWNORMAL);

	m_cmdDlg.OpenSerial();
}

CDevSettingToolDlg::~CDevSettingToolDlg()
{

}


void CDevSettingToolDlg::OnOpencfgfile()
{
	m_cyhk.OnBnClickedOpencfgfile();
}


void CDevSettingToolDlg::OnSavecfgfile()
{
	m_cyhk.OnBnClickedSavecfgfile();
}
