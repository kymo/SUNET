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
	_init();
}

TextStrategy::~TextStrategy() {
}

int TextStrategy::_process(const Request& req, Json::Value& root, const int& level) {
	root["text"] = "yes";
	return SUB_OK;
}

int TextStrategy::_init() {
	
	std::cout << "TextStrategy Init!" << std::endl;
	return SUB_OK;
}

}
