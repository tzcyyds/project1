#pragma once


// CDlgData dialog

class CDlgData : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgData)

public:
	CDlgData(CString &_fileName, std::vector<double> &_points,
		CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgData();

private:
	CString& fileName;
	std::vector<double>& points;
	ULONG_PTR gdiplusToken;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGDATA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};
