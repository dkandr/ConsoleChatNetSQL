#include "client.h"

void Client::start()
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");;
    addr.sin_port = htons(DEFAULT_PORT);

    Net net = Net(addr);

    try
    {
        net.Connect();

        showStartMenu(net);
    }
    catch(const std::exception& e)
    {
        std::cerr << "\033[31m";
        std::cerr << e.what() << std::endl;
        std::cerr << "\033[0m";
    }
}



void Client::showStartMenu(Net& net)
{
    char choice;
    do
    {
        std::cout << "\033[1;34m";
        std::cout << "----------------" << std::endl;
        std::cout << "(1) Login" << std::endl;
        std::cout << "(2) Registration" << std::endl;
        std::cout << "(q) Exit" << std::endl;
        std::cout << "\033[0;36m";
        std::cout << ">> ";
        std::cin >> choice;
        std::cout << "\033[0m";

        switch (choice)
        {
        case '1':
            showLoginMenu(net);
            break;
        case '2':
            showRegistrationMenu(net);
            break;        
        default:
            break;
        }
    } while (choice != 'q');
}



void Client::showLoginMenu(Net& net)
{
    std::string login, pass;
    std::cout << "\033[34m"<< "----------------" << std::endl;
    std::cout << "Login    : " << "\033[36m";
    std::cin.ignore();
    getline(std::cin, login);
    std::cout << "\033[34m" << "Password : " << "\033[36m";
    getline(std::cin, pass);
    std::cout << "\033[0m";

    user_.setLogin(login);
    user_.setPassword(pass);

    net.simpleWrite<Net::PacketType>(Net::PacketType::Login);
    net.Write<User>(user_);

    bool isLoginSuccessfully = false;
    net.simpleRead<bool>(isLoginSuccessfully);

    if(isLoginSuccessfully)
    {
        net.Read<User>(user_);

        showChatMenu(net);
    }
    else
    {
        std::cout << "\033[31m" << "Login failed" << "\033[0m" << std::endl;
    }
}



void Client::showRegistrationMenu(Net& net)
{
    std::string login,  name, pass;
    std::cout << "\033[34m"<< "----------------" << std::endl;
    std::cout << "Login    : " << "\033[36m";
    std::cin.ignore();
    getline(std::cin, login);
    std::cout << "\033[34m" << "Name     : " << "\033[36m";
    getline(std::cin, name);
    std::cout << "\033[34m" << "Password : " << "\033[36m";
    getline(std::cin, pass);
    std::cout << "\033[0m";

    user_.setLogin(login);
    user_.setName(name);
    user_.setPassword(pass);

    net.simpleWrite<Net::PacketType>(Net::PacketType::Registration);
    net.Write<User>(user_);

    bool isRegistrationSuccessfully = false;
    net.simpleRead<bool>(isRegistrationSuccessfully);

    if(isRegistrationSuccessfully)
    {
        showChatMenu(net);
    }
    else
    {
        std::cout << "\033[31m" << "Registration failed" << "\033[0m" << std::endl;
    }
}



void Client::showChatMenu(Net& net)
{
    std::cout << "\033[32m" << "Hi, " << user_.getName() << "!" << "\033[0m" << std::endl;

    char choice;
    do
    {
        std::cout << "\033[32m" << "Menu: (1)Show chat | (2)Add message | (3)Users | (q)Logout" << std::endl;
        std::cout << "\033[36m";
        std::cout << ">> ";
        std::cin >> choice;
        std::cout << "\033[0m";

        switch (choice)
        {
        case '1':
            showChat(net);
            break;
        case '2':
            addMessage(net);
            break;
        case '3':
            showAllUsers(net);
            break;       
        default:
            break;
        }
    } while (choice != 'q');
}

void Client::showChat(Net& net)
{
    net.simpleWrite<Net::PacketType>(Net::PacketType::GetMessages);

    size_t count;
    net.simpleRead<size_t>(count);

    for (size_t i = 0; i < count; ++i)
    {
        Message message;
        net.Read<Message>(message);

        std::cout << "\033[33m" << "-(" << i + 1 << ")--------------------------" << std::endl;
        std::cout << "From : " << message.getFrom() << std::endl;
        std::cout << "To   : " << message.getTo() << std::endl;
        std::cout << "Text : " << message.getText() << std::endl;
        std::cout << "\033[0m";
    }

}

void Client::addMessage(Net& net)
{
    std::string to, text;

    std::cout << "\033[33m" << "To (name or all): ";
    std::cin.ignore();
    getline(std::cin, to);
    std::cout << "Text: ";
    getline(std::cin, text);
    std::cout << "\033[0m";

    Message message = {user_.getName(), to, text};

    net.simpleWrite<Net::PacketType>(Net::PacketType::AddMessage);
    net.Write<Message>(message);
}

void Client::showAllUsers(Net& net)
{
    net.simpleWrite<Net::PacketType>(Net::PacketType::getNameUsers);

    size_t count;
    net.simpleRead<size_t>(count);
    std::cout << count << std::endl;
    std::cout << "\033[33m" << "------------------------------" << std::endl;
    for (size_t i = 0; i < count; ++i)
    {
        User user;
        net.Read<User>(user);

        std::cout << user.getName() << std::endl;
    }
    std::cout << "\033[0m";
}