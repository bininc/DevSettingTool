// SerialCmdDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DevSettingTool.h"
#include "SerialCmdDlg.h"
#include <string>


int Ascii2Hex(char* ascii, char* hex);
int Hex2Ascii(char* hex, char* ascii);

// SerialCmdDlg 对话框

IMPLEMENT_DYNAMIC(SerialCmdDlg, CDialog)

SerialCmdDlg::SerialCmdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SerialCmdDlg, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_sRx = _T("");
	m_sTx = _T("");
	m_bRxHex = FALSE;
	m_bTxHex = FALSE;
}

SerialCmdDlg::~SerialCmdDlg()
{
}

void SerialCmdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_RX_HEX, m_bRxHex);
	DDX_Check(pDX, IDC_CHECK_TX_HEX, m_bTxHex);
	DDX_Text(pDX, IDC_RECEIVEDATA, m_sRx);
	DDX_Control(pDX, IDC_SENDDATA, m_editSendData);
	DDX_Check(pDX, IDC_CHECK_STOPMOVE, m_bStopMove);
}

BOOL SerialCmdDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	Comm_.SetWnd(this->m_hWnd);
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	Rx_.SetMinBlockSize(4096);

	UpdateData(FALSE);

	return TRUE;
}


BEGIN_MESSAGE_MAP(SerialCmdDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_MESSAGE(ON_COM_RECEIVE, OnReceive)
	ON_MESSAGE(ON_COM_RLSD, OnRLSD)
	ON_MESSAGE(ON_COM_BREAK, OnBreak)
	ON_MESSAGE(ON_COM_RING, OnRing)
	ON_MESSAGE(ON_COM_CTS, OnCTS)
	ON_MESSAGE(ON_COM_DSR, OnDSR)
	ON_BN_CLICKED(IDC_CHECK_RX_HEX, OnCheckRxHex)
	ON_BN_CLICKED(IDC_CHECK_TX_HEX, OnCheckTxHex)
	ON_BN_CLICKED(IDC_BTNSEND, &SerialCmdDlg::OnBnClickedBtnsend)
	ON_BN_CLICKED(IDC_CHECK_STOPMOVE, &SerialCmdDlg::OnBnClickedCheckStopmove)
	ON_EN_KILLFOCUS(IDC_SENDDATA, &SerialCmdDlg::OnEnKillfocusSenddata)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// SerialCmdDlg 消息处理程序


void SerialCmdDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnClose();
}


void SerialCmdDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (nType == 1) return;//最小化则什么都不做	
}

void SerialCmdDlg::AppendMsg(CString msg)
{
	if (msg.IsEmpty()) return;
	//if (!m_bStopMove)
		//UpdateData();
	m_sRx.Append(msg);
	if (!m_bStopMove)
		UpdateData(FALSE);
	Sel2End();
}

bool SerialCmdDlg::OpenSerial()
{
	CString msg("==>");
	if (ComPort == NULL)
	{	//未定义串口 打开失败	
		msg.AppendFormat(_T("打开串口COM%d失败！\r\n"), ComPort);
		AppendMsg(msg);
		NotifyMainWnd(ON_COM_OPEN, (WPARAM)-1);
		return false;
	}

	if (Comm_.IsOpen())
	{
		if (m_changed)
			CloseSerial();
		else
			return true;
	}
	bool suc = false;
	try {
		suc = Comm_.Open(ComPort, BaudRate, Parity, ByteSize, StopBits);
		if (suc)
		{
			m_changed = false;
			msg.AppendFormat(_T("打开串口COM%d成功！...\r\n"), ComPort);
			AppendMsg(msg);
			NotifyMainWnd(ON_COM_OPEN, (WPARAM)Comm_.GetPort());
		}
	}
	catch (CString errMsg)
	{

	}

	return  suc;
}

void SerialCmdDlg::CloseSerial(DWORD dwWaitTime)
{
	if (Comm_.IsOpen()) {
		CString msg("==>");
		Comm_.Close(dwWaitTime);
		msg.AppendFormat(_T("\r\n==>关闭串口COM%d\r\n"), ComPort);
		AppendMsg(msg);
		NotifyMainWnd(ON_COM_CLOSE);
	}
}

bool SerialCmdDlg::IsOpen()
{
	return Comm_.IsOpen();
}

void SerialCmdDlg::SetComPort(DWORD dwComPort)
{
	if (dwComPort != ComPort)
	{
		CloseSerial();
		ComPort = dwComPort;
		m_changed = true;
	}
}

DWORD SerialCmdDlg::GetComPort()
{
	return ComPort;
}

