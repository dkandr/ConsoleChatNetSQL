#include "user.h"

User::User(const std::string &login, const std::string &name, const std::string &password)
        : login_(login), name_(name), password_(password) 
{} 



const std::string& User::getLogin()
{ 
    return login_;
}



void User::setLogin(const std::string& login)
{
    login_ = login;
}




const std::string& User::getName()
{
    return name_; 
}



void User::setName(const std::string& name)
{
    name_ = name;
}



const std::string& User::getPassword() 
{
    return password_;
}



void User::setPassword(const std::string& password)
{
    password_ = password;
}



std::istream& operator>> (std::istream& is, User& obj) 
{
    std::getline(is, obj.login_);
    std::getline(is, obj.name_);
    std::getline(is, obj.password_);
    return is;
}



std::ostream& operator<< (std::ostream& os, const User& obj) 
{
    os << obj.login_ << '\n';
    os << obj.name_ << '\n';
    os << obj.password_;
    return os;
}