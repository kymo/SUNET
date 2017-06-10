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
    
    std::string q = req.get_query();

    std::vector<std::string> seg_res;
    wordseg->segment(q, seg_res);
    std::string segstr = "";
    for (int i = 0; i < seg_res.size(); i++) {
        segstr += seg_res[i] + "_";
    }
    root["seg"] = segstr;
    return SUB_OK;
}

int TextStrategy::_init() {
	
	std::cout << "TextStrategy Init!" << std::endl;
    wordseg = WordSeg::_get_instance();
    if (NULL == wordseg) {
        return SUB_FAIL;
    }
    std::string crf_model_path = "";
    SubConfig::_get_instance()->_get_conf_val("crf_model", crf_model_path);
    if (! wordseg->_init_tagger(crf_model_path.c_str())) { 
        return SUB_FAIL;
    }

	return SUB_OK;
}

}