void SerialCmdDlg::SetBaudRate(DWORD dwBaudRate)
{
	if (dwBaudRate != BaudRate)
	{
		CloseSerial();
		BaudRate = dwBaudRate;
		m_changed = true;
	}
}

DWORD SerialCmdDlg::GetBaudRate()
{
	return BaudRate;
}

void SerialCmdDlg::SetParity(BYTE bParity)
{
	if (bParity != Parity)
	{
		CloseSerial();
		Parity = bParity;
		m_changed = true;
	}
}

BYTE SerialCmdDlg::GetParity()
{
	return Parity;
}

void SerialCmdDlg::SetByteSize(BYTE bByteSize)
{
	if (bByteSize != ByteSize)
	{
		CloseSerial();
		ByteSize = bByteSize;
		m_changed = true;
	}
}

BYTE SerialCmdDlg::GetByteSize()
{
	return ByteSize;
}

void SerialCmdDlg::SetStopBits(BYTE bStopBits)
{
	if (bStopBits != StopBits)
	{
		CloseSerial();
		StopBits = bStopBits;
		m_changed = true;
	}
}

BYTE SerialCmdDlg::GetStopBits()
{
	return StopBits;
}

//! 串口接收消息 读取串口数据
LRESULT SerialCmdDlg::OnReceive(WPARAM wParam, LPARAM lParam)
{
	if (wParam == Comm_.GetPort())
	{
		CString bufferStr;
		do {

			char buffer[1024];
			int len = Comm_.Read(buffer, 1023);
			buffer[len] = _T('\0');
			Rx_.Write(buffer, len);

			if (m_bRxHex)
			{
				char hex_buffer[1024 * 3];
				Ascii2Hex(buffer, hex_buffer);
				bufferStr += hex_buffer;
				bufferStr.Replace(_T("0A"), _T("\r\n"));
			}
			else
			{
				bufferStr += buffer;
				bufferStr.Replace(_T("\n"), _T("\r\n"));
			}
			AppendMsg(bufferStr);
			/*
			if (m_bAutoSave)
			{
				FILE* fp;
				fp = fopen(m_RxFileName, "a+");
				fwrite(buffer, len, 1, fp);
				fclose(fp);
			}*/
			//! 接收缓冲模式下，要确保把已经在Comm_.Input()缓冲区的数据处理完
			//! 否则如果没有新的数据再来，不会继续通知你
		} while (Comm_.IsRxBufferMode() && Comm_.Input().SafeSize());
		if (!bufferStr.IsEmpty())
			NotifyMainWnd(ON_RECEIVE_DATA, wParam, lParam);
	}

	return TRUE;
}

void SerialCmdDlg::OnCheckRxHex()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void SerialCmdDlg::OnCheckTxHex()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_sTx.Empty();
	m_editSendData.SetWindowText(m_sTx);
}

LRESULT SerialCmdDlg::OnRLSD(WPARAM wParam, LPARAM)
{
	/*if (wParam  == Comm_.GetPort())
		SendDlgItemMessage(IDC_CHECK_RLSD, BM_SETCHECK, Comm_.CheckRLSD());*/
	return TRUE;
}

LRESULT SerialCmdDlg::OnBreak(WPARAM wParam, LPARAM)
{
	/*CString str;
	str.Format(_T("COM%d be placed in a break state."), (int)wParam);
	MessageBox(str);*/
	return TRUE;
}

LRESULT SerialCmdDlg::OnRing(WPARAM wParam, LPARAM)
{
	/*if (wParam  == Comm_.GetPort())
		SendDlgItemMessage(IDC_CHECK_RING, BM_SETCHECK, Comm_.CheckRING());*/
	return TRUE;
}

LRESULT SerialCmdDlg::OnCTS(WPARAM wParam, LPARAM)
{
	/*if (wParam  == Comm_.GetPort())
		SendDlgItemMessage(IDC_CHECK_CTS, BM_SETCHECK, Comm_.CheckCTS());*/

	return TRUE;
}

LRESULT SerialCmdDlg::OnDSR(WPARAM wParam, LPARAM)
{
	/*if (wParam  == Comm_.GetPort())
		SendDlgItemMessage(IDC_CHECK_DSR, BM_SETCHECK, Comm_.CheckDSR());*/
	return TRUE;
}


void SerialCmdDlg::OnDestroy()
{
	CloseSerial();
}

