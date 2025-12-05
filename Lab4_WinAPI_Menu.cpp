// Lab4_WinAPI_Menu.cpp
// Полноценная версия ЛР4 (вариант A): меню, OpenFileDialog (PNG/JPG/BMP через GDI+),
// режимы отображения, панель управления (TextBox, ComboBox), адаптивный layout,
// фильтры: Чёрно-белый и Сепия.

#include <windows.h>
#include <commdlg.h>
#include <gdiplus.h>
#include <string>

#pragma comment (lib, "gdiplus.lib")

using namespace Gdiplus;

// ID элементов
enum {
    ID_MENU_FILE_OPEN = 9001,
    ID_MENU_FILE_EXIT,
    ID_MENU_VIEW_NORMAL,
    ID_MENU_VIEW_STRETCH,
    ID_MENU_VIEW_CENTER,
    ID_MENU_VIEW_ZOOM,
    ID_MENU_HELP_ABOUT,

    ID_CTRL_EDIT = 9101,
    ID_CTRL_COMBO,
    ID_LABEL_EDIT,
    ID_LABEL_COMBO,

    ID_FILTER_GRAY = 9201,
    ID_FILTER_SEPIA = 9202
};

// Отображение картинки
enum DisplayMode { MODE_NORMAL = 0, MODE_STRETCH, MODE_CENTER, MODE_ZOOM };

// Глобальные
HINSTANCE g_hInst = nullptr;
ULONG_PTR g_gdiplusToken = 0;
Image* g_image = nullptr;
std::wstring g_imagePath;
DisplayMode g_displayMode = MODE_NORMAL;
COLORREF g_bgColor = RGB(240, 240, 240);

// Контролы интерфейса
HWND g_hEdit = nullptr;
HWND g_hCombo = nullptr;
HWND g_hLabelEdit = nullptr;
HWND g_hLabelCombo = nullptr;

// Прототипы
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CreateAppMenu(HWND);
void LayoutControls(HWND);
bool LoadImageFromFile(const std::wstring& path);
void FreeLoadedImage();
void DrawImageInRect(Graphics& g, RECT rcClient);
void ApplyGrayScale();
void ApplySepia();

