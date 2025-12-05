//#include <windows.h>
//#include <tchar.h>
//#include <cmath>
//
//#define ID_DISPLAY   1
//#define ID_BTN_START 100
//#define ID_BTN_CLEAR 200
//
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//
//HINSTANCE hInst;
//HWND hDisplay;
//wchar_t currentInput[256] = L"";
//double lastValue = 0;
//wchar_t lastOp = 0;
//
//void UpdateDisplay() {
//    SetWindowText(hDisplay, currentInput);
//}
//
//void AddChar(wchar_t c) {
//    size_t len = wcslen(currentInput);
//    if (len < 255) {
//        currentInput[len] = c;
//        currentInput[len + 1] = 0;
//    }
//    UpdateDisplay();
//}
//
//void Calculate() {
//    double num = _wtof(currentInput);
//    switch (lastOp) {
//    case L'+': lastValue += num; break;
//    case L'-': lastValue -= num; break;
//    case L'*': lastValue *= num; break;
//    case L'/': lastValue = num != 0 ? lastValue / num : 0; break;
//    case L'^': lastValue = pow(lastValue, num); break;
//    case L'√': lastValue = sqrt(num); break;
//    case 0: lastValue = num; break;
//    }
//    swprintf(currentInput, 256, L"%.2f", lastValue);
//    UpdateDisplay();
//    lastOp = 0;
//}
//
//int APIENTRY wWinMain(HINSTANCE hInstance,
//    HINSTANCE hPrevInstance,
//    LPWSTR lpCmdLine,
//    int nCmdShow)
//{
//    UNREFERENCED_PARAMETER(hPrevInstance);
//    UNREFERENCED_PARAMETER(lpCmdLine);
//
//    const wchar_t CLASS_NAME[] = L"AdvancedCalc";
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
//        L"Калькулятор",
//        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
//        CW_USEDEFAULT, CW_USEDEFAULT, 300, 400,
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
//    while (GetMessage(&msg, nullptr, 0, 0)) {
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }
//    return (int)msg.wParam;
//}
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
//    static HWND buttons[20];
//    static wchar_t btnLabels[20] = {
//        L'7',L'8',L'9',L'/',
//        L'4',L'5',L'6',L'*',
//        L'1',L'2',L'3',L'-',
//        L'0',L'.',L'=',L'+',
//        L'√',L'^',L'C',0
//    };
//
//    switch (message) {
//    case WM_CREATE: {
//        hDisplay = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | ES_READONLY,
//            10, 10, 260, 40, hWnd, (HMENU)ID_DISPLAY, hInst, nullptr);
//
//        int x = 10, y = 60;
//        for (int i = 0; i < 20; i++) {
//            if (btnLabels[i] == 0) continue;
//            wchar_t buf[2] = { btnLabels[i], 0 };
//            buttons[i] = CreateWindow(L"BUTTON", buf,
//                WS_VISIBLE | WS_CHILD,
//                x, y, 60, 40, hWnd, (HMENU)(ID_BTN_START + i), hInst, nullptr);
//            x += 65;
//            if ((i + 1) % 4 == 0) { x = 10; y += 45; }
//        }
//        break;
//    }
//
//    case WM_COMMAND: {
//        int id = LOWORD(wParam);
//        if (id >= ID_BTN_START && id < ID_BTN_START + 20) {
//            int idx = id - ID_BTN_START;
//            wchar_t label = btnLabels[idx];
//
//            if ((label >= L'0' && label <= L'9') || label == L'.') {
//                AddChar(label);
//            }
//            else if (label == L'=') {
//                Calculate();
//            }
//            else if (label == L'C') {
//                currentInput[0] = 0;
//                lastOp = 0;
//                lastValue = 0;
//                UpdateDisplay();
//            }
//            else if (label == L'√') {
//                lastOp = label;
//                Calculate();
//            }
//            else {
//                if (wcslen(currentInput) > 0) {
//                    lastValue = _wtof(currentInput);
//                    lastOp = label;
//                    currentInput[0] = 0;
//                    UpdateDisplay();
//                }
//            }
//        }
//        break;
//    }
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
