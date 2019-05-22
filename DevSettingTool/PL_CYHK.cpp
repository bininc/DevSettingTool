// PL_CYHK.cpp : 实现文件
//

#include "stdafx.h"
#include "DevSettingTool.h"
#include "PL_CYHK.h"
#include "afxdialogex.h"
#include "Common.h"
#include "DevSettingToolDlg.h"


int PROTOCOL_RANGE[] = { IDC_RADIOCYHK,IDC_RADIOCMCC,IDC_RADIOTIANHE,IDC_RADIOYOUWEI,IDC_RADIBUBIAO };
bool ValitadeProtocol(int dwProtocol);

// PL_CYHK 对话框

IMPLEMENT_DYNAMIC(PL_CYHK, CDialog)

PL_CYHK::PL_CYHK(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CYHK, pParent)
{
	m_cmd_dlg = NULL;
	hThreadReadData = INVALID_HANDLE_VALUE;
}

PL_CYHK::~PL_CYHK()
{

}

void PL_CYHK::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OEM_CODE, m_dc_oem_code);
	DDX_Control(pDX, IDC_CUR_OEM_CODE, m_dc_cur_oem_code);
	DDX_Control(pDX, IDC_CUR_MFRS_CODE, m_dc_cur_mfrs_code);
	DDX_Control(pDX, IDC_MFRS_CODE, m_dc_mfrs_code);
}

CYHK_INFO PL_CYHK::GetInfo()
{
	CYHK_INFO info;
	info.PROTOCOL = GetCheckedRadioButton();
	info.APN = (GetEditText(IDC_APN));
	info.SVIP = GetEditText(IDC_SVIP);
	info.SVPORT = GetEditText(IDC_SVPORT);
	info.VEID = GetEditText(IDC_VEID);
	info.OEM_CODE = GetEditText(IDC_OEM_CODE);
	info.PHONES[0] = GetEditText(IDC_PHONE1);
	info.PHONES[1] = GetEditText(IDC_PHONE2);
	info.PHONES[2] = GetEditText(IDC_PHONE3);
	info.PHONES[3] = GetEditText(IDC_PHONE4);
	if (info.PROTOCOL == IDC_RADIOTIANHE)
	{
		int sindex = m_dc_mfrs_code.GetCurSel();
		if (sindex >= 0)
		{
			m_dc_mfrs_code.GetLBText(sindex, info.MFRS_CODE);
		}
	}
	return info;
}

void PL_CYHK::SetInfo(CYHK_INFO* m_info)
{
	SetRadioButtonChecked(m_info->PROTOCOL);
	m_dc_oem_code.EnableWindow(m_info->PROTOCOL == IDC_RADIOCMCC);
	m_dc_cur_oem_code.EnableWindow(m_info->PROTOCOL == IDC_RADIOCMCC);
	m_dc_mfrs_code.EnableWindow(m_info->PROTOCOL == IDC_RADIOTIANHE);
	SetEditText(IDC_APN, m_info->APN);
	SetEditText(IDC_SVIP, m_info->SVIP);
	SetEditText(IDC_SVPORT, m_info->SVPORT);
	SetEditText(IDC_VEID, m_info->VEID);
	SetEditText(IDC_OEM_CODE, m_info->OEM_CODE);
	SetEditText(IDC_PHONE1, m_info->PHONES[0]);
	SetEditText(IDC_PHONE2, m_info->PHONES[1]);
	SetEditText(IDC_PHONE3, m_info->PHONES[2]);
	SetEditText(IDC_PHONE4, m_info->PHONES[3]);
	if (m_info->PROTOCOL == IDC_RADIOTIANHE)
	{
		int sindex = m_dc_mfrs_code.FindString(-1, m_info->MFRS_CODE);
		if (sindex != CB_ERR)
		{
			m_dc_mfrs_code.SetCurSel(sindex);
		}
	}
}

