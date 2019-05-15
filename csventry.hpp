#ifndef CSVENTRY_HPP
#define CSVENTRY_HPP

#include "locale.hpp"
#include <boost/date_time.hpp>
#include <tuple>
#include <string>
#include <iostream>

class Csventry
{
public:
	
	void compare_and_update(const std::tuple<std::string, double, double>& csv_tuple)
	{
		auto date = locale_.to_date(std::get<0>(csv_tuple));
		if(date > max_date_) 
		{
			max_date_ = date;
			numerator_ = std::get<1>(csv_tuple);
			denominator_ = std::get<2>(csv_tuple);
		}
	}
	
	std::string get_log_info()
	{
		if(denominator_ == 0.0) throw std::runtime_error("denominator is zero!");
		std::string division_result = std::to_string(numerator_/denominator_);
		std::string max_date = locale_.to_string(max_date_);
		return std::string("date: ") + max_date + ", division result: " + division_result;
	}

private:
	
	Locale locale_;
	boost::posix_time::ptime max_date_{boost::gregorian::date{1400,1,1}, boost::posix_time::time_duration{0,0,0}};
	
	double numerator_ = std::numeric_limits<double>::min(); 
	double denominator_ = std::numeric_limits<double>::min();
};

#endif
