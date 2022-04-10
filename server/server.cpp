#include "server.h"

void Server::start()
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(DEFAULT_PORT);

    Net net {addr};

    std::string dbHost, dbUser, dbPass, dbBase;
    setDatabaseSetting(dbHost, dbUser, dbPass, dbBase);

    Database db {dbHost, dbUser, dbPass, dbBase};

    try
    {

        db.connect();

        db.getUsers(users_);
        db.getMessages(messages_);

        net.Bind();
        net.Listen();

        std::string log_file_name = "log.txt";        
        Logger log {log_file_name};

        //server always works
        while (true)
        {
            //wait client
            net.Accept();

            while(net.isConnect()) 
            {
                Net::PacketType packetType = Net::PacketType::Unknown;
                net.simpleRead<Net::PacketType>(packetType);

                switch (packetType)
                {
                case Net::PacketType::Login :
                    userLogin(net);
                    break;
                case Net::PacketType::Registration :
                    userRegistration(net, db);
                    break;
                case Net::PacketType::GetMessages :
                    sendMessages(net);
                    break;               
                case Net::PacketType::AddMessage :
                    addMessage(net, db, log);
                    break;               
                case Net::PacketType::getNameUsers :
                    sendAllUserNames(net);
                    break;               
                default:
                    break;
                }
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "\033[31m";
        std::cerr << e.what() << std::endl;
        std::cerr << "\033[0m";
    }
}



void Server::setDatabaseSetting(std::string& dbHost, std::string& dbUser, std::string& dbPass, std::string& dbBase)
{
    std::cout << "\nDatabase setting: " << std::endl;
    std::cout << "Host     : ";
    std::cin >> dbHost;
    std::cout << "User     : ";
    std::cin >> dbUser;
    std::cout << "Password : ";
    std::cin >> dbPass;
    std::cout << "Base     : ";
    std::cin >> dbBase;
}



void Server::userLogin(Net& net)
{
    User u;
    net.Read<User>(u);

    std::cout << "(" << net.getConnection() << ")login : "; 

    for(auto &user : users_)
    {
        if(user.getLogin() == u.getLogin() && user.getPassword() == u.getPassword())
        {
            std::cout << "successfully" << std::endl;
            loginUser_ = std::make_unique<User>(user);
            net.simpleWrite<bool>(true);
            net.Write<User>(user);
            return;
        }
    }

    std::cout << "failed" << std::endl;
    net.simpleWrite<bool>(false);
}



void Server::userRegistration(Net& net, Database& db)
{
    User u;
    net.Read<User>(u);

    std::cout << "(" << net.getConnection() << ")registration : "; 

    //can't create user with login or name "all"
    if (u.getLogin() == "all" || u.getName() == "all")
    {      
        std::cout << "failed" << std::endl;
        net.simpleWrite<bool>(false);
        return;
    }

    for(auto &user : users_)
    {
        if(user.getLogin() == u.getLogin())
        {            
            std::cout << "failed" << std::endl;
            net.simpleWrite<bool>(false);
            return;
        }
    }

    users_.push_back(u);
    db.addUser(u);

    std::cout << "successfully" << std::endl;
    
    loginUser_ = std::make_unique<User>(u);
    net.simpleWrite<bool>(true);
}



void Server::sendMessages(Net& net)
{
    std::vector<Message> send_messages;

    for (auto& message : messages_)
    {
        if (message.getFrom() == loginUser_->getName() || message.getTo() == loginUser_->getName() || message.getTo() == "all")
            send_messages.push_back(message);
    }

    net.simpleWrite<size_t>(send_messages.size());

    for (auto& message : send_messages)
    {
        net.Write<Message>(message);
    }
}



void Server::addMessage(Net& net, Database& db, Logger& log)
{
    Message message;
    net.Read<Message>(message);

    messages_.push_back(message);
    db.addMessage(message);

    log.write_message(message);
}

void Server::sendAllUserNames(Net& net)
{
    std::vector<User> send_users;

    for (auto& user : users_)
    {
        send_users.push_back({user.getLogin(),user.getName(),""});
    }

    net.simpleWrite<size_t>(send_users.size());

    for (auto& user : send_users)
    {
        net.Write<User>(user);
    }
}