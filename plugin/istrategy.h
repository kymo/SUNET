/**
 * @file istrategy.h
 * @author kymowind@gmail.com
 * @date 2017/02/14 20:17:42
 * @brief abstract class of strategy 
 *  
 **/

#ifndef _ISTRATEGY_H_
#define _ISTRATEGY_H_

#include <iostream>

namespace sub_framework {
	
class IStrategy {

private:
	std::string _strategy_name;  // strategy name
	int _child_stragey_cnt;		 // children strategies count

public:
	IStrategy();
	virtual ~IStrategy();
	virtual int _process(const HttpRequest& req, std::string& ret_json, const int& level) = 0;
	virtual int _init() = 0;

};


}


#endif



