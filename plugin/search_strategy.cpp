/**
 * @file wordseg_strategy.h
 * @author kymowind@gmail.com
 * @date 2017/02/14 20:17:42
 * @brief abstract class of strategy 
 *  
 **/

#include "search_strategy.h"
#include "sub_log.h"
namespace sub_framework {

SearchStrategy::SearchStrategy() {
    _init();
}

SearchStrategy::~SearchStrategy() {
}

int SearchStrategy::_process(const Request& req, Json::Value& root, const int& level) {
    std::string q = req.get_query();
    std::vector<std::string> seg_res;
    DEBUG_LOG("Search Strategy %s", q.c_str());
	std::cout << "Search Strategy" << std::endl;
	Json::Value& jarray = root["seg"];
	if (jarray.size() == 0) {
		root["search"]["msg"] = "none";
		return SUB_OK;
	}
	Json::Value result;
	for (int i = 0; i < jarray.size(); i++) {	
		std::vector<reverse_index> search_results;
		search_results = _dat->_reversed_search(jarray[i].asString());
		for (int j = 0; j < search_results.size(); j++) {
			result.append(search_results[j].doc_id);	
		}
	}
	if (result.size() == 0) {
		root["search"]["msg"] = "none";
		return SUB_OK;
	}

	root["search"]["msg"] = "ok";
	root["search"]["result"] = result;
	return SUB_OK;
}

int SearchStrategy::_init() {
	_dat = new DatImpl<std::vector<reverse_index> >();
	std::string index_file_path = "";
	SubConfig::_get_instance()->_get_conf_val("index_file", index_file_path);
	if (! _dat->_load_index_dict(index_file_path.c_str())) {
		FATAL_LOG("Init Double Array Trie Error!");
		return SUB_FAIL;
	}
	_dat->_build();
	return SUB_OK;
}

}
