
// MFCApplication2Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication2.h"
#include "MFCApplication2Dlg.h"
#include "afxdialogex.h"
#include "CDlgData.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication2Dlg 对话框



CMFCApplication2Dlg::CMFCApplication2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION2_DIALOG, pParent)
	, m_times(10)
	, m_points(30)
	, m_port(9190)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, ser_ip);
	DDX_Text(pDX, IDC_EDIT3, m_times);
	DDX_Text(pDX, IDC_EDIT4, m_points);
	DDX_Text(pDX, IDC_EDIT2, m_port);
	DDX_Control(pDX, IDC_BUTTON1, m_con);
	DDX_Control(pDX, IDC_BUTTON4, m_sen);
	DDX_Control(pDX, IDC_BUTTON3, m_data);
	DDX_Control(pDX, IDC_COMBO1, m_cb1);
}

BEGIN_MESSAGE_MAP(CMFCApplication2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication2Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication2Dlg::OnBnClickedButton3)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMFCApplication2Dlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CMFCApplication2Dlg 消息处理程序

BOOL CMFCApplication2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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
	//初始化时，连接可点，发送不可点
	m_con.EnableWindow(true);
	m_sen.EnableWindow(false);

	ser_ip.SetAddress(127, 0, 0, 1);

	m_cb1.AddString(_T("TCP"));  // 添加字符串
	m_cb1.AddString(_T("UDP"));
	m_cb1.SetCurSel(0);  // 默认选择第一个 
	UpdateData(FALSE);
	zlog_info(c, "OnInitDialog:对话框初始化成功。");
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication2Dlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



m_socket mscli;

void CMFCApplication2Dlg::OnBnClickedButton1()
{
	//连接
	CString csIP;
	ser_ip.GetWindowText(csIP);
	UpdateData(TRUE);
	//这里的CString怎么转换成char数组啊，感觉很混乱。

	CT2A ascii(csIP);
	//TRACE(_T("ASCII: %S\n"), ascii.m_psz);

	//CT2A temp(m_port);
	//TRACE(_T("ASCII: %S\n"), temp.m_psz);

	u_short port = 0;
	port = (u_short)m_port;

	// 获取选择的是TCP or UDP
	int cb_index = m_cb1.GetCurSel();  // 这个函数用于得到用户选择的是下拉列表中的第几行，第一行的话，返回0
	if(cb_index == 0)
		zlog_info(c, "尝试与服务器建立连接，服务器IP地址：%s，端口号：%d，协议：TCP", ascii.m_szBuffer, port);
	else
		zlog_info(c, "尝试与服务器建立连接，服务器IP地址：%s，端口号：%d，协议：UDP", ascii.m_szBuffer, port);

	if (mscli.m_client(ascii.m_szBuffer, port, cb_index))
	{
		zlog_info(c, "与服务器建立连接成功");
		m_sen.EnableWindow(true);
		m_con.EnableWindow(false);
		MessageBox(TEXT("connetced"));
	}
	else
	{
		zlog_info(c, "与服务器建立连接失败");
		m_sen.EnableWindow(false);
		m_con.EnableWindow(true);
		MessageBox(TEXT("connect error!"));
	}
}


