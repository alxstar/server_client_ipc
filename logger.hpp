#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <string>

#include <fstream>

namespace bip = boost::interprocess;

class Logger
{
public: 
	
	Logger(const std::string& filename="../log.txt"): out_(filename, std::ios_base::app)
		, flk_(filename.c_str())
	{
	}
	
	void log(const std::string& text)
	{
		bip::scoped_lock<bip::file_lock> lk(flk_);
		out_ << header_ << text << std::endl;
	}
private:
	std::ofstream out_;
	
	bip::file_lock flk_;
	const std::string header_ = "SERVER: ";
};

#endif

