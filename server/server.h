#pragma once
#include <vector>
#include <memory>
#include "../shared/net.h"
#include "../shared/logger.h"
#include "database.h"

class Server
{
public:
    void start();

private:
    void setDatabaseSetting(std::string& dbHost, std::string& dbUser, std::string& dbPass, std::string& dbBase);

    void userLogin(Net& net);
    void userRegistration(Net& net, Database& db);

    void sendMessages(Net& net);
    void addMessage(Net& net, Database& db, Logger& log);
    void sendAllUserNames(Net& net);

    std::vector<User> users_;
    std::vector<Message> messages_;
    std::unique_ptr<User> loginUser_;
};