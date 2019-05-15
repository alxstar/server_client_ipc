#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include <thread>
#include <vector>
#include <string>

#include "connection.hpp"
#include "logger.hpp"

class Server
{
public:
	explicit Server(boost::asio::io_service& io_s, const std::string& ip, std::size_t port, Logger& logger); 
	~Server();
private:
	void listen();
	void launch_threadpool(boost::asio::io_service& io_s);
	void set_thread_affinity(std::thread& th, std::size_t i);
	
	boost::asio::ip::tcp::acceptor acceptor_;
	std::size_t threadcount_;
	std::vector<std::thread> threadpool_;
	Logger& logger_;
};
#endif