void CMFCApplication2Dlg::OnBnClickedButton4()
{
	// 日志文件以时间命名，获取时间
	struct tm time_info;
	__time64_t ltime;
	errno_t err_time, err_log;
	_time64(&ltime);
	err_time = _localtime64_s(&time_info, &ltime);
	//if (err_time)
	//{
		// TODO: 报错，获取时间失败
		// exit(1);
	//}
	char log_dir[32];
	int cb_index = m_cb1.GetCurSel();
	if (cb_index == 0)
		strftime(log_dir, 32, ".\\log\\%Y-%m-%d %H%M%S TCP.txt", &time_info);
	else
		strftime(log_dir, 32, ".\\log\\%Y-%m-%d %H%M%S UDP.txt", &time_info);


	// 发送
	UpdateData(TRUE);
	CString temp;
	m_sen.EnableWindow(false);
	zlog_info(c, "发送启动，测试次数：%d，测试样本点：%d",m_times,m_points);
	double* m_sum = mscli.send_ehco(m_times,m_points);
	for (int i = 0;i < m_times;i++) {
		temp.Format(L"Average: %.1f us\n", m_sum[i] * 1e6 / 30);
		MessageBox(temp);
	}
	m_data.EnableWindow(true);


	// 创建日志文件
	if (_mkdir(".\\log") != 0)
	{
		// TODO: 警告，目录已经创建了
	}
	FILE* fp = NULL;
	err_log = fopen_s(&fp, log_dir, "w");
	if (!err_log && fp)
	{
		for (int i = 0; i < m_times; i++)
		fprintf_s(fp, "%lf\n", m_sum[i] * 1e6 / 30);

		zlog_info(data, "手动分界线");
		for (int i = 0; i < m_times; i++)
		zlog_info(data, "%1f", m_sum[i] * 1e6 / 30);
	}

	//else
	//{
	//	// TODO: 报错，日志文件不能成功打开
	//	// exit(1);
	//}


	//要处理calloc出的内存地址，不然会泄露的。比如free(m_sum)
	free(m_sum);
	if (fp) err_log = fclose(fp);

	//测试完成后重新激活连接按钮
	m_con.EnableWindow(true);
	m_sen.EnableWindow(false);
}


void CMFCApplication2Dlg::OnBnClickedButton3()
{
	// 创建文件对话框
	TCHAR szFilters[] = _T("Log Files (*.txt)|*.txt||");

	CFileDialog fileDlg(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	TCHAR PWD[MAX_PATH];

	GetCurrentDirectory(MAX_PATH, PWD);

	lstrcat(PWD, _T("\\log\\"));

	fileDlg.m_ofn.lpstrInitialDir = PWD;// 设置文件对话框初始路径


	if (fileDlg.DoModal() == IDOK)
	{
		// 获取日志文件完整路径，打开日志文件
		CString pathName = fileDlg.GetPathName();
		CString fileName = fileDlg.GetFileTitle();
		CT2A ascii(fileName);
		//LPTSTR lpsz = (LPTSTR)(LPCTSTR)pathName;
		FILE* fp = NULL;
		errno_t err_log;
		//pathName = "D:\\作业\\网络软件设计\\单元1 基本通信程序设计\\添加了udp\\MFCApplication2\\MFCApplication2\\log\\data.log";
		err_log = _tfopen_s(&fp, pathName, _T("r"));
		//fp = _fsopen(ascii.m_szBuffer, "rb", _SH_DENYNO);
		//fp = fopen(ascii.m_szBuffer, "r");
		std::vector<double> points;
		
		if (err_log != 0)
			zlog_debug(c, "日志文件不能成功打开 err_log:%d",err_log);
		if (!err_log && fp)
		{
			double t;
			while (fscanf_s(fp, "%lf\n", &t) != EOF)
			{

				points.push_back(t);
				
			}
			zlog_info(c,"数据导入完毕，开始画图...");
		}
		//else
		//{
		//	// TODO: 报错，日志文件不能成功打开
		//	// exit(1);
		//}


		// 构造CDlgData，传入fileName和points
		// fileName是CString， points是vector<double>
		CDlgData m_data(fileName, points);
		m_data.DoModal();
	}
}


void CMFCApplication2Dlg::OnCbnSelchangeCombo1()
{
	int cb_index = m_cb1.GetCurSel();
	// TODO: 在此添加控件通知处理程序代码
	if (cb_index == 0) {
		m_con.SetWindowText(TEXT("连接"));
	}
	else if(cb_index == 1){
		m_con.SetWindowText(TEXT("绑定"));
	}
}