// Вспомогательная функция: показать OpenFileDialog (unicode)
bool ShowOpenFileDialog(HWND hwndOwner, std::wstring& outPath)
{
    OPENFILENAME ofn;
    wchar_t szFile[4096] = { 0 };
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwndOwner;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
    ofn.lpstrFilter = L"Изображения (*.png;*.jpg;*.jpeg;*.bmp)\0*.png;*.jpg;*.jpeg;*.bmp\0Все файлы\0*.*\0";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.lpstrTitle = L"Открыть изображение";
    if (GetOpenFileName(&ofn))
    {
        outPath = std::wstring(ofn.lpstrFile);
        return true;
    }
    return false;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int nCmdShow)
{
    g_hInst = hInstance;

    // Инициализация GDI+
    GdiplusStartupInput gdiplusStartupInput;
    if (GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, nullptr) != Ok) {
        MessageBox(nullptr, L"Не удалось инициализировать GDI+.", L"Ошибка", MB_OK | MB_ICONERROR);
        return 0;
    }

    const wchar_t CLASS_NAME[] = L"Lab4WinApiClass";
    WNDCLASS wc{};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(g_bgColor);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"ЛР4 — Меню и работа с изображением (GDI+)",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 900, 600,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hwnd) {
        GdiplusShutdown(g_gdiplusToken);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Цикл сообщений
    MSG msg{};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Очистка
    FreeLoadedImage();
    GdiplusShutdown(g_gdiplusToken);
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        // Меню
        CreateAppMenu(hwnd);

        // Панель управления: подписи, edit и combobox
        g_hLabelEdit = CreateWindowW(L"STATIC", L"Текст сбоку:", WS_CHILD | WS_VISIBLE,
            10, 0, 120, 20, hwnd, (HMENU)ID_LABEL_EDIT, g_hInst, nullptr);

        g_hEdit = CreateWindowW(L"EDIT", L"Введи любой текст", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
            10, 0, 240, 24, hwnd, (HMENU)ID_CTRL_EDIT, g_hInst, nullptr);

        g_hLabelCombo = CreateWindowW(L"STATIC", L"Выбери вариант:", WS_CHILD | WS_VISIBLE,
            260, 0, 120, 20, hwnd, (HMENU)ID_LABEL_COMBO, g_hInst, nullptr);

        g_hCombo = CreateWindowW(L"COMBOBOX", nullptr, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,
            260, 0, 200, 200, hwnd, (HMENU)ID_CTRL_COMBO, g_hInst, nullptr);

        // Заполняем ComboBox вариантами
        SendMessageW(g_hCombo, (UINT)CB_ADDSTRING, 0, (LPARAM)L"Вариант A");
        SendMessageW(g_hCombo, (UINT)CB_ADDSTRING, 0, (LPARAM)L"Вариант B");
        SendMessageW(g_hCombo, (UINT)CB_ADDSTRING, 0, (LPARAM)L"Вариант C");
        SendMessageW(g_hCombo, (UINT)CB_SETCURSEL, 0, 0);

        // Layout
        LayoutControls(hwnd);
    }
    return 0;

    case WM_SIZE:
        LayoutControls(hwnd);
        InvalidateRect(hwnd, nullptr, TRUE);
        return 0;

    case WM_COMMAND:
    {
        int id = LOWORD(wParam);
        switch (id)
        {
        case ID_MENU_FILE_OPEN:
        {
            std::wstring path;
            if (ShowOpenFileDialog(hwnd, path)) {
                if (!LoadImageFromFile(path)) {
                    std::wstring msg = L"Не удалось загрузить изображение:\n" + path;
                    MessageBox(hwnd, msg.c_str(), L"Ошибка", MB_OK | MB_ICONERROR);
                }
                else {
                    // сохранить путь, перерисовать
                    g_imagePath = path;
                    InvalidateRect(hwnd, nullptr, TRUE);
                }
            }
            break;
        }
        case ID_MENU_FILE_EXIT:
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            break;

        case ID_MENU_VIEW_NORMAL:
            g_displayMode = MODE_NORMAL;
            InvalidateRect(hwnd, nullptr, TRUE);
            break;
        case ID_MENU_VIEW_STRETCH:
            g_displayMode = MODE_STRETCH;
            InvalidateRect(hwnd, nullptr, TRUE);
            break;
        case ID_MENU_VIEW_CENTER:
            g_displayMode = MODE_CENTER;
            InvalidateRect(hwnd, nullptr, TRUE);
            break;
        case ID_MENU_VIEW_ZOOM:
            g_displayMode = MODE_ZOOM;
            InvalidateRect(hwnd, nullptr, TRUE);
            break;

        case ID_MENU_HELP_ABOUT:
            MessageBox(hwnd,
                L"Лабораторная работа №4\nWinAPI + GDI+\nМеню, загрузка изображений и режимы отображения.",
                L"О программе", MB_OK | MB_ICONINFORMATION);
            break;

        case ID_CTRL_EDIT:
            if (HIWORD(wParam) == EN_UPDATE || HIWORD(wParam) == EN_CHANGE) {
                // Текст в edit изменился — можно реагировать при необходимости
            }
            break;

        case ID_CTRL_COMBO:
            if (HIWORD(wParam) == CBN_SELCHANGE) {
                int sel = (int)SendMessage(g_hCombo, (UINT)CB_GETCURSEL, 0, 0);
                if (sel >= 0) {
                    wchar_t text[256]; SendMessageW(g_hCombo, (UINT)CB_GETLBTEXT, sel, (LPARAM)text);
                    std::wstring msg = L"Вы выбрали: ";
                    msg += text;
                    MessageBox(hwnd, msg.c_str(), L"Комбо", MB_OK | MB_ICONINFORMATION);
                }
            }
            break;

        case ID_FILTER_GRAY: // Grayscale
            ApplyGrayScale();
            InvalidateRect(hwnd, NULL, TRUE);
            break;

        case ID_FILTER_SEPIA: // Sepia
            ApplySepia();
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
    }
    return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        // Используем GDI+ Graphics для плавного рендера
        {
            Graphics g(hdc);
            RECT rc;
            GetClientRect(hwnd, &rc);

            // Рисуем фон
            SolidBrush bgBrush(Color(GetRValue(g_bgColor), GetGValue(g_bgColor), GetBValue(g_bgColor)));
            g.FillRectangle(&bgBrush, (REAL)rc.left, (REAL)rc.top, (REAL)(rc.right - rc.left), (REAL)(rc.bottom - rc.top));

            // Вычисляем прямоугольник, где изображение будет рисоваться:
            // оставляем верхнюю полосу (например, 40px) для меню/панели
            RECT rcImage = rc;
            const int topMargin = 40;
            rcImage.top += topMargin;

            // Отрисовка рамки под изображение
            Pen pen(Color(180, 180, 180));
            g.DrawRectangle(&pen, (REAL)rcImage.left + 10, (REAL)rcImage.top + 10, (REAL)(rcImage.right - rcImage.left - 20), (REAL)(rcImage.bottom - rcImage.top - 20));

            if (g_image) {
                DrawImageInRect(g, rcImage);
            }
            else {
                // Если нет изображения — показать подсказку
                FontFamily ff(L"Segoe UI");
                Font font(&ff, 14, FontStyleRegular, UnitPixel);
                SolidBrush txtBrush(Color(80, 80, 80));
                std::wstring hint = L"Файл не загружен. Файл -> Открыть (поддерживается PNG/JPG/BMP)";
                g.DrawString(hint.c_str(), -1, &font, PointF((REAL)rcImage.left + 20, (REAL)rcImage.top + 20), &txtBrush);
            }
        }
        EndPaint(hwnd, &ps);
    }
    return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// Создаём меню приложения
