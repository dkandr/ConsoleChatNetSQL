#include "logger.h"

Logger::Logger(std::string& file_name)
{
    fs_.open(file_name, std::ios::app);

    if (fs_.fail())
        throw std::runtime_error("can't open log file");
}



Logger::~Logger()
{
    fs_.close();
}



void Logger::write_message(const Message& message)
{
    shared_mutex_.lock();

    fs_ << message << std::endl;

    shared_mutex_.unlock();
}



void Logger::read_last_message(Message& message)
{
    shared_mutex_.lock_shared();
    
    while (!fs_.eof())
    {
        fs_ >> message; 
    }

    shared_mutex_.unlock_shared();
}