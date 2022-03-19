#pragma once
#include <iostream>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "user.h"
#include "message.h"

#define DEFAULT_PORT 34567

class Net
{
public:
    Net(sockaddr_in& addr);
    ~Net();

    int getConnection();

    bool isConnect();

    //обертки для стандартных сетевых функций
    //server
    void Bind();
    void Listen();
    void Accept();
    //client
    void Connect();
    
    //for User, Message
    template <typename T> void Write(const T& t);
    template <typename T> void Read(T& t);

    //for bool, size_t, ...
    template <typename T> void simpleWrite(T);
    template <typename T> void simpleRead(T&);

    void closeConnection();

    enum PacketType
    {
        Login = 1,
        Registration,
        AddMessage,
        GetMessages,
        getNameUsers,
        Exit,
        Unknown
    };

private:
    sockaddr_in addr_;
    socklen_t addr_length_;
    int socket_file_description_;
    int connection_;
    bool isConnect_;

//    void Write(const std::stringstream& ss);
//    void Read(std::stringstream& ss);
};