void CreateAppMenu(HWND hwnd)
{
    HMENU hMenu = CreateMenu();
    HMENU hFile = CreatePopupMenu();
    HMENU hView = CreatePopupMenu();
    HMENU hHelp = CreatePopupMenu();
    HMENU hFilters = CreatePopupMenu();

    // --- Фильтры ---
    AppendMenuW(hFilters, MF_STRING, ID_FILTER_GRAY, L"Чёрно-белый");
    AppendMenuW(hFilters, MF_STRING, ID_FILTER_SEPIA, L"Сепия");

    // --- Файл ---
    AppendMenuW(hFile, MF_STRING, ID_MENU_FILE_OPEN, L"&Открыть...\tCtrl+O");
    AppendMenuW(hFile, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(hFile, MF_STRING, ID_MENU_FILE_EXIT, L"&Выход");

    // --- Вид ---
    AppendMenuW(hView,
        MF_STRING | ((g_displayMode == MODE_NORMAL) ? MF_CHECKED : MF_UNCHECKED),
        ID_MENU_VIEW_NORMAL, L"Normal");
    AppendMenuW(hView,
        MF_STRING | ((g_displayMode == MODE_STRETCH) ? MF_CHECKED : MF_UNCHECKED),
        ID_MENU_VIEW_STRETCH, L"Stretch");
    AppendMenuW(hView,
        MF_STRING | ((g_displayMode == MODE_CENTER) ? MF_CHECKED : MF_UNCHECKED),
        ID_MENU_VIEW_CENTER, L"Center");
    AppendMenuW(hView,
        MF_STRING | ((g_displayMode == MODE_ZOOM) ? MF_CHECKED : MF_UNCHECKED),
        ID_MENU_VIEW_ZOOM, L"Zoom");

    // --- Справка ---
    AppendMenuW(hHelp, MF_STRING, ID_MENU_HELP_ABOUT, L"&О программе");

    // Порядок меню (ВАЖНО!)
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hFile, L"&Файл");
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hView, L"&Вид");
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hFilters, L"&Фильтры");
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hHelp, L"&Справка");

    SetMenu(hwnd, hMenu);
}

// Расстановка контролов (адаптивный layout)
void LayoutControls(HWND hwnd)
{
    RECT rc; GetClientRect(hwnd, &rc);
    const int margin = 10;
    const int topOffset = 4;
    const int ctrlHeight = 24;

    // Edit и Combo в верхней панели
    int x = margin;
    int y = topOffset + 4;
    int editW = 260;
    int comboW = 180;

    MoveWindow(g_hLabelEdit, x, y, 100, ctrlHeight, TRUE);
    MoveWindow(g_hEdit, x + 100, y, editW, ctrlHeight, TRUE);

    MoveWindow(g_hLabelCombo, x + 100 + editW + 10, y, 100, ctrlHeight, TRUE);
    MoveWindow(g_hCombo, x + 100 + editW + 110, y, comboW, ctrlHeight, TRUE);
}

