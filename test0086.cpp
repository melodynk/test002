#include <windows.h>
#include <iostream>
#include <string>

int main() {
    // 串口名
//    LPCSTR portName = "\\\\.\\COM6";
	    // 串口名（宽字符串）
    LPCWSTR portName = L"\\\\.\\COM6";

    // 打开串口
    HANDLE hSerial = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Unable to open COM port." << std::endl;
        return 1;
    }

    // 配置串口参数
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error: Unable to get COM port state." << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = 15200; // 波特率
    dcbSerialParams.ByteSize = 8;    // 数据位
    dcbSerialParams.StopBits = ONESTOPBIT; // 停止位
    dcbSerialParams.Parity = NOPARITY;     // 无校验位

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error: Unable to configure COM port." << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    // 设置超时时间
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cerr << "Error: Unable to set COM port timeouts." << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    std::cout << "COM6 opened successfully. Waiting for data..." << std::endl;

    // 读取数据
    char buffer[128];  // 读取缓冲区
    DWORD bytesRead;   // 实际读取的字节数

    while (true) {
        if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0'; // 确保缓冲区以空字符结束
                std::cout << "Received: " << buffer << std::endl;
            }
        } else {
            std::cerr << "Error: Unable to read from COM port." << std::endl;
            break;
        }
    }

    // 关闭串口
    CloseHandle(hSerial);
    return 0;
}
