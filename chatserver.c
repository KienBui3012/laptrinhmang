#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)
void RemoveClient(SOCKET* clients, int* numClients, int i)
{
    // Xoa client khoi mang
    if (i < *numClients - 1)
        clients[i] = clients[*numClients - 1];
    *numClients--;
}
int main()
{
    // Khoi tao thu vien
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    // Tao socket
    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Khai bao dia chi server
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);
    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    listen(listener, 5);
    fd_set fdread;
    int ret;
    SOCKET clients[64];
    int numClients = 0;
    char buf[256];
    while (1)
    {
        FD_ZERO(&fdread);
        FD_SET(listener, &fdread);
        for (int i = 0; i < numClients; i++)
            FD_SET(clients[i], &fdread);
        ret = select(0, &fdread, 0, 0, 0);
        if (ret > 0)
        {
            // Kiem tra su kien ket noi moi
            if (FD_ISSET(listener, &fdread))
            {
                SOCKET client = accept(listener, NULL, NULL);
                printf("Ket noi moi: %d\n", client);
                const char* msg = "Hello client\n";
                send(client, msg, strlen(msg), 0);
                clients[numClients] = client;
                numClients++;
            }
            for (int i = 0; i < numClients; i++)
            {
                if (FD_ISSET(clients[i], &fdread))
                {
                    ret = recv(clients[i], buf, sizeof(buf), 0);
                    if (ret <= 0)
                    {
                        // Xoa client ra khoi mang clients
                        RemoveClient(clients, &numClients, i);
                        i--;
                        continue;
                    }
                    buf[ret] = 0;
                    printf("Du lieu nhan duoc tu %d: %s\n", clients[i], buf);
                }
            }
        }
    }
}