// Загрузка изображения (через GDI+)
bool LoadImageFromFile(const std::wstring& path)
{
    FreeLoadedImage();
    // Создаём Image. GDI+ поддерживает PNG, JPG, BMP и др.
    Image* img = Image::FromFile(path.c_str());
    if (!img) return false;
    Status st = img->GetLastStatus();
    if (st != Ok) {
        delete img;
        return false;
    }
    g_image = img;
    return true;
}

void FreeLoadedImage()
{
    if (g_image) {
        delete g_image;
        g_image = nullptr;
    }
}

// Вспомогательная: рисует g_image в области rcClient по текущему режиму
void DrawImageInRect(Graphics& g, RECT rcClient)
{
    if (!g_image) return;

    // вычисляем рабочую область (с отступами)
    int left = rcClient.left + 20;
    int top = rcClient.top + 20;
    int right = rcClient.right - 20;
    int bottom = rcClient.bottom - 20;
    int w = right - left;
    int h = bottom - top;
    if (w <= 0 || h <= 0) return;

    REAL destX = (REAL)left, destY = (REAL)top, destW = (REAL)w, destH = (REAL)h;

    UINT imgW = g_image->GetWidth();
    UINT imgH = g_image->GetHeight();

    if (g_displayMode == MODE_NORMAL) {
        // рисуем в оригинальном размере в левом верхнем углу
        g.DrawImage(g_image, destX, destY, (REAL)imgW, (REAL)imgH);
    }
    else if (g_displayMode == MODE_STRETCH) {
        // растянуть картинку на весь rect (может исказить пропорции)
        g.DrawImage(g_image, destX, destY, destW, destH);
    }
    else if (g_displayMode == MODE_CENTER) {
        // разместить по центру без масштабирования
        REAL x = destX + (destW - imgW) / 2.0f;
        REAL y = destY + (destH - imgH) / 2.0f;
        g.DrawImage(g_image, x, y, (REAL)imgW, (REAL)imgH);
    }
    else if (g_displayMode == MODE_ZOOM) {
        // масштабируем пропорционально, чтобы умещалось в rect
        REAL scaleX = destW / (REAL)imgW;
        REAL scaleY = destH / (REAL)imgH;
        REAL scale = (scaleX < scaleY) ? scaleX : scaleY;
        REAL dw = imgW * scale;
        REAL dh = imgH * scale;
        REAL x = destX + (destW - dw) / 2.0f;
        REAL y = destY + (destH - dh) / 2.0f;
        g.DrawImage(g_image, x, y, dw, dh);
    }
}

// Быстрый фильтр: Черно-белый
void ApplyGrayScale()
{
    if (!g_image) return;

    ColorMatrix matrix = {
        0.3f, 0.3f, 0.3f, 0, 0,
        0.59f,0.59f,0.59f,0, 0,
        0.11f,0.11f,0.11f,0, 0,
        0,    0,    0,    1, 0,
        0,    0,    0,    0, 1
    };

    ImageAttributes attr;
    attr.SetColorMatrix(&matrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

    Bitmap* bmp = new Bitmap(g_image->GetWidth(), g_image->GetHeight(), PixelFormat32bppARGB);
    Graphics g(bmp);

    g.DrawImage(
        g_image,
        Rect(0, 0, bmp->GetWidth(), bmp->GetHeight()),
        0, 0, g_image->GetWidth(), g_image->GetHeight(),
        UnitPixel,
        &attr
    );

    delete g_image;
    g_image = bmp;
}

// Быстрый фильтр: Сепия
void ApplySepia()
{
    if (!g_image) return;

    ColorMatrix matrix = {
        0.393f, 0.349f, 0.272f, 0, 0,
        0.769f, 0.686f, 0.534f, 0, 0,
        0.189f, 0.168f, 0.131f, 0, 0,
        0,      0,      0,      1, 0,
        0,      0,      0,      0, 1
    };

    ImageAttributes attr;
    attr.SetColorMatrix(&matrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

    Bitmap* bmp = new Bitmap(g_image->GetWidth(), g_image->GetHeight(), PixelFormat32bppARGB);
    Graphics g(bmp);

    g.DrawImage(
        g_image,
        Rect(0, 0, bmp->GetWidth(), bmp->GetHeight()),
        0, 0, g_image->GetWidth(), g_image->GetHeight(),
        UnitPixel,
        &attr
    );

    delete g_image;
    g_image = bmp;
}
