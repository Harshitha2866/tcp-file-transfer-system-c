#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    WSADATA wsa;

    SOCKET clientSocket;

    struct sockaddr_in serverAddr;

    char command[50];
    char filename[100];
    char buffer[BUFFER_SIZE];
    char path[200];

    // Initialize Winsock
    if(WSAStartup(MAKEWORD(2,2),&wsa)!=0)
    {
        printf("WSAStartup Failed\n");
        return 1;
    }

    printf("Winsock Initialized.\n");

    // Create Socket
    clientSocket=socket(AF_INET,
                        SOCK_STREAM,
                        0);

    if(clientSocket==INVALID_SOCKET)
    {
        printf("Socket Creation Failed\n");
        return 1;
    }

    printf("Socket Created.\n");

    // Server Details
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(PORT);
    serverAddr.sin_addr.s_addr=inet_addr("127.0.0.1");

    // Connect
    if(connect(clientSocket,
               (struct sockaddr*)&serverAddr,
               sizeof(serverAddr))
               ==SOCKET_ERROR)
    {
        printf("Connection Failed\n");
        return 1;
    }

    printf("Connected Successfully.\n");

    while(1)
    {
        printf("\n");
printf("=========================================\n");
printf("      FILE TRANSFER SYSTEM\n");
printf("=========================================\n");
printf("1. LIST\n");
printf("2. UPLOAD\n");
printf("3. DOWNLOAD\n");
printf("4. EXIT\n");
printf("=========================================\n");
printf("Enter Command : ");

        fgets(command,
              sizeof(command),
              stdin);

        command[strcspn(command,"\n")]='\0';

        send(clientSocket,
             command,
             strlen(command)+1,
             0);

        // -------- LIST --------
        if(strcmp(command,"LIST")==0)
        {
            memset(buffer,0,sizeof(buffer));

            recv(clientSocket,
                 buffer,
                 BUFFER_SIZE,
                 0);

            printf("\n==============================\n");
printf("Files Available\n");
printf("==============================\n");
printf("%s",buffer);
printf("==============================\n");
        }

        // -------- UPLOAD --------
        else if(strcmp(command,"UPLOAD")==0)
        {
            // Receive READY from server
            recv(clientSocket,
                 buffer,
                 BUFFER_SIZE,
                 0);

            printf("Enter Filename : ");

            fgets(filename,
                  sizeof(filename),
                  stdin);

            filename[strcspn(filename,"\n")] = '\0';

            // Send filename
            send(clientSocket,
                 filename,
                 strlen(filename)+1,
                 0);

            sprintf(path,
                    "client_files\\%s",
                    filename);

            FILE *fp = fopen(path,"rb");

            if(fp == NULL)
            {
                printf("File Not Found!\n");
                continue;
            }

            while(1)
            {
                int bytesRead;

                bytesRead = fread(buffer,
                                  1,
                                  BUFFER_SIZE,
                                  fp);

                if(bytesRead <= 0)
                    break;

                send(clientSocket,
                     buffer,
                     bytesRead,
                     0);

                if(bytesRead < BUFFER_SIZE)
                    break;
            }

            strcpy(buffer,"FILE_END");

            send(clientSocket,
                 buffer,
                 strlen(buffer)+1,
                 0);

            fclose(fp);

            memset(buffer,0,sizeof(buffer));

            recv(clientSocket,
                 buffer,
                 BUFFER_SIZE,
                 0);

            printf("%s\n",buffer);
        }

        // -------- DOWNLOAD --------
        else if(strcmp(command,"DOWNLOAD")==0)
        {
            // Receive READY from server
            recv(clientSocket,
                 buffer,
                 BUFFER_SIZE,
                 0);

            printf("Enter Filename : ");

            fgets(filename,
                  sizeof(filename),
                  stdin);

            filename[strcspn(filename,"\n")] = '\0';

            // Send filename
            send(clientSocket,
                 filename,
                 strlen(filename)+1,
                 0);

            sprintf(path,
                    "client_files\\%s",
                    filename);

            FILE *fp = fopen(path,"wb");

            if(fp == NULL)
            {
                printf("Cannot Create File!\n");
                continue;
            }

            while(1)
            {
                int bytesReceived;

                bytesReceived = recv(clientSocket,
                                     buffer,
                                     BUFFER_SIZE,
                                     0);

                if(strcmp(buffer,"FILE_NOT_FOUND")==0)
                {
                    printf("File Not Found On Server.\n");
                    break;
                }

                if(strcmp(buffer,"FILE_END")==0)
                {
                    printf("Download Complete.\n");
                    break;
                }

                fwrite(buffer,
                       1,
                       bytesReceived,
                       fp);

                if(bytesReceived < BUFFER_SIZE)
                    break;
            }

            fclose(fp);
        }

        // -------- EXIT --------
        else if(strcmp(command,"EXIT")==0)
        {
           printf("\n=====================================\n");
printf("Thank You For Using\n");
printf("File Transfer System\n");
printf("=====================================\n");
            break;
        }

        // -------- UNKNOWN COMMAND --------
        else
        {
            memset(buffer,0,sizeof(buffer));

            recv(clientSocket,
                 buffer,
                 BUFFER_SIZE,
                 0);

            printf("%s\n",buffer);
        }
    }

    // Close socket
    closesocket(clientSocket);

    // Cleanup Winsock
    WSACleanup();

    return 0;
}