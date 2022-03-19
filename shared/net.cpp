#include "net.h"

Net::Net(sockaddr_in& addr) : addr_(addr)
{
    addr_length_ = sizeof(addr_);
    socket_file_description_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_description_ == -1)
        throw std::runtime_error(std::strerror(errno));

    std::cout << "socket(" << socket_file_description_ << ") open" << std::endl;
}



int Net::getConnection()
{
    return connection_;
}



bool Net::isConnect()
{
    return isConnect_;
}



Net::~Net()
{    
    std::cout << "socket(" << socket_file_description_ << ") closed" << std::endl;
    close(socket_file_description_);
}



void Net::closeConnection()
{
    std::cout << "connection(" << connection_ << ") closed" << std::endl;
    close(connection_);
    isConnect_ = false;
}



void Net::Bind()
{
    int result = bind(socket_file_description_, (struct sockaddr*)&addr_, addr_length_);
    if (result == -1)
        throw std::runtime_error(std::strerror(errno));
}



void Net::Listen()
{
    int result = listen(socket_file_description_, SOMAXCONN);
    if (result == -1)
        throw std::runtime_error(std::strerror(errno));
}



void Net::Accept()
{
    std::cout << "Wait client..." << std::endl;
    connection_ = accept(socket_file_description_, (struct sockaddr*)&addr_, &addr_length_);
    if (connection_ == -1)
        throw std::runtime_error(std::strerror(errno));
    
    isConnect_ = true;
    std::cout << "connection(" << connection_ << ") open" << std::endl;
}



void Net::Connect()
{
    int result = connect(socket_file_description_, (struct sockaddr*)&addr_, addr_length_);
    if (result == -1)
        throw std::runtime_error(std::strerror(errno));
    
    isConnect_ = true;
    connection_ = socket_file_description_;
    std::cout << "connection(" << connection_ << ") open" << std::endl;
}



template void Net::Write<User>(const User&);
template void Net::Write<Message>(const Message&);
template <typename T> void Net::Write(const T& t)
{
    std::stringstream ss;

    ss << t;

    size_t data_size = ss.str().size();

    //write size packet with class
    simpleWrite<size_t>(data_size);

    ssize_t bytes = write(connection_, ss.str().c_str(), data_size);
    if (bytes == -1)
        throw std::runtime_error(std::strerror(errno));
}



template void Net::Read<User>(User&);
template void Net::Read<Message>(Message&);
template <typename T> void Net::Read(T& t)
{
    std::stringstream ss;
   
    size_t data_size;
    
    //read size packet with class
    simpleRead<size_t>(data_size);

    char *buffer = new char[data_size];
 
    ssize_t bytes = read(connection_, buffer, data_size);
    if (bytes == -1)
        throw std::runtime_error(std::strerror(errno));

    //connection closed
    if (bytes == 0)
        closeConnection();

    ss.write(buffer, data_size);

    ss >> t;

    delete[] buffer;
}



template void Net::simpleWrite<bool>(bool);
template void Net::simpleWrite<size_t>(size_t);
template void Net::simpleWrite<Net::PacketType>(Net::PacketType);
template <typename T> void Net::simpleWrite(T t)
{
    ssize_t bytes = write(connection_, (char*)&t, sizeof(t));
    if (bytes == -1)
        throw std::runtime_error(std::strerror(errno));
}



template void Net::simpleRead<bool>(bool&);
template void Net::simpleRead<size_t>(size_t&);
template void Net::simpleRead<Net::PacketType>(Net::PacketType&);
template <typename T> void Net::simpleRead(T& t)
{
    ssize_t bytes = read(connection_, (char*)&t, sizeof(t));
    if (bytes == -1)
        throw std::runtime_error(std::strerror(errno));

    //connection closed
    if (bytes == 0)
        closeConnection();
}