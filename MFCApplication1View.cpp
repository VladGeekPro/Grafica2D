
// MFCApplication1View.cpp: реализация класса CMFCApplication1View
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "Procedure.h"
#include <fstream>
#include <cstdlib>


// CMFCApplication1View

IMPLEMENT_DYNCREATE(CMFCApplication1View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication1View, CView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCApplication1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
//	ON_WM_LBUTTONDOWN()
//	ON_WM_ACTIVATEAPP()
END_MESSAGE_MAP()

// Создание или уничтожение CMFCApplication1View

CMFCApplication1View::CMFCApplication1View() noexcept
{
	// TODO: добавьте код создания

}

CMFCApplication1View::~CMFCApplication1View()
{
}

BOOL CMFCApplication1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CMFCApplication1View

//void CMFCApplication1View::OnDraw(CDC* /*pDC*/)
//{
//	CMFCApplication1Doc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
//	if (!pDoc)
//		return;
//
//	// TODO: добавьте здесь код отрисовки для собственных данных
//}


// Печать CMFCApplication1View


void CMFCApplication1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCApplication1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CMFCApplication1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью
}

void CMFCApplication1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}

void CMFCApplication1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCApplication1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Диагностика CMFCApplication1View

#ifdef _DEBUG
void CMFCApplication1View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication1Doc* CMFCApplication1View::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication1Doc)));
	return (CMFCApplication1Doc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CMFCApplication1View

//Part 2 

void createFiles(double a, double b, double h, double epsilon) {
    std::ofstream file1("file1.txt");
    std::ofstream file2("file2.txt");

    double x = a;

    while (x < b) {
        double resultOwn = ln1MinusX(x, epsilon);
        double resultLibrary = std::log(1 - x);
        double absoluteDifference = std::abs(resultOwn - resultLibrary);

        // Запись в file1.txt
        file1 << x << " " << resultOwn << " " << resultLibrary << " " << absoluteDifference << " " << epsilon << std::endl;

        // Запись в file2.txt
        file2 << x << " " << resultOwn << std::endl;

        x += h;
    }

    file1.close();
    file2.close();

    system("gnuplot -persist C:\\Users\\CRISTINA\\Desktop\\Grafica\\MFCApplication1\\MFCApplication1\\gnuplot_commands.txt");
}


void CMFCApplication1View::OnDraw(CDC* pDC)
{
    CMFCApplication1Doc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    double a = -1;
    double b = 1.0;
    double h = 0.1;

    COLORREF lineColorBlue = RGB(0, 0, 255);
    COLORREF pointColorRed = RGB(255, 0, 0); 

    int width = pDC->GetDeviceCaps(HORZRES);
    int height = pDC->GetDeviceCaps(VERTRES);
    int x0 = width / 4;
    int y0 = height / 4;

    
    double x = a;
    double epsilon = 1e-6;

    createFiles(a, b, h, epsilon);

    double y = ln1MinusX(x, epsilon);
    int xPrev = x0 ;
    int yPrev = y0 - int(y * 100);

    x += h;

    while (x < b) {
        y = ln1MinusX(x, epsilon);
        int xCurrent = x0 + int((x - a) * 100);
        int yCurrent = y0 - int(y * 100);

        pDC->SelectObject(GetStockObject(DC_PEN));
        pDC->SetDCPenColor(lineColorBlue);

        pDC->MoveTo(xPrev, yPrev);
        pDC->LineTo(xCurrent, yCurrent);

        xPrev = xCurrent;
        yPrev = yCurrent;

        x += h;
    }

    
    x = a;
    xPrev = x0;
    yPrev = y0 - int(std::log(1 - x) * 100);

    pDC->SelectObject(GetStockObject(DC_PEN));
    pDC->SetDCPenColor(pointColorRed);

    while (x < b) {
        int xCurrent = x0 + int((x - a) * 100);
        int yCurrent = y0 - int(std::log(1 - x) * 100);

        int pointSize = 2;  

        pDC->Ellipse(xCurrent - pointSize, yCurrent - pointSize, xCurrent + pointSize, yCurrent + pointSize);


        x += h;
    }



}



//