void SerialCmdDlg::OnBnClickedBtnsend()
{
	if (!Comm_.IsOpen()) return;

	UpdateData();

	char* buffer = new char[m_sTx.GetLength() * 2 + 1];

#ifdef UNICODE
	int len = wcstombs(buffer, (LPCTSTR)m_sTx, m_sTx.GetLength());
	buffer[len] = 0;
#else
	strcpy(buffer, (LPCTSTR)m_sTx);
	int len = m_sTx.GetLength();
#endif

	if (m_bTxHex)
	{
		int HexCnt = Hex2Ascii(buffer, buffer);

		if (Comm_.IsOverlappedMode() || Comm_.IsTxBufferMode())
			Comm_.Write(buffer, HexCnt);
		else
		{
			for (int i = 0; i < HexCnt; i++)
				i += Comm_.Write(buffer + i, HexCnt - i);
		}
	}
	else
	{	//! 重叠模式下或者缓冲区模式下 可以不检查输出返回值，并直接输出
		if (Comm_.IsOverlappedMode() || Comm_.IsTxBufferMode())
			Comm_.Write(buffer);
		else
		{//! 阻塞非缓冲区模式 必须检查返回值，确保数据完全发送出
			for (int i = 0; i < len; i++)
				i += Comm_.Write(buffer + i, len - i);
		}
	}

	delete[] buffer;
}

int Ascii2Hex(char* ascii, char* hex)
{
	int i, len = strlen(ascii);
	char chHex[] = "0123456789ABCDEF";

	for (i = 0; i < len; i++)
	{
		hex[i * 3] = chHex[((BYTE)ascii[i]) >> 4];
		hex[i * 3 + 1] = chHex[((BYTE)ascii[i]) & 0xf];
		hex[i * 3 + 2] = ' ';
	}

	hex[len * 3] = '\0';

	return len * 3;
}

int Hex2Ascii(char* hex, char* ascii)
{
	int len = strlen(hex), tlen, i, cnt;

	for (i = 0, cnt = 0, tlen = 0; i < len; i++)
	{
		char c = toupper(hex[i]);

		if ((c >= '0'&& c <= '9') || (c >= 'A'&& c <= 'F'))
		{
			BYTE t = (c >= 'A') ? c - 'A' + 10 : c - '0';

			if (cnt)
				ascii[tlen++] += t, cnt = 0;
			else
				ascii[tlen] = t << 4, cnt = 1;
		}
	}

	return tlen;
}

void SerialCmdDlg::Sel2End()
{
	if (m_bStopMove) return;
	CEdit * edit = (CEdit*)GetDlgItem(IDC_RECEIVEDATA);
	int cnt = edit->GetLineCount();
	edit->LineScroll(cnt);
	//long len = m_sRx.GetLength();
	//edit->SetSel(len, len, true);
	//edit->SetFocus();
}

DWORD SerialCmdDlg::ReadData(LPVOID lpBuf, DWORD dwSize)
{
	if (Rx_.Size() == 0) return 0;
	return Rx_.Read(lpBuf, dwSize);
}

void SerialCmdDlg::NotifyMainWnd(UINT uMsg, LPARAM wParam, LPARAM lParam)
{
	AfxGetMainWnd()->SendMessage(uMsg, wParam, lParam);
}

void SerialCmdDlg::AppendMsgMainWnd(CString msg)
{
	NotifyMainWnd(ON_APPEND_MSG, 0, (LPARAM)&msg);
}

void SerialCmdDlg::OnBnClickedCheckStopmove()
{
	int	state = ((CButton*)GetDlgItem(IDC_CHECK_STOPMOVE))->GetCheck();
	m_bStopMove = state == 1;
	if (!m_bStopMove) {
		UpdateData(FALSE);
		Sel2End();
	}
}

void SerialCmdDlg::OnEnKillfocusSenddata()
{
	m_editSendData.GetWindowText(m_sTx);
}


void SerialCmdDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 813;
	lpMMI->ptMinTrackSize.y = 573;

	CDialog::OnGetMinMaxInfo(lpMMI);
}

void SerialCmdDlg::WriteMsg(CString msg)
{
	if (msg.IsEmpty()) return;

	char* buffer = new char[msg.GetLength() * 2 + 1];

#ifdef UNICODE
	int len = wcstombs(buffer, (LPCTSTR)msg, msg.GetLength());
	buffer[len] = 0;
#else
	strcpy(buffer, (LPCTSTR)msg);
	int len = msg.GetLength();
#endif

	//! 重叠模式下或者缓冲区模式下 可以不检查输出返回值，并直接输出
	if (Comm_.IsOverlappedMode() || Comm_.IsTxBufferMode())
		Comm_.Write(buffer);
	else
	{//! 阻塞非缓冲区模式 必须检查返回值，确保数据完全发送出
		for (int i = 0; i < len; i++)
			i += Comm_.Write(buffer + i, len - i);
	}

	delete[] buffer;
}