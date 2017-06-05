/**
 * @file text_strategy.h
 * @author kymowind@gmail.com
 * @date 2017/02/14 20:17:42
 * @brief abstract class of strategy 
 *  
 **/

#ifndef _TEXT_STRTEGY_H_
#define _TEXT_STRTEGY_H_

namespace sub_framework {
	
class TextStrategy : IStrategy {
// wordseg , normalization, and number normalized
public:
	TextStrategy();
	virtual ~TextStrategy() {
	}

	int _process(const HttpRequest& req, std::string& ret_json, const int& level);
	int _init();


};

}

#endif

