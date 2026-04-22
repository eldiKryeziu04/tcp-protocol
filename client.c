#include <stdio.h>
#include <winsock2.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    char server_ip[20];
    printf("Vendosni IP adresen e serverit: ");
    scanf("%s", server_ip);
    getchar();

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1200);
    addr.sin_addr.s_addr = inet_addr(server_ip);

    printf("Duke u lidhur me %s...\n", server_ip);
    if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        printf("Gabim: Nuk u lidh dot me serverin ne ate IP!\n");
        WSACleanup();
        return 1;
    }
    

    char welcomeMsg[512] = {0};
    int r = recv(s, welcomeMsg, sizeof(welcomeMsg) - 1, 0);
    if (r > 0) {
        printf("%s\n", welcomeMsg);
        fflush(stdout);
    }

    while (1) {
        char cmd[100];
        printf("Komandat:\n/read <filename> \n/list\n/info <filename>\n/download <filename>\n/upload <filename>\n/search <keyword>\n/delete <filename>\nkomanda:");
        fflush(stdout);
        
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strcspn(cmd, "\n")] = 0; 

        if (strlen(cmd) == 0) continue; 

        send(s, cmd, (int)strlen(cmd), 0);

        if (strcmp(cmd, "/exit") == 0) break;

        char response[2048] = { 0 };
        int n = recv(s, response, sizeof(response) - 1, 0);
        
        if (n > 0) {
            response[n] = '\0';
            printf("PERGJIGJA:\n%s\n", response);
            fflush(stdout);
        } else {
            printf("\nLidhja u keput nga serveri.\n");
            break;
        }
    }

    closesocket(s);
    WSACleanup();
    printf("Programi u mbyll. Shtyp Enter...");
    getchar();
    return 0;
}