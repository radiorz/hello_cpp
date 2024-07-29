#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

const int PORT = 8080;

int main()
{
  // 创建套接字
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1)
  {
    std::cerr << "Failed to create socket." << std::endl;
    return -1;
  }

  // 准备服务器地址
  sockaddr_in serverAddress{};
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(PORT);

  // 绑定套接字到指定地址和端口
  if (bind(serverSocket, reinterpret_cast<struct sockaddr *>(&serverAddress), sizeof(serverAddress)) < 0)
  {
    std::cerr << "Failed to bind socket to address." << std::endl;
    close(serverSocket);
    return -1;
  }

  // 监听连接请求
  if (listen(serverSocket, 1) < 0)
  {
    std::cerr << "Failed to listen for connections." << std::endl;
    close(serverSocket);
    return -1;
  }

  std::cout << "Server listening on port " << PORT << std::endl;

  while (true)
  {
    // 接受客户端连接
    sockaddr_in clientAddress{};
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr *>(&clientAddress), &clientAddressLength);
    if (clientSocket < 0)
    {
      std::cerr << "Failed to accept client connection." << std::endl;
      close(serverSocket);
      return -1;
    }


    

    std::cout << "Client connected." << std::endl;

    // 接收和处理数据
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead < 0)
    {
      std::cerr << "Failed to receive data from client." << std::endl;
      close(clientSocket);
      close(serverSocket);
      return -1;
    }

    std::string request(buffer);
    std::cout << "Received message: " << request << std::endl;

    std::string response;
    if (request == "ping")
    {
      response = "pong";
    }
    else
    {
      response = "Unknown command";
    }

    // 发送回复
    ssize_t bytesSent = send(clientSocket, response.c_str(), response.length(), 0);
    if (bytesSent < 0)
    {
      std::cerr << "Failed to send response to client." << std::endl;
      close(clientSocket);
      close(serverSocket);
      return -1;
    }

    // 关闭客户端连接
    close(clientSocket);
    std::cout << "Client disconnected." << std::endl;
  }

  // 关闭服务器套接字
  close(serverSocket);
  return 0;
}
