//#include <windows.h>
//#include <tchar.h>
//
//#define ID_BUTTON_HELLO   1
//#define ID_BUTTON_RANDOM  2
//#define ID_BUTTON_COLOR   3
//#define ID_BUTTON_INFO    4
//#define ID_BUTTON_EXIT    5
//
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//HINSTANCE hInst;
//
//COLORREF currentColor = RGB(255, 255, 255);
//
//int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
//{
//    const wchar_t CLASS_NAME[] = L"Lab2WinAPIImproved";
//
//    WNDCLASS wc = {};
//    wc.lpfnWndProc = WndProc;
//    wc.hInstance = hInstance;
//    wc.lpszClassName = CLASS_NAME;
//    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
//
//    RegisterClass(&wc);
//
//    HWND hWnd = CreateWindowEx(
//        0,
//        CLASS_NAME,
//        L"Лабораторная №2 — Определение кнопки и MessageBox",
//        WS_OVERLAPPEDWINDOW,
//        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,
//        nullptr, nullptr, hInstance, nullptr
//    );
//
//    if (!hWnd) return 0;
//
//    hInst = hInstance;
//    ShowWindow(hWnd, nCmdShow);
//    UpdateWindow(hWnd);
//
//    MSG msg;
//    while (GetMessage(&msg, nullptr, 0, 0))
//    {
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }
//
//    return (int)msg.wParam;
//}
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    static HWND btnHello, btnRandom, btnColor, btnInfo, btnExit;
//    HDC hdc;
//    PAINTSTRUCT ps;
//    RECT rect;
//
//    switch (message)
//    {
//    case WM_CREATE:
//        btnHello = CreateWindow(L"BUTTON", L"Поздороваться", WS_VISIBLE | WS_CHILD,
//            30, 30, 150, 35, hWnd, (HMENU)ID_BUTTON_HELLO, hInst, nullptr);
//
//        btnRandom = CreateWindow(L"BUTTON", L"Случайное число", WS_VISIBLE | WS_CHILD,
//            30, 80, 150, 35, hWnd, (HMENU)ID_BUTTON_RANDOM, hInst, nullptr);
//
//        btnColor = CreateWindow(L"BUTTON", L"Сменить цвет", WS_VISIBLE | WS_CHILD,
//            30, 130, 150, 35, hWnd, (HMENU)ID_BUTTON_COLOR, hInst, nullptr);
//
//        btnInfo = CreateWindow(L"BUTTON", L"Информация", WS_VISIBLE | WS_CHILD,
//            30, 180, 150, 35, hWnd, (HMENU)ID_BUTTON_INFO, hInst, nullptr);
//
//        btnExit = CreateWindow(L"BUTTON", L"Выход", WS_VISIBLE | WS_CHILD,
//            30, 230, 150, 35, hWnd, (HMENU)ID_BUTTON_EXIT, hInst, nullptr);
//        break;
//
//    case WM_COMMAND:
//        switch (LOWORD(wParam))
//        {
//        case ID_BUTTON_HELLO:
//            MessageBox(hWnd, L"Привет! Это событие от кнопки 'Поздороваться'.",
//                L"Сообщение", MB_OK | MB_ICONINFORMATION);
//            break;
//
//        case ID_BUTTON_RANDOM:
//        {
//            int num = rand() % 100 + 1;
//            wchar_t buffer[100];
//            wsprintf(buffer, L"Случайное число: %d", num);
//            MessageBox(hWnd, buffer, L"Результат", MB_OK | MB_ICONASTERISK);
//        }
//        break;
//
//        case ID_BUTTON_COLOR:
//        {
//            int r = rand() % 256;
//            int g = rand() % 256;
//            int b = rand() % 256;
//            currentColor = RGB(r, g, b);
//            InvalidateRect(hWnd, NULL, TRUE);
//        }
//        break;
//
//        case ID_BUTTON_INFO:
//        {
//            int result = MessageBox(hWnd,
//                L"Это демонстрация диалоговых окон.\nХочешь продолжить?",
//                L"Информация",
//                MB_YESNOCANCEL | MB_ICONQUESTION);
//
//            if (result == IDYES)
//                MessageBox(hWnd, L"Отлично! Двигаемся дальше.", L"Ответ", MB_OK);
//            else if (result == IDNO)
//                MessageBox(hWnd, L"Жаль, но ладно.", L"Ответ", MB_OK);
//            else
//                MessageBox(hWnd, L"Ты закрыл окно выбора.", L"Ответ", MB_OK);
//        }
//        break;
//
//        case ID_BUTTON_EXIT:
//        {
//            int result = MessageBox(hWnd, L"Выйти из программы?", L"Подтверждение", MB_YESNO | MB_ICONQUESTION);
//            if (result == IDYES)
//                PostQuitMessage(0);
//        }
//        break;
//        }
//        break;
//
//        case WM_PAINT:
//        {
//            PAINTSTRUCT ps;
//            HDC hdc = BeginPaint(hWnd, &ps);
//            RECT rect;
//            GetClientRect(hWnd, &rect);
//
//            HBRUSH brush = CreateSolidBrush(currentColor);
//            FillRect(hdc, &rect, brush);
//            DeleteObject(brush);
//
//            EndPaint(hWnd, &ps);
//        }
//        break;
//
//
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//
//    default:
//        return DefWindowProc(hWnd, message, wParam, lParam);
//    }
//    return 0;
//}
