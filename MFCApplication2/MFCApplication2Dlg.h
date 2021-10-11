
// MFCApplication2Dlg.h: 头文件
//

#pragma once

// CMFCApplication2Dlg 对话框
class CMFCApplication2Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication2Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();

private:
	// 服务器IP地址
	CIPAddressCtrl ser_ip;
	int m_times;
	int m_points;
	int m_port;

public:
	afx_msg void OnBnClickedButton3();
private:
	// connect
	CButton m_con;
	// disconnect
//	CButton m_dcon;
	// send
	CButton m_sen;
	// stop
//	CButton m_stp;
	// analyze data
	CButton m_data;
public:
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox m_cb1;
};
