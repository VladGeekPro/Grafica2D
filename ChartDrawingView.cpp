
// ChartDrawingView.cpp: реализация класса CChartDrawingView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "ChartDrawing.h"
#endif

//#include "stdafx.h"
#include "ChartDrawing.h"  
#include "ChartDrawingDoc.h"
#include "ChartDrawingView.h"
#include <fstream>
#include <sstream>
#include <unordered_map>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include <vector>
#include "ChartDrawing.h"
#include "ChartDrawingDoc.h"
#include "ChartDrawingView.h"
#include <fstream>
#include <sstream>

// CChartDrawingView

IMPLEMENT_DYNCREATE(CChartDrawingView, CView)

BEGIN_MESSAGE_MAP(CChartDrawingView, CView)
    // Стандартные команды печати
    ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CChartDrawingView::OnFilePrintPreview)
    ON_WM_CONTEXTMENU()
    ON_WM_RBUTTONUP()
    ON_WM_PAINT()
END_MESSAGE_MAP()

// Создание или уничтожение CChartDrawingView

CChartDrawingView::CChartDrawingView() noexcept
{
    // TODO: добавьте код создания

}

CChartDrawingView::~CChartDrawingView()
{
}

BOOL CChartDrawingView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: изменить класс Window или стили посредством изменения
    //  CREATESTRUCT cs

    return CView::PreCreateWindow(cs);
}

// Рисование CChartDrawingView

