//#include <windows.h>
//#include <tchar.h>
//#include <string>
//
//#define ID_EDIT_X     101
//#define ID_EDIT_Y     102205
//#define ID_EDIT_Z     103
//#define ID_EDIT_RESULT 104
//#define ID_EDIT_PERCENT 105
//
//#define ID_BUTTON_CALC 201
//#define ID_BUTTON_PERCENT 202
//
//#define ID_RADIO_10   301
//#define ID_RADIO_20   302
//#define ID_RADIO_50   303
//#define ID_GROUPBOX   304
//
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//HINSTANCE hInst;
//
//int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
//{
//    const wchar_t CLASS_NAME[] = L"Lab3CalcClass";
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
//    HWND hWnd = CreateWindow(CLASS_NAME,
//        L"Лабораторная №3 — Расчет и проценты",
//        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, // без изменения размера
//        CW_USEDEFAULT, CW_USEDEFAULT, 550, 350,
//        nullptr, nullptr, hInstance, nullptr);
//
//    if (!hWnd)
//        return 0;
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
//LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//    static HWND editX, editY, editZ, editResult, editPercent;
//    static HWND btnCalc, btnPercent;
//    static HWND groupBox, radio10, radio20, radio50;
//
//    switch (msg)
//    {
//    case WM_CREATE:
//        CreateWindow(L"STATIC", L"Введите X:", WS_VISIBLE | WS_CHILD,
//            30, 30, 100, 20, hWnd, 0, hInst, nullptr);
//        editX = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
//            130, 25, 100, 25, hWnd, (HMENU)ID_EDIT_X, hInst, nullptr);
//
//        CreateWindow(L"STATIC", L"Введите Y:", WS_VISIBLE | WS_CHILD,
//            30, 65, 100, 20, hWnd, 0, hInst, nullptr);
//        editY = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
//            130, 60, 100, 25, hWnd, (HMENU)ID_EDIT_Y, hInst, nullptr);
//
//        CreateWindow(L"STATIC", L"Введите Z:", WS_VISIBLE | WS_CHILD,
//            30, 100, 100, 20, hWnd, 0, hInst, nullptr);
//        editZ = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
//            130, 95, 100, 25, hWnd, (HMENU)ID_EDIT_Z, hInst, nullptr);
//
//        CreateWindow(L"STATIC", L"Результат (x+y)/z:", WS_VISIBLE | WS_CHILD,
//            30, 145, 120, 20, hWnd, 0, hInst, nullptr);
//        editResult = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY,
//            160, 140, 100, 25, hWnd, (HMENU)ID_EDIT_RESULT, hInst, nullptr);
//
//        btnCalc = CreateWindow(L"BUTTON", L"Вычислить", WS_VISIBLE | WS_CHILD,
//            280, 138, 100, 28, hWnd, (HMENU)ID_BUTTON_CALC, hInst, nullptr);
//
//        // Группа радиокнопок
//        groupBox = CreateWindow(L"BUTTON", L"Выберите процент", WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
//            30, 190, 220, 100, hWnd, (HMENU)ID_GROUPBOX, hInst, nullptr);
//
//        radio10 = CreateWindow(L"BUTTON", L"10%", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP,
//            50, 215, 80, 20, hWnd, (HMENU)ID_RADIO_10, hInst, nullptr);
//        radio20 = CreateWindow(L"BUTTON", L"20%", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
//            50, 240, 80, 20, hWnd, (HMENU)ID_RADIO_20, hInst, nullptr);
//        radio50 = CreateWindow(L"BUTTON", L"50%", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
//            50, 265, 80, 20, hWnd, (HMENU)ID_RADIO_50, hInst, nullptr);
//
//        // Результат процента
//        CreateWindow(L"STATIC", L"Результат процента:", WS_VISIBLE | WS_CHILD,
//            270, 215, 130, 20, hWnd, 0, hInst, nullptr);
//        editPercent = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY,
//            400, 210, 100, 25, hWnd, (HMENU)ID_EDIT_PERCENT, hInst, nullptr);
//
//        btnPercent = CreateWindow(L"BUTTON", L"Рассчитать %", WS_VISIBLE | WS_CHILD,
//            280, 260, 100, 28, hWnd, (HMENU)ID_BUTTON_PERCENT, hInst, nullptr);
//        break;
//
//    case WM_COMMAND:
//    {
//        switch (LOWORD(wParam))
//        {
//        case ID_BUTTON_CALC:
//        {
//            wchar_t sx[50], sy[50], sz[50];
//            GetWindowText(editX, sx, 50);
//            GetWindowText(editY, sy, 50);
//            GetWindowText(editZ, sz, 50);
//
//            double x = _wtof(sx);
//            double y = _wtof(sy);
//            double z = _wtof(sz);
//
//            if (z == 0) {
//                MessageBox(hWnd, L"Ошибка: деление на ноль!", L"Ошибка", MB_OK | MB_ICONERROR);
//                break;
//            }
//
//            double result = (x + y) / z;
//
//            wchar_t output[50];
//            swprintf_s(output, L"%.2f", result);
//            SetWindowText(editResult, output);
//        }
//        break;
//
//        case ID_BUTTON_PERCENT:
//        {
//            wchar_t sres[50];
//            GetWindowText(editResult, sres, 50);
//            double baseValue = _wtof(sres);
//            double percent = 0;
//
//            if (SendMessage(radio10, BM_GETCHECK, 0, 0) == BST_CHECKED)
//                percent = 10;
//            else if (SendMessage(radio20, BM_GETCHECK, 0, 0) == BST_CHECKED)
//                percent = 20;
//            else if (SendMessage(radio50, BM_GETCHECK, 0, 0) == BST_CHECKED)
//                percent = 50;
//
//            double value = baseValue * percent / 100;
//
//            wchar_t output[50];
//            swprintf_s(output, L"%.2f", value);
//            SetWindowText(editPercent, output);
//        }
//        break;
//        }
//    }
//    break;
//
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//
//    default:
//        return DefWindowProc(hWnd, msg, wParam, lParam);
//    }
//    return 0;
//}
