#pragma once
#include <vector>
#include "../shared/net.h"

class Client
{
public:
    void start();

private:
    void showStartMenu(Net& net);
    void showLoginMenu(Net& net);
    void showRegistrationMenu(Net& net);
    void showChatMenu(Net& net);

    void showChat(Net& net);
    void addMessage(Net& net);
    void showAllUsers(Net& net);

    User user_;
};