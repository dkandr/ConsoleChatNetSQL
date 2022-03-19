#include "message.h"

Message::Message(const std::string& from, const std::string& to, const std::string& text) 
        : from_(from), to_(to), text_(text) 
{}



const std::string& Message::getFrom() 
{ 
    return from_; 
}



const std::string& Message::getTo() 
{ 
    return to_; 
}



const std::string& Message::getText() 
{ 
    return text_; 
}



std::istream& operator>> (std::istream& is, Message& obj) 
{
    std::getline(is, obj.from_);
    std::getline(is, obj.to_);
    std::getline(is, obj.text_);
    return is;
}



std::ostream& operator<< (std::ostream& os, const Message& obj) {
    os << obj.from_ << '\n';
    os << obj.to_ << '\n';
    os << obj.text_;
    return os;
}