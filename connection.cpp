#include "connection.hpp"
#include "csventry.hpp"
#include "csvparser.hpp"
#include "logger.hpp"

#include <boost/asio.hpp>
#include <boost/date_time.hpp>
#include <memory>
#include <string>
#include <tuple>


namespace ba = boost::asio;
namespace bs = boost::system;

Connection::Connection(ba::io_service& io_s, Logger& logger): 
	socket_(io_s), logger_(logger) 
{
}
	
ba::ip::tcp::socket& 
Connection::socket() 
{ 
	return socket_; 
}

void
Connection::process_client_request()
{
	try
	{
		int line_count = read_client_request();
		send_success_response(std::to_string(line_count));
		log();
	}
	catch(bs::system_error& e)
	{ 
		send_failure_response(e.what()); 
		logger_.log(e.what()); 
	}
	catch(std::exception& e)  
	{ 
		send_failure_response(e.what());
		logger_.log(e.what()); 
	}
}

int 
Connection::read_client_request()
{
	int line_count = 0;
	const std::string end_of_input = "\r";
	for(bs::error_code ec; ba::read_until(socket_, buf_, '\n', ec); ++line_count) 
	{	
		if(ec) { throw bs::system_error(ec); }
		
		std::string line = read_line();
		if(line==end_of_input) { break; }
		auto csv_tuple = csv_parser_.parse(line);
		csv_entry_.compare_and_update(csv_tuple);
	}
	return line_count;
}


std::string 
Connection::read_line()
{
	std::string line;
	std::istream input(&buf_);
	std::getline(input, line); 
	return line;
}

void 
Connection::send_response(const std::string& message)
{
	ba::write(socket_, ba::buffer(message + "\n"));
}

void 
Connection::send_success_response(const std::string& message)
{
	std::string header = "number of entries: ";
	send_response(header + message);
}

void 
Connection::send_failure_response(const std::string& message)
{
	std::string header = "failure on server side: ";
	send_response(header + message);
}

void 
Connection::log()
{
	try
	{
		std::string log_info = csv_entry_.get_log_info();
		logger_.log(log_info);
	}
	catch(std::exception& e)
	{
		logger_.log(e.what());
	}
}

