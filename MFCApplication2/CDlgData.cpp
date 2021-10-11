// CDlgData.cpp : implementation file
//

#include "pch.h"
#include "MFCApplication2.h"
#include "CDlgData.h"
#include "afxdialogex.h"


// CDlgData dialog

IMPLEMENT_DYNAMIC(CDlgData, CDialogEx)

CDlgData::CDlgData(CString& _fileName, std::vector<double>& _points,
	CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGDATA, pParent), fileName(_fileName), points(_points)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

CDlgData::~CDlgData()
{
	Gdiplus::GdiplusShutdown(gdiplusToken);
}

void CDlgData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgData, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


int x_offset = 10;// 左右偏移
int top_offset = 40, bottom_offset = 50;// 上偏移和下偏移
int comp1 = 8, comp2 = 2;// 补偿修正

using namespace Gdiplus;

// CDlgData message handlers
void CDlgData::OnPaint()
{
	CPaintDC dc(this);
	Graphics graphics(dc.m_hDC);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);// 平滑曲线
	CRect rect;
	GetClientRect(rect);// 获取用户区矩形

	//dc.SetMapMode(MM_ANISOTROPIC);
	//dc.SetWindowExt(rect.Width(), rect.Height());
	//dc.SetViewportExt(rect.Width(), -rect.Height());
	//dc.SetViewportOrg(0, rect.Height());
	//rect.OffsetRect(0, -rect.Height());

	int n = (int)points.size();
	int m_height = rect.Height() - top_offset - bottom_offset;
	int m_width = rect.Width() - x_offset - x_offset;
	int draw_width = m_width - comp1 - comp1;

	//CRect rect_draw(x_offset, bottom_offset, x_offset + m_width, bottom_offset + m_height);
	//COLORREF color = RGB(255, 255, 255);
	//CBrush brush(color);
	//dc.FillRect(&rect_draw, &brush);
	//dc.Rectangle(rect_draw);

	graphics.SetTransform(new Matrix(1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f));// 反转Y轴（正方向朝上）
	graphics.TranslateTransform(0, -(float)rect.Height());// 设置坐标原点（左下角）
	Rect frame(x_offset, bottom_offset, m_width, m_height);
	graphics.DrawRectangle(new Pen(Color(255, 0, 0, 0), 1), frame);// 边框颜色（黑色）
	graphics.FillRectangle(new SolidBrush(Color(255, 255, 255, 255)), frame);// 画图区背景色（白色）

	//CPen pen(PS_SOLID, 2, RGB(0, 0, 255));
	//dc.SelectObject(&pen);

	Pen blue(Color(255, 0, 0, 255), 1);// 折线图画笔颜色（蓝色）
	Pen red(Color(255, 255, 0, 0), 1);// 折线图数据点颜色（红色）
	std::vector<int> pointsToDraw;
	double min = DBL_MAX, max = DBL_MIN, avg = 0, var = 0;
	for (int i = 0; i < n; i++)// 找到最小值和最大值
	{
		avg += points[i];
		var += pow(points[i], 2.0);
		if (points[i] < min) min = points[i];
		if (points[i] > max) max = points[i];
	}
	avg /= n; var = var / n - pow(avg, 2.0);
	int act_range = m_height - comp1 - comp1;
	double ratio = (double)act_range / (max - min);
	for (int i = 0; i < n; i++)// 映射到绘图框
	{
		points[i] -= min;
		points[i] *= ratio;
		pointsToDraw.push_back(lround(points[i]));
		pointsToDraw[i] += bottom_offset + comp1;
	}

	int step = draw_width / (n - 1);// x轴上的步进
	int x = x_offset + comp1 + comp2;
	int r = 3, d = 6;// 点的直径和半径
	for (int i = 0; i < n - 1; i++)// 连线，描点
	{
		graphics.DrawLine(&blue, x, pointsToDraw[i], x + step, pointsToDraw[i + 1]);
		graphics.DrawEllipse(&red, x - r, pointsToDraw[i] - r, d, d);
		graphics.FillEllipse(new SolidBrush(Color(255, 255, 0, 0)), x - r, pointsToDraw[i] - r, d, d);
		x += step;
	}
	graphics.DrawEllipse(&red, x - r, pointsToDraw[n - 1] - r, d, d);
	graphics.FillEllipse(new SolidBrush(Color(255, 255, 0, 0)), x - r, pointsToDraw[n - 1] - r, d, d);

	graphics.SetTransform(new Matrix(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f));// 回正Y轴（正方向朝下）
	fileName.Insert(13, _T(':')); fileName.Insert(16, _T(':'));
	fileName += _T(" Capture");
	Gdiplus::Font font(_T("Arial"), 10);// 字体和大小
	SolidBrush text(Color(255, 0, 0, 0));// 文字颜色
	graphics.DrawString(fileName, fileName.GetLength(), &font, PointF(10.0f, 10.0f), &text);
	WCHAR stat[128];

	double std = sqrt(var);
	if (min > 1e3)
	{
		min /= 1e3;
		max /= 1e3;
		avg /= 1e3;
		std /= 1e3;
		swprintf_s(stat, 128, _T("Max: %.2lf, Min: %.2lf, Avg: %.2lf, Std: %.2lf (ms)"), max, min, avg, std);
	}
	else
	{
		swprintf_s(stat, 128, _T("Max: %.2lf, Min: %.2lf, Avg: %.2lf, Std: %.2lf (us)"), max, min, avg, std);
	}
	graphics.DrawString(stat, lstrlenW(stat), &font, PointF(10.0f, 320.0f), &text);
	zlog_info(c, "绘图完毕");
}