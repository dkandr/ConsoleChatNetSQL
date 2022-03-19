#include "database.h"

Database::Database(const std::string& serverName, const std::string& userName, const std::string& userPassword, const std::string& baseName)
    : serverName_(serverName), userName_(userName), userPassword_(userPassword), baseName_(baseName)
{
    mysql_init(&mysql_);
}



Database::~Database()
{
    mysql_close(&mysql_);
}



void Database::query_to_db(std::string& query)
{
    int res = mysql_query(&mysql_, query.c_str());

    if (res)
    {
        throw std::runtime_error(mysql_error(&mysql_));
    }
}



void Database::connect()
{
	if (&mysql_ == NULL)
    {
		// Если дескриптор не получен — выводим сообщение об ошибке
        throw "Error: can't create MySQL-descriptor";
	}

	// Подключаемся к серверу
	if (!mysql_real_connect(&mysql_, serverName_.c_str(), userName_.c_str(), userPassword_.c_str(), "", 0, NULL, 0))
    {
		// Если нет возможности установить соединение с БД выводим сообщение об ошибке
        throw std::runtime_error(mysql_error(&mysql_));
	}

    mysql_set_character_set(&mysql_, "utf8mb4");

    std::string query;

    query = "CREATE DATABASE IF NOT EXISTS " + baseName_;
    query_to_db(query);

    query = "USE " + baseName_;
    query_to_db(query);

    query = "CREATE TABLE IF NOT EXISTS users(login_ VARCHAR(255), name_ VARCHAR(255), password_ VARCHAR(255))";
    query_to_db(query);

    query = "CREATE TABLE IF NOT EXISTS messages(from_ VARCHAR(255), to_ VARCHAR(255), text_ TEXT)";
    query_to_db(query);

    std::cout << "Database connent successfully" << std::endl;
}



void Database::getUsers(std::vector<User>& users)
{
    std::string query = "SELECT * from users";

    mysql_query(&mysql_, query.c_str());

    if ((res_ = mysql_store_result(&mysql_)))
    {
		while ((row_ = mysql_fetch_row(res_)))
        {
            users.push_back({row_[0], row_[1], row_[2]});
		}
	}
	else
		throw std::runtime_error(mysql_error(&mysql_));
}



void Database::getMessages(std::vector<Message>& messages)
{
    std::string query = "SELECT * from messages";

    mysql_query(&mysql_, query.c_str());

    if ((res_ = mysql_store_result(&mysql_)))
    {
		while ((row_ = mysql_fetch_row(res_)))
        {
            messages.push_back({row_[0], row_[1], row_[2]});
		}
	}
	else
		throw std::runtime_error(mysql_error(&mysql_));
}



void Database::addUser(User& user)
{
    std::string query;
    query = "INSERT INTO users(login_, name_, password_) values('" + user.getLogin() + "', '" + user.getName() + "', '" + user.getPassword() + "')";
    query_to_db(query);
}



void Database::addMessage(Message& message)
{
    std::string query;
    query = "INSERT INTO messages(from_, to_, text_) values('" + message.getFrom() + "', '" + message.getTo() + "', '" + message.getText() + "')";
    query_to_db(query);
}