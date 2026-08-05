#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib,"ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to display all files inside uploads folder
void listFiles(char fileList[])
{
    WIN32_FIND_DATA file;
    HANDLE hFind;

    fileList[0]='\0';

int count=1;

    hFind = FindFirstFile("uploads\\*.*", &file);

    if(hFind == INVALID_HANDLE_VALUE)
    {
        strcpy(fileList,"No Files Found\n");
        return;
    }

    do
    {
        if(!(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            char temp[200];

sprintf(temp,"%d. %s\n",
        count++,
        file.cFileName);

strcat(fileList,temp);
        }

    }while(FindNextFile(hFind,&file));

    FindClose(hFind);
}

// Receive a file from client
void receiveFile(SOCKET clientSocket,char filename[])
{
    char path[200];
    char buffer[BUFFER_SIZE];

    sprintf(path,"uploads\\%s",filename);

    FILE *fp=fopen(path,"wb");

    if(fp==NULL)
    {
        printf("Cannot create file.\n");
        return;
    }

    while(1)
    {
        int bytesReceived;

        bytesReceived=recv(clientSocket,
                           buffer,
                           BUFFER_SIZE,
                           0);

        if(bytesReceived<=0)
            break;

        if(strcmp(buffer,"FILE_END")==0)
            break;

        fwrite(buffer,
               1,
               bytesReceived,
               fp);

        if(bytesReceived<BUFFER_SIZE)
            break;
    }

    fclose(fp);

    printf("Upload Successful.\n");
}

// Send a file to client
void sendFile(SOCKET clientSocket,char filename[])
{
    char path[200];
    char buffer[BUFFER_SIZE];

    sprintf(path,"uploads\\%s",filename);

    FILE *fp=fopen(path,"rb");

    if(fp==NULL)
    {
        strcpy(buffer,"FILE_NOT_FOUND");

        send(clientSocket,
             buffer,
             strlen(buffer)+1,
             0);

        return;
    }

    while(1)
    {
        int bytesRead;

        bytesRead=fread(buffer,
                        1,
                        BUFFER_SIZE,
                        fp);

        if(bytesRead<=0)
            break;

        send(clientSocket,
             buffer,
             bytesRead,
             0);

        if(bytesRead<BUFFER_SIZE)
            break;
    }

    strcpy(buffer,"FILE_END");

    send(clientSocket,
         buffer,
         strlen(buffer)+1,
         0);

    fclose(fp);
}

int main()
{
    WSADATA wsa;

    SOCKET serverSocket;
    SOCKET clientSocket;

    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;

    int clientSize=sizeof(clientAddr);

    char message[BUFFER_SIZE];
    char command[50];
    char filename[100];
    char fileList[4096];

    // Initialize Winsock
    if(WSAStartup(MAKEWORD(2,2),&wsa)!=0)
    {
        printf("WSAStartup Failed\n");
        return 1;
    }

    printf("Winsock Initialized.\n");

    // Create Socket
    serverSocket=socket(AF_INET,
                        SOCK_STREAM,
                        0);

    if(serverSocket==INVALID_SOCKET)
    {
        printf("Socket Creation Failed\n");
        return 1;
    }

    printf("Socket Created.\n");

    // Server Details
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(PORT);
    serverAddr.sin_addr.s_addr=INADDR_ANY;

    // Bind
    if(bind(serverSocket,
            (struct sockaddr*)&serverAddr,
            sizeof(serverAddr))
            ==SOCKET_ERROR)
    {
        printf("Bind Failed\n");
        return 1;
    }

    printf("Bind Successful.\n");

    // Listen
if(listen(serverSocket,5)==SOCKET_ERROR)
{
    printf("Listen Failed\n");
    closesocket(serverSocket);
    WSACleanup();
    return 1;
}

printf("Server Listening On Port %d...\n",PORT);

    while(1)
{
    printf("\n=====================================\n");
    printf("Waiting For Client...\n");
    printf("=====================================\n");

    clientSocket = accept(serverSocket,
                          (struct sockaddr*)&clientAddr,
                          &clientSize);

    if(clientSocket==INVALID_SOCKET)
    {
        printf("Accept Failed\n");
        continue;
    }

    printf("\nClient Connected Successfully.\n");

    while(1)
    {
        memset(message,0,sizeof(message));
        memset(command,0,sizeof(command));
        memset(filename,0,sizeof(filename));

       int bytesReceived;

bytesReceived = recv(clientSocket,
                     message,
                     BUFFER_SIZE,
                     0);

if(bytesReceived <= 0)
{
    printf("Client Disconnected.\n");
    break;
}

sscanf(message,"%s",command);
        // -------- LIST --------
        if(strcmp(command,"LIST")==0)
        {
            listFiles(fileList);

            send(clientSocket,
                 fileList,
                 strlen(fileList)+1,
                 0);
        }

        // -------- UPLOAD --------
        else if(strcmp(command,"UPLOAD")==0)
        {
            char ready[]="READY";

            // Tell client to send filename
            send(clientSocket,
                 ready,
                 strlen(ready)+1,
                 0);

            memset(filename,0,sizeof(filename));

// Receive filename from client
int bytesReceived;

bytesReceived = recv(clientSocket,
                     filename,
                     sizeof(filename),
                     0);

if(bytesReceived <= 0)
{
    printf("Failed to receive filename.\n");
    break;
}

printf("\n----------------------------------\n");
printf("UPLOAD REQUEST\n");
printf("Receiving : %s\n", filename);
printf("----------------------------------\n");

// Receive the file
receiveFile(clientSocket, filename);

            char success[]="UPLOAD SUCCESS";

            send(clientSocket,
                 success,
                 strlen(success)+1,
                 0);
        }

        // -------- DOWNLOAD --------
        else if(strcmp(command,"DOWNLOAD")==0)
        {
            char ready[]="READY";

            // Tell client to send filename
            send(clientSocket,
                 ready,
                 strlen(ready)+1,
                 0);

            memset(filename,0,sizeof(filename));

            // Receive filename
            recv(clientSocket,
                 filename,
                 sizeof(filename),
                 0);

            printf("\n----------------------------------\n");
printf("DOWNLOAD REQUEST\n");
printf("Sending : %s\n",filename);
printf("----------------------------------\n");

            // Send file contents
            sendFile(clientSocket,filename);
        }

        // -------- EXIT --------
       else if(strcmp(command,"EXIT")==0)
{
    printf("Client Requested Exit.\n");
    break;
}

        // -------- INVALID COMMAND --------
        else
        {
            char error[] =
"\nInvalid Command!\n\n"
"Available Commands:\n"
"LIST\n"
"UPLOAD\n"
"DOWNLOAD\n"
"EXIT\n";

            send(clientSocket,
                 error,
                 strlen(error)+1,
                 0);
        }

    }

    // Close sockets
            closesocket(clientSocket);

    printf("Client Disconnected.\n");
}

closesocket(serverSocket);

WSACleanup();

return 0;
}
