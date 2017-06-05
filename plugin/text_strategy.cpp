/**
 * @file wordseg_strategy.h
 * @author kymowind@gmail.com
 * @date 2017/02/14 20:17:42
 * @brief abstract class of strategy 
 *  
 **/

#include "text_strategy.h"

namespace sub_framework {

TextStrategy::TextStrategy() {
}

TextStrategy::~TextStrategy() {
}

int TextStrategy::_process(const HttpRequest& req, std::string& ret_json, const int& level) {
	ret_json += "{\"ws\":\"你好\001我好\"}";
	return SUB_OK;

}

int TextStrategy::_init() {
	
	std::cout << "TextStrategy Init!" << std::endl;
	return SUB_OK;
}

}
