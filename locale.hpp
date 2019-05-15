#ifndef LOCALE_HPP
#define LOCALE_HPP

#include <boost/date_time.hpp>
#include <sstream>
#include <locale>
#include <string>
#include <stdexcept>


class Locale
{
public:
	Locale(): input_locale_(std::locale::classic(), 
		new boost::posix_time::time_input_facet("%d.%m.%Y %H:%M:%S"))
		, output_locale_(std::locale::classic(),
			new boost::posix_time::time_facet("%d.%m.%Y %H:%M:%S"))
	{
	}
	
	boost::posix_time::ptime to_date(const std::string& date) const
	{
	    validate_date(date);	
		
		std::istringstream is(date);
		is.exceptions(std::ios_base::failbit);
		is.imbue(input_locale_);
		boost::posix_time::ptime pt;
		is >> pt;
		return pt;
	}

	
	std::string to_string(const boost::posix_time::ptime& pt) const
	{
		std::ostringstream os;
		os.imbue(output_locale_);
		os << pt;
		return os.str();
	}

private:
	
	void validate_date(const std::string& date) const
	{
		if(date.size() != 19) throw std::runtime_error("parsed date has incorrect length");
		int decimal_hour = std::stoi(date.substr(11, 1));
		int hour = std::stoi(date.substr(12, 1));
		int decimal_minute = std::stoi(date.substr(14, 1));
		int decimal_second = std::stoi(date.substr(17, 1));
		if(decimal_hour > 2 || (decimal_hour == 2 && hour > 3) || decimal_minute > 5 || decimal_second > 5)
			throw std::runtime_error("invalid day time range");	
	}
	
	std::locale input_locale_; 
	std::locale output_locale_; 
};

#endif