CYHK_INFO PL_CYHK::GetCurInfo()
{
	CYHK_INFO info;
	info.PROTOCOL = GetCheckedRadioButton();
	info.APN = (GetEditText(IDC_CUR_APN));
	info.SVIP = GetEditText(IDC_CUR_SVIP);
	info.SVPORT = GetEditText(IDC_CUR_SVPORT);
	info.VEID = GetEditText(IDC_CUR_VEID);
	info.OEM_CODE = GetEditText(IDC_CUR_OEM_CODE);
	info.PHONES[0] = GetEditText(IDC_CUR_PHONE1);
	info.PHONES[1] = GetEditText(IDC_CUR_PHONE2);
	info.PHONES[2] = GetEditText(IDC_CUR_PHONE3);
	info.PHONES[3] = GetEditText(IDC_CUR_PHONE4);
	if (info.PROTOCOL == IDC_RADIOTIANHE)
	{
		int sindex = m_dc_cur_mfrs_code.GetCurSel();
		if (sindex >= 0)
		{
			m_dc_cur_mfrs_code.GetLBText(sindex, info.MFRS_CODE);
		}
	}
	return info;
}

void PL_CYHK::SetCurInfo(CYHK_INFO * m_info)
{
	SetRadioButtonChecked(m_info->PROTOCOL);
	m_dc_oem_code.EnableWindow(m_info->PROTOCOL == IDC_RADIOCMCC);
	m_dc_cur_oem_code.EnableWindow(m_info->PROTOCOL == IDC_RADIOCMCC);
	m_dc_mfrs_code.EnableWindow(m_info->PROTOCOL == IDC_RADIOTIANHE);
	SetEditText(IDC_CUR_APN, m_info->APN);
	SetEditText(IDC_CUR_SVIP, m_info->SVIP);
	SetEditText(IDC_CUR_SVPORT, m_info->SVPORT);
	SetEditText(IDC_CUR_VEID, m_info->VEID);
	SetEditText(IDC_CUR_OEM_CODE, m_info->OEM_CODE);
	SetEditText(IDC_CUR_PHONE1, m_info->PHONES[0]);
	SetEditText(IDC_CUR_PHONE2, m_info->PHONES[1]);
	SetEditText(IDC_CUR_PHONE3, m_info->PHONES[2]);
	SetEditText(IDC_CUR_PHONE4, m_info->PHONES[3]);
	if (m_info->PROTOCOL == IDC_RADIOTIANHE)
	{
		int sindex = m_dc_cur_mfrs_code.FindString(-1, m_info->MFRS_CODE);
		if (sindex != CB_ERR)
		{
			m_dc_cur_mfrs_code.SetCurSel(sindex);
		}
	}
}

void PL_CYHK::Init(SerialCmdDlg * p_cmd_dlg)
{
	m_cmd_dlg = p_cmd_dlg;
}


BEGIN_MESSAGE_MAP(PL_CYHK, CDialog)
	ON_BN_CLICKED(IDC_READV, &PL_CYHK::OnBnClickedReadv)
	ON_BN_CLICKED(IDC_SETTING, &PL_CYHK::OnBnClickedSetting)
	ON_MESSAGE(ON_RECEIVE_DATA, &PL_CYHK::OnReceiveData)
	ON_BN_CLICKED(IDC_ANTOSET, &PL_CYHK::OnBnClickedAntoset)
	ON_BN_CLICKED(IDC_SAVECFGFILE, &PL_CYHK::OnBnClickedSavecfgfile)
	ON_BN_CLICKED(IDC_OPENCFGFILE, &PL_CYHK::OnBnClickedOpencfgfile)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_ANTOSETMAC, &PL_CYHK::OnBnClickedAntosetmac)
END_MESSAGE_MAP()


// PL_CYHK 消息处理程序


int PL_CYHK::GetCheckedRadioButton()
{
	int nIdc = 0, state = 0;
	int len = getArrayLen(PROTOCOL_RANGE);
	for (int i = 0; i < len; i++)
	{
		state = ((CButton*)GetDlgItem(PROTOCOL_RANGE[i]))->GetCheck();
		if (state == 1)
		{
			nIdc = PROTOCOL_RANGE[i];
			break;
		}
	}
	return nIdc;
}

