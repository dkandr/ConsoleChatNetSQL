#pragma once
#include <fstream>
#include <shared_mutex>
#include "message.h"

class Logger
{
public:
    Logger(std::string& file_name);
    ~Logger();

    void write_message(const Message&);
    void read_last_message(Message&);

private:
    std::fstream fs_;
    std::shared_mutex shared_mutex_;
};