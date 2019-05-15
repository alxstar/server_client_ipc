#ifndef CSVPARSER_HPP
#define CSVPARSER_HPP

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <string>
#include <tuple>
#include <stdexcept>

template<typename Iterator, typename Skipper>
struct Grammar: boost::spirit::qi::grammar
<
	Iterator, 
	std::tuple<std::string, double, double>(), 
	Skipper
>
{
	Grammar(): Grammar::base_type(t_)
	{
		t_ = boost::spirit::qi::lexeme
		[	
			boost::spirit::qi::as_string
			[
				boost::spirit::qi::repeat(2)
				[
					boost::spirit::qi::repeat(2)[boost::spirit::qi::digit] >> boost::spirit::qi::char_('.')  
				] 
				>> boost::spirit::qi::repeat(4)[boost::spirit::qi::digit] >>
				
				boost::spirit::qi::char_(" ") >>
				
				boost::spirit::qi::repeat(2)
				[
					boost::spirit::qi::repeat(2)[boost::spirit::qi::digit] >> boost::spirit::qi::char_(':')  
				]
				>> 
				boost::spirit::qi::repeat(2)[boost::spirit::qi::digit] 
			]
		] >> ',' >> boost::spirit::qi::double_ >> ',' >> boost::spirit::qi::double_;
	}
private:	
	boost::spirit::qi::rule<Iterator, std::tuple<std::string, double, double>(), Skipper> t_;
};

class Csvparser
{
public:
	std::tuple<std::string, double, double> parse(const std::string& csv_line) const
	{
		std::tuple<std::string, double, double> tpl;
		auto begin = csv_line.begin();
		auto end = csv_line.end();
		bool b = boost::spirit::qi::phrase_parse(begin, end, g_, boost::spirit::qi::space, tpl);
		if(b && begin == end)
		{
			return tpl;
		}
		throw std::runtime_error("failed to parse csv line");
	}

private:
	const Grammar<std::string::const_iterator, boost::spirit::qi::space_type> g_;	
};

#endif
