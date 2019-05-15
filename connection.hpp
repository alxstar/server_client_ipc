#ifndef CONNECTION_HPP
#define CONNECTION_HPP 

#include "csvparser.hpp"
#include "csventry.hpp"
#include "logger.hpp"

#include <boost/asio.hpp>
#include <memory>
#include <string>

class Connection : public std::enable_shared_from_this<Connection>
{
public:
	explicit Connection(boost::asio::io_service& io_s, Logger& logger);	
	boost::asio::ip::tcp::socket& socket(); 
	void process_client_request();
private:
	int read_client_request();
	std::string read_line();
	void send_response(const std::string& message);
	void send_success_response(const std::string& message);
	void send_failure_response(const std::string& message);
	void log();
	
	boost::asio::ip::tcp::socket socket_;
	Logger& logger_;
	Csvparser csv_parser_;
	Csventry csv_entry_;
	boost::asio::streambuf buf_;
};
#endif