void CChartDrawingView::OnDraw(CDC* pDC)
{
    CChartDrawingDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // Получаем размеры области рисования
    CRect rcClient;
    GetClientRect(&rcClient);
    int maxx = rcClient.Width();
    int maxy = rcClient.Height();

    // Считываем данные из текстового файла
    std::wifstream inputFile(L"C:\\Users\\CRISTINA\\Desktop\\Grafica\\chartData.txt");
    if (!inputFile.is_open())
    {
        AfxMessageBox(L"Unable to open file");
        return;
    }

    // Пропускаем заголовок
    std::wstring line;
    std::getline(inputFile, line);

    // Читаем данные и рисуем диаграмму
    int yPos = 50; // начальная вертикальная позиция для рисования
    int maxValue = 0; // максимальное значение прибыли
    std::vector<std::wstring> companyNames; // названия компаний
    while (std::getline(inputFile, line))
    {
        std::wstringstream iss(line);
        std::wstring companyName;
        int profit;
        if (iss >> companyName >> profit)
        {
            companyNames.push_back(companyName);
            maxValue = (maxValue > profit) ? maxValue : profit;

        }
    }

    // Рисуем ось OY
    int stepY = static_cast<int>((maxy - 70) / static_cast<int>(companyNames.size()));
    for (int i = 0; i < static_cast<int>(companyNames.size()); ++i)
    {
        pDC->TextOut(40, 50 + i * stepY + stepY / 2 - 10, companyNames[i].c_str());
    }


    // Рисуем ось OX
    int stepX = static_cast<int>((maxx - 50) / (maxValue / 50));
    for (int i = 0; i <= maxValue; i += 50)
    {
        CString label;
        label.Format(L"%d", i);
        pDC->TextOut(147 + i * stepX / 100, maxy - 20, label);
    }



    // Рисуем диаграмму
    inputFile.close(); // Закрываем и снова открываем, чтобы вернуться в начало файла
    inputFile.open(L"C:\\Users\\CRISTINA\\Desktop\\Grafica\\chartData.txt");
    std::getline(inputFile, line); // Пропускаем заголовок

    for (int i = 0; i < companyNames.size(); ++i)
    {
        std::getline(inputFile, line);
        std::wstringstream iss(line);
        std::wstring companyName;
        int profit;
        if (iss >> companyName >> profit)
        {
            CBrush brFill(RGB(0, 0, 255)); // синий цвет, вы можете изменить его
            pDC->SelectObject(&brFill);
            pDC->Rectangle(150, 55 + i * stepY, 150 + profit * stepX / 100, 40 + (i + 1) * stepY);
        }
    }


    // Закрываем файл
    inputFile.close();

    // Рисуем doughnut chart
    int totalProfit = 0;
    std::vector<double> percentages;

    // Снова открываем файл
    inputFile.close();
    inputFile.open(L"C:\\Users\\CRISTINA\\Desktop\\Grafica\\chartData.txt");

    // Пропускаем заголовок
    std::getline(inputFile, line);

    // Вычисляем общую прибыль
    while (std::getline(inputFile, line))
    {
        std::wstringstream iss(line);
        std::wstring companyName;
        int profit;
        if (iss >> companyName >> profit)
        {
            totalProfit += profit;
        }
    }

    // Возвращаемся в начало файла, чтобы прочитать данные снова
    inputFile.clear();
    inputFile.seekg(0, std::ios::beg);
    std::getline(inputFile, line); // Пропускаем заголовок

    int centerX = 700;
    int centerY = 175;
    int radius = 80;
    double startAngle = 90;

    // Создайте ассоциативный массив для хранения цветов компаний
    std::unordered_map<std::wstring, COLORREF> companyColors = {
        {L"BlueMouse", RGB(0, 0, 255)},          // Синий
        {L"OrangeCream", RGB(255, 69, 0)},       // Красный
        {L"AquaFresh", RGB(255, 255, 0)},        // Желтый
        {L"InnovateCorp", RGB(0, 128, 0)},       // Зеленый
        {L"QuantumIndust", RGB(255, 165, 0)},  // Оранжевый
        {L"TechSolutions", RGB(0, 206, 209)},    // Бирюзовый
        {L"GlobalVentures", RGB(173, 216, 230)}  // Голубой
    };

    // Рисуем doughnut chart
    while (std::getline(inputFile, line))
    {
        std::wstringstream iss(line);
        std::wstring companyName;
        int profit;
        if (iss >> companyName >> profit)
        {
            // Рассчитываем процент для каждой компании
            double percentage = static_cast<double>(profit) / static_cast<double>(totalProfit);
            percentages.push_back(percentage);

            // Рассчитываем углы для внешней и внутренней окружности сегмента
            double outerStartAngle = startAngle;
            double outerEndAngle = startAngle - 360.0 * percentage;

            // Получаем цвет для текущей компании
            COLORREF fillColor = companyColors[companyName];

            // Преобразуем значения в тип int
            int outerStartAngleInt = static_cast<int>(outerStartAngle);
            int outerEndAngleInt = static_cast<int>(outerEndAngle);

            // Рисуем кольцевой сегмент
            CBrush brFill(fillColor);
            pDC->SelectObject(&brFill);
            pDC->Pie(centerX - radius, centerY - radius, centerX + radius, centerY + radius,
                centerX + radius * cos(outerEndAngleInt * 3.14 / 180), centerY - radius * sin(outerEndAngleInt * 3.14 / 180),
                centerX + radius * cos(outerStartAngleInt * 3.14 / 180), centerY - radius * sin(outerStartAngleInt * 3.14 / 180));

            // Обновляем углы для следующего сегмента
            startAngle = outerEndAngle;
        }

        // Рисуем белый круг внутри сегмента
        CBrush brFill(RGB(255, 255, 255));
        pDC->SelectObject(&brFill);
        pDC->Ellipse(centerX - (radius - 37), centerY - (radius - 37), centerX + (radius - 37), centerY + (radius - 37));
    }

    // Закрываем файл
    inputFile.close();

    // Восстанавливаем исходные перо и кисть
    pDC->SelectStockObject(NULL_PEN);
    pDC->SelectStockObject(NULL_BRUSH);
}

// Печать CChartDrawingView


void CChartDrawingView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
    AFXPrintPreview(this);
#endif
}

BOOL CChartDrawingView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // подготовка по умолчанию
    return DoPreparePrinting(pInfo);
}

void CChartDrawingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: добавьте дополнительную инициализацию перед печатью
}

void CChartDrawingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: добавьте очистку после печати
}

void CChartDrawingView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
    ClientToScreen(&point);
    OnContextMenu(this, point);
}

void CChartDrawingView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
    theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Диагностика CChartDrawingView

#ifdef _DEBUG
void CChartDrawingView::AssertValid() const
{
    CView::AssertValid();
}

void CChartDrawingView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CChartDrawingDoc* CChartDrawingView::GetDocument() const // встроена неотлаженная версия
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CChartDrawingDoc)));
    return (CChartDrawingDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CChartDrawingView
