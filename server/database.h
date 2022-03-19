#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <mysql.h>
#include "../shared/user.h"
#include "../shared/message.h"

class Database
{
public:
    Database(const std::string& serverName, const std::string& userName, const std::string& userPassword, const std::string& baseName);
    ~Database();

    void connect();
    void getUsers(std::vector<User>& users);
    void getMessages(std::vector<Message>& messages);
    void addUser(User& user);
    void addMessage(Message& message);

private:
    void query_to_db(std::string& query);

    MYSQL mysql_; // Дескриптор соединения c MySql
	MYSQL_RES* res_;
	MYSQL_ROW row_;
    
    std::string serverName_;
    std::string userName_;
    std::string userPassword_;
    std::string baseName_;
};