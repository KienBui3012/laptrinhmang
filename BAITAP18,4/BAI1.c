#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)
int main(int argc, char * argv[])
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);
    system("pause");
    connect(client, (SOCKADDR*)&addr, sizeof(addr));
    // Buffer chua du lieu can gui
    char sendBuf[256];
    int pos = 0;
    // Doc ten cua may tinh va copy vao buffer
    char computerName[256];
    DWORD nameLen;
    GetComputerNameA(computerName, &nameLen);
    strcpy(sendBuf, computerName);
    pos = nameLen + 1;
    // Doc danh sach o dia va copy vao buffer
    char driveString[256];
    DWORD len = GetLogicalDriveStringsA(sizeof(driveString), driveString);
    int numDrivers = len / 4;
    DWORD spc, bps, freeClusters, totalClusters;
    for (int i = 0; i < numDrivers; i++)
    {
        // Doc kich thuoc o dia
        GetDiskFreeSpaceA(&driveString[i * 4], &spc, &bps, &freeClusters, &totalClusters);
        float diskSize = (float)spc * (float)bps * (float)totalClusters / (float)(1 << 30);
        // Copy vao xau sendBuf
        sendBuf[pos] = driveString[i * 4]; // Ky tu o dia - 1 byte
        pos = pos + 1;
        memcpy(sendBuf + pos, &diskSize, 4); // Kich thuoc o dia - 4 bytes
        pos = pos + 4;
    }

    send(client, sendBuf, pos, 0);
    closesocket(client);
    WSACleanup();
}
