#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main()
{
  // 创建套接字
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
  {
    perror("Failed to create socket");
    return 1;
  }

  // 设置服务器地址和端口
  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(8080);
  if (inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr)) <= 0)
  {
    perror("Invalid address");
    close(sockfd);
    return 1;
  }

  // 连接服务器
  if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
  {
    perror("Failed to connect");
    close(sockfd);
    return 1;
  }

  printf("Connected to server.\n");

  // 接收和发送数据
  char buffer[BUFFER_SIZE];
  while (1)
  {
    memset(buffer, 0, sizeof(buffer));
    int bytesRead = read(sockfd, buffer, sizeof(buffer) - 1);
    if (bytesRead <= 0)
    {
      printf("Connection closed by server.\n");
      break;
    }

    printf("Received: %s\n", buffer);

    if (strcmp(buffer, "ping") == 0)
    {
      char reply[] = "pong";
      write(sockfd, reply, strlen(reply));
      printf("Sent: %s\n", reply);
    }
  }

  // 关闭套接字
  close(sockfd);

  return 0;
}
