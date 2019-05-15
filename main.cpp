#include "logger.hpp"
#include "server.hpp"

#include <boost/asio.hpp>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		std::cerr << "Usage: server <port>\n";
		return -1;
	}
	
	Logger logger;
	try
	{
		std::string ip = "127.0.0.1";
		std::size_t port = std::atoi(argv[1]);
		boost::asio::io_service io_s;
		Server server(io_s, ip, port, logger);
	}
	catch(std::exception& e) { logger.log(e.what());            }
	catch(...) 				 { logger.log("unknown exception"); }
}