void PL_CYHK::SetRadioButtonChecked(int nIdc)
{
	int len = getArrayLen(PROTOCOL_RANGE);
	for (int i = 0; i < len; i++)
	{
		CButton* chk = (CButton*)GetDlgItem(PROTOCOL_RANGE[i]);
		if (PROTOCOL_RANGE[i] == nIdc)
			chk->SetCheck(1);
		else
			chk->SetCheck(0);
	}

}

CString PL_CYHK::GetEditText(int nIdc)
{
	CString text;
	CEdit* edit = (CEdit*)GetDlgItem(nIdc);
	if (edit != nullptr)
	{
		edit->GetWindowText(text);
	}
	return  text;
}

void PL_CYHK::SetEditText(int nIdc, CString text)
{
	CEdit* edit = (CEdit*)GetDlgItem(nIdc);
	if (edit != nullptr)
	{
		edit->SetWindowText(text);
	}
}

BOOL PL_CYHK::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_dc_mfrs_code.AddString(_T("TH"));
	m_dc_mfrs_code.AddString(_T("HQ"));
	m_dc_cur_mfrs_code.AddString(_T("TH"));
	m_dc_cur_mfrs_code.AddString(_T("HQ"));

	SetTimer(m_timerid_1s, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

bool ValitadeProtocol(int dwProtocol)
{
	bool result = false;
	int len = getArrayLen(PROTOCOL_RANGE);
	for (int i = 0; i < len; ++i)
	{
		if (dwProtocol == PROTOCOL_RANGE[i])
		{
			result = true;
			break;
		}
	}
	return result;
}

void PL_CYHK::OnBnClickedReadv()
{
	if (m_cmd_dlg == NULL)
	{
		MessageBox(_T("错误，未初始化串口对象！"), _T("提示"), MB_TOPMOST | MB_ICONSTOP | MB_OK);
		return;
	}

	if (m_cmd_dlg->OpenSerial())
	{
		CString readCmd;
		readCmd.Format(_T("%s;%s;%s"), _T(SETTING_HEAD), _T(SETTING_CMD_READ), _T(SETTING_TAIL));
		m_cmd_dlg->WriteMsg(readCmd);
		reading = TRUE;
		PrintMsg(_T("读取参数..."));
	}
	else
	{
		MessageBox(_T("串口打开失败！"), _T("温馨提示"), MB_TOPMOST | MB_ICONSTOP | MB_OK);
	}
}


void PL_CYHK::OnBnClickedSetting()
{

	if (!IsReadedSetting())
	{
		return;
	}

	if (m_cmd_dlg == NULL)
	{
		MessageBox(_T("错误，未初始化串口对象！"), _T("提示"), MB_TOPMOST | MB_ICONSTOP | MB_OK);
		return;
	}

	if (!m_cmd_dlg->OpenSerial())
	{
		MessageBox(_T("串口打开失败！"), _T("温馨提示"), MB_TOPMOST | MB_ICONSTOP | MB_OK);
		return;
	}

	CYHK_INFO info = GetInfo();

	if (info.APN.IsEmpty())
	{
		MessageBox(_T("接入点APN不能为空！"), _T("温馨提示"), MB_TOPMOST | MB_ICONEXCLAMATION | MB_OK);
		GetDlgItem(IDC_APN)->SetFocus();
		return;
	}
	if (info.SVIP.IsEmpty())
	{
		MessageBox(_T("服务器地址不能为空！"), _T("温馨提示"), MB_TOPMOST | MB_ICONEXCLAMATION | MB_OK);
		GetDlgItem(IDC_SVIP)->SetFocus();
		return;
	}
	if (info.SVPORT.IsEmpty())
	{
		MessageBox(_T("服务器端口不能为空！"), _T("温馨提示"), MB_TOPMOST | MB_ICONEXCLAMATION | MB_OK);
		GetDlgItem(IDC_SVPORT)->SetFocus();
		return;
	}
	if (info.VEID.IsEmpty())
	{
		MessageBox(_T("终端识别码不能为空！"), _T("温馨提示"), MB_TOPMOST | MB_ICONEXCLAMATION | MB_OK);
		GetDlgItem(IDC_VEID)->SetFocus();
		return;
	}

	if (info.PROTOCOL == IDC_RADIOCMCC && info.OEM_CODE.IsEmpty())
	{
		MessageBox(_T("OEM CODE不能为空！"), _T("温馨提示"), MB_TOPMOST | MB_ICONEXCLAMATION | MB_OK);
		GetDlgItem(IDC_OEM_CODE)->SetFocus();
		return;
	}

	if (info.PROTOCOL == IDC_RADIOTIANHE && info.MFRS_CODE.IsEmpty())
	{
		MessageBox(_T("制造商代码不能为空！"), _T("温馨提示"), MB_TOPMOST | MB_ICONEXCLAMATION | MB_OK);
		GetDlgItem(IDC_MFRS_CODE)->SetFocus();
		return;
	}

	CString protolStr = GetProtocolStr(info.PROTOCOL);

	CString setCmd;
	setCmd.Format(_T("%s;%s;%s;APN:%s;SERVER:%s,%s;"), _T(SETTING_HEAD), _T(SETTING_CMD_SET), protolStr, info.APN, info.SVIP, info.SVPORT);
	if (info.PROTOCOL == IDC_RADIOCMCC)
		setCmd.AppendFormat(_T("OEMCODE:%s;"), info.OEM_CODE);
	else if (info.PROTOCOL == IDC_RADIOTIANHE)
		setCmd.AppendFormat(_T("VENDORID:%s;"), info.MFRS_CODE);
	setCmd.AppendFormat(_T("VEID:%s;SUPERNUMBER:%s,%s,%s,%s;%s"), info.VEID, info.PHONES[0], info.PHONES[1], info.PHONES[2], info.PHONES[3], _T(SETTING_TAIL));
	m_cmd_dlg->WriteMsg(setCmd);
	setting = true;
	PrintMsg(_T("设置参数..."));
}

bool PL_CYHK::IsReadedSetting(bool showAlert)
{
	int protocol = GetCheckedRadioButton();
	if (!ValitadeProtocol(protocol))
	{
		if (showAlert)
			MessageBox(_T("未识别协议类型，请先读取参数！"), _T("温馨提示"), MB_ICONSTOP | MB_OK | MB_TOPMOST);
		return false;
	}
	return true;
}

DWORD WINAPI ReadDataFun(LPVOID lpParamter)
{
	PL_CYHK* cyhk = (PL_CYHK*)lpParamter;
	int len = 0;
	do
	{
		char buffer[STRING_LEN];
		len = cyhk->m_cmd_dlg->ReadData(buffer, STRING_LEN);
		if (len > 0 && (cyhk->reading || cyhk->setting))
			cyhk->ReadData(buffer, len);
	} while (len > 0);

	return 0;
}

LRESULT PL_CYHK::OnReceiveData(WPARAM wParam, LPARAM lParam)
{
	if (hThreadReadData == INVALID_HANDLE_VALUE)
		hThreadReadData = CreateThread(NULL, 0, ReadDataFun, this, NULL, NULL);
	DWORD code = 0;
	GetExitCodeThread(hThreadReadData, &code);
	if (code != STILL_ACTIVE)
		hThreadReadData = CreateThread(NULL, 0, ReadDataFun, this, NULL, NULL);

	return 0;
}

LPCTSTR PL_CYHK::GetProtocolStr(int protocol)
{
	switch (protocol)
	{
	case IDC_RADIOCYHK:
		return _T("STRONG");
	case IDC_RADIOCMCC:
		return _T("CMCC");
	case IDC_RADIBUBIAO:
		return  _T("GUOBIAO");
	case IDC_RADIOTIANHE:
		return _T("TIANHE");
	case IDC_RADIOYOUWEI:
		return _T("YOUWEI");
	default:
		return _T("");
	}
}

static u16 CmdLength = 0;
void PL_CYHK::ReadData(char *p_data, int len)
{
	enum
	{
		Pack_Head,
		Pack_CMD,
		Pack_Tail,
	};

	int  i = 0;
	static u8 nUnPackStatus = Pack_Head;

	for (i = 0; i < len; i++, CmdLength++)
	{
		switch (nUnPackStatus)
		{
		case Pack_Head:
			CmdLength = 0;
			if (p_data[i] == '$')
			{
				memset(m_RecvBuffer, 0x0, CMD_BUFFER_MAX_LEN);
				m_RecvBuffer[CmdLength] = p_data[i];
				nUnPackStatus = Pack_CMD;
			}
			break;

		case Pack_CMD:
			m_RecvBuffer[CmdLength] = p_data[i];
			if (CmdLength == CMD_CMDLEN)
			{
				if (strstr((char*)m_RecvBuffer, "$SET$") != NULL)
				{
					nUnPackStatus = Pack_Tail;
				}
				else
				{
					nUnPackStatus = Pack_Head;
				}
			}

			break;

		case Pack_Tail:
		{
			if (CmdLength > CMD_BUFFER_MAX_LEN - 1)
			{
				nUnPackStatus = Pack_Head;
				i = len;
				break;
			}

			m_RecvBuffer[CmdLength] = p_data[i];

			if ((m_RecvBuffer[CmdLength] == PACK_END)
				&& (p_data[i + 1] == 'E')
				&& (p_data[i + 2] == 'N')
				&& (p_data[i + 3] == 'D')
				&& (p_data[i + 4] == PACK_END))
			{
				char *pResp;

				strcat((char *)m_RecvBuffer, "END$");
				CmdLength += strlen("$END$");
				m_RecvBuffer[CmdLength] = 0x0;

				if ((pResp = strchr((char *)m_RecvBuffer, ';')) == NULL)
				{
					nUnPackStatus = Pack_Head;
					i = len;
					break;
				}

				pResp++;
				CYHK_INFO info;
				if (strncmp(pResp, "CMCC", 4) == 0)
				{
					info.PROTOCOL = IDC_RADIOCMCC;
				}
				else if (strncmp(pResp, "TIANHE", 6) == 0)
				{
					info.PROTOCOL = IDC_RADIOTIANHE;
				}
				else if (strncmp(pResp, "YOUWEI", 6) == 0)
				{
					info.PROTOCOL = IDC_RADIOYOUWEI;
				}
				else if (strncmp(pResp, "STRONG", 6) == 0)
				{
					info.PROTOCOL = IDC_RADIOCYHK;
				}
				else if (strncmp(pResp, "GUOBIAO", 7) == 0)
				{
					info.PROTOCOL = IDC_RADIBUBIAO;
				}

				if (ValitadeProtocol(info.PROTOCOL))
				{
					DecodeCYHK_INFO(CString(m_RecvBuffer), &info);
					SetCurInfo(&info);
					PrintReadSuccessMsg();
				}
				else if (strncmp(pResp, "OK", 2) == 0)
				{
					setting = false;
					PrintMsg(_T("设置成功"));
					MessageBox(_T("设置成功"), _T("参数设置"), MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
				}
				else if (strncmp(pResp, "ERROR", 5) == 0)
				{
					char *pEnd;
					pResp = strchr(pResp, ':') + 1;
					if ((pEnd = strchr(pResp, ';')) != NULL)
					{
						*pEnd = 0;
					}
					setting = false;
					CString errStr, errCode(pResp);
					errStr.Format(_T("设置错误:%s"), errCode);
					PrintMsg(errStr);
					MessageBox(errStr, _T("参数设置"), MB_ICONERROR | MB_OK | MB_TOPMOST);
				}

				nUnPackStatus = Pack_Head;
				i = len;
			}
		}
		break;

		default:
			break;
		}

	}

}

void PL_CYHK::PrintMsg(LPCTSTR msg)
{
	m_cmd_dlg->AppendMsgMainWnd(msg);
}

void PL_CYHK::PrintReadSuccessMsg()
{
	reading = FALSE;
	PrintMsg(_T("读取参数成功"));
}

void PL_CYHK::DecodeCYHK_INFO(CString dataStr, CYHK_INFO* info)
{
	if (dataStr.IsEmpty()) return;
	CStringArray strArr;
	Common::SplitStr(dataStr, _T(";"), strArr);
	int len = strArr.GetCount();
	for (int k = 0; k < len; ++k)
	{
		CString item = strArr[k];
		int i = item.Find(':');
		if (i > 0)
		{
			CString key = item.Left(i);
			CString value = item.Right(item.GetLength() - i - 1);
			if (key == APN_ITEM_NAME)
			{
				info->APN = value;
			}
			else if (key == SERVER_ITEM_NAME)
			{
				i = value.Find(',');
				if (i > 0)
				{
					info->SVIP = value.Left(i);
					info->SVPORT = value.Right(value.GetLength() - i - 1);
				}
			}
			else if (key == VEID_ITEM_NAME)
			{
				info->VEID = value;
			}
			else if (key == SUPER_NO_ITEM_NAME)
			{
				int readCount = 0;
				for (int j = 0; j < 4; j++)
				{
					i = value.Find(',', readCount);
					if (i == -1)
						i = value.GetLength();

					if (i > 0)
					{
						info->PHONES[j] = value.Mid(readCount, i - readCount);
						readCount = i + 1;
					}
					else
					{
						i = value.Find(';', readCount);
						if (i == -1)
							i = value.GetLength();

						if (i > 0)
						{
							info->PHONES[j] = value.Mid(readCount, i - readCount);
							readCount = i + 1;
						}
						else
						{
							break;
						}
					}
				}
			}
			else if (key == OEM_CODE_ITEM_NAME)
			{
				info->OEM_CODE = value;
			}
			else if (key == VENDOR_ID_ITEM_NAME)
			{
				info->MFRS_CODE = value;
			}
		}
	}
}

void PL_CYHK::OnBnClickedAntoset()
{
	if (!IsReadedSetting())
	{
		return;
	}

	CYHK_INFO info = GetCurInfo();
	SetInfo(&info);
}



void PL_CYHK::OnBnClickedSavecfgfile()
{
	if (!IsReadedSetting())
	{
		return;
	}

	CYHK_INFO info = GetCurInfo();
	if (info.APN.IsEmpty() || info.SVIP.IsEmpty() || info.SVPORT.IsEmpty())
	{
		MessageBox(_T("当前值信息不完整，请读取完整参数！"), _T("温馨提示"), MB_ICONSTOP | MB_OK | MB_TOPMOST);
		return;
	}
	TCHAR szFilter[] = _T("配置文件(*.ini)|*.ini||");
	// 构造保存文件对话框   
	CFileDialog fileDlg(FALSE, _T("ini"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString strFilePath;

	// 显示保存文件对话框   
	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“保存”按钮，则将选择的文件路径显示到编辑框里   
		strFilePath = fileDlg.GetPathName();
		CDevSettingToolDlg* dlg = (CDevSettingToolDlg*)AfxGetApp()->m_pMainWnd;
		dlg->SaveCfg(&info, strFilePath);
	}
}


void PL_CYHK::OnBnClickedOpencfgfile()
{
	TCHAR szFilter[] = _T("配置文件(*.ini)|*.ini||");
	// 构造保存文件对话框   
	CFileDialog fileDlg(TRUE, _T("ini"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString strFilePath;

	// 显示保存文件对话框   
	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“保存”按钮，则将选择的文件路径显示到编辑框里   
		strFilePath = fileDlg.GetPathName();
		CDevSettingToolDlg* dlg = (CDevSettingToolDlg*)AfxGetApp()->m_pMainWnd;
		dlg->LoadCfg(strFilePath);
	}
}


void PL_CYHK::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_timerid_1s)
	{
		if (reading)
		{
			if (reading > 10)
			{	//超时
				reading = FALSE;
				PrintMsg(_T("读取参数超时"));
			}
			else
			{
				reading = reading + 1;
			}
		}
		if (setting)
		{
			if (setting > 10)
			{	//超时
				setting = FALSE;
				PrintMsg(_T("设置参数超时"));
			}
			else
			{
				setting = setting + 1;
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}


void PL_CYHK::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	KillTimer(m_timerid_1s);
}


void PL_CYHK::OnBnClickedAntosetmac()
{
	CString veid = GetEditText(IDC_CUR_VEID);
	if (!veid.IsEmpty())
	{
		SetEditText(IDC_VEID, veid);
	}
}
