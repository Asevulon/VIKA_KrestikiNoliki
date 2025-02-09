#include"pch.h"
#include"Drawer.h"
using namespace Gdiplus;

Drawer::Drawer()
{
	GdiplusStartupInput si{  };
	GdiplusStartup(&token, &si, NULL);
}
Drawer::~Drawer()
{
	GdiplusShutdown(token);
}
void Drawer::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	Graphics ToWindow(lpDrawItemStruct->hDC);
	width = lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left;
	height = lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;
	Bitmap bmp(width, height);
	Graphics gr(&bmp);
	gr.Clear(Color::White);

	if (m_data.size() != 9)return;

	gr.SetSmoothingMode(SmoothingModeAntiAlias);
	gr.TranslateTransform(3, 3);

	Pen BackgroundPen(Color::Black, 3);
	double WidthStep = width / 3. - 2;
	double HeightStep = height / 3. - 2;
	for (int i = 0; i <= 3; i++)
	{
		PointF p1(i * WidthStep, 0);
		PointF p2(i * WidthStep, height - 6);
		gr.DrawLine(&BackgroundPen, p1, p2);
	}

	for (int i = 0; i <= 3; i++)
	{
		PointF p1(0, i* HeightStep);
		PointF p2(width - 6, i* HeightStep);
		gr.DrawLine(&BackgroundPen, p1, p2);
	}

	Pen BluePen(Color::Blue, 2);
	Pen RedPen(Color::Red, 2);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (m_data[i * 3 + j] == 0)continue;
			if (m_data[i * 3 + j] == 1)
			{
				PointF p1((j + 0.1) * WidthStep, (i + 0.1) * HeightStep);
				PointF p2((j + 0.9) * WidthStep, (i + 0.9) * HeightStep);
				gr.DrawLine(&BluePen, p1, p2);

				PointF p3((j + 0.9) * WidthStep, (i + 0.1) * HeightStep);
				PointF p4((j + 0.1) * WidthStep, (i + 0.9) * HeightStep);
				gr.DrawLine(&BluePen, p3, p4);
			}
			else
			{
				PointF p1((j + 0.1) * WidthStep, (i + 0.1) * HeightStep);
				gr.DrawEllipse(&RedPen, p1.X, p1.Y, 0.8 * WidthStep, 0.8 * HeightStep);
			}
		}
	}

	BluePen.SetWidth(3);
	BluePen.SetColor(Color::BlueViolet);

	RedPen.SetWidth(3);
	RedPen.SetColor(Color::IndianRed);

	if ((m_data[0] == m_data[1]) && (m_data[0] == m_data[2]) && (m_data[0] != 0))
	{
		PointF p1(0, 0.5 * HeightStep);
		PointF p2(width - 6, 0.5 * HeightStep);
		if (m_data[0] == 1)gr.DrawLine(&BluePen, p1, p2);
		else gr.DrawLine(&RedPen, p1, p2);
	}
	if ((m_data[3] == m_data[4]) && (m_data[3] == m_data[5]) && (m_data[3] != 0))
	{
		PointF p1(0, 1.5 * HeightStep);
		PointF p2(width - 6, 1.5 * HeightStep);
		if (m_data[3] == 1)gr.DrawLine(&BluePen, p1, p2);
		else gr.DrawLine(&RedPen, p1, p2);
	}
	if ((m_data[6] == m_data[7]) && (m_data[6] == m_data[8]) && (m_data[6] != 0))
	{
		PointF p1(0, 2.5 * HeightStep);
		PointF p2(width - 6, 2.5 * HeightStep);
		if (m_data[6] == 1)gr.DrawLine(&BluePen, p1, p2);
		else gr.DrawLine(&RedPen, p1, p2);
	}

	if ((m_data[0] == m_data[3]) && (m_data[0] == m_data[6]) && (m_data[0] != 0))
	{
		PointF p1(0.5 * WidthStep, 0);
		PointF p2(0.5 * WidthStep, height - 6);
		if (m_data[0] == 1)gr.DrawLine(&BluePen, p1, p2);
		else gr.DrawLine(&RedPen, p1, p2);
	}
	if ((m_data[1] == m_data[4]) && (m_data[1] == m_data[7]) && (m_data[1] != 0))
	{
		PointF p1(1.5 * WidthStep, 0);
		PointF p2(1.5 * WidthStep, height - 6);
		if (m_data[1] == 1)gr.DrawLine(&BluePen, p1, p2);
		else gr.DrawLine(&RedPen, p1, p2);
	}
	if ((m_data[2] == m_data[5]) && (m_data[2] == m_data[8]) && (m_data[2] != 0))
	{
		PointF p1(2.5 * WidthStep, 0);
		PointF p2(2.5 * WidthStep, height - 6);
		if (m_data[2] == 1)gr.DrawLine(&BluePen, p1, p2);
		else gr.DrawLine(&RedPen, p1, p2);
	}

	if ((m_data[0] == m_data[4]) && (m_data[0] == m_data[8]) && (m_data[0] != 0))
	{
		PointF p1(0, 0);
		PointF p2(width - 6, height - 6);
		if (m_data[0] == 1)gr.DrawLine(&BluePen, p1, p2);
		else gr.DrawLine(&RedPen, p1, p2);
	}
	if ((m_data[2] == m_data[4]) && (m_data[2] == m_data[6]) && (m_data[2] != 0))
	{
		PointF p1(width - 6, 0);
		PointF p2(0, height - 6);
		if (m_data[2] == 1)gr.DrawLine(&BluePen, p1, p2);
		else gr.DrawLine(&RedPen, p1, p2);
	}


	ToWindow.DrawImage(&bmp, 0, 0);
}

void Drawer::SetData(std::vector<int>& val)
{
	m_data = val;
	Invalidate();
}



BEGIN_MESSAGE_MAP(Drawer, CStatic)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()




void Drawer::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (init) {
		if (!stop)
		{
			double WidthStep = width / 3;
			double HeightStep = height / 3;
			int i = point.y / HeightStep;
			int j = point.x / WidthStep;
			if ((i < 0) || (j < 0))return;
			if ((i > 2) || (j > 2))return;
			int pos = i * 3 + j;
			if (m_data[pos] != 0)return;
			PostMessageA(parent, TURN_DONE, pos, NULL);
		}
	}
	CStatic::OnRButtonUp(nFlags, point);
}
