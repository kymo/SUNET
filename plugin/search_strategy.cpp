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
	int page = req.get_page();
    std::vector<std::string> seg_res;
    DEBUG_LOG("Search Strategy %s", q.c_str());
	std::cout << "Search Strategy" << std::endl;
	Json::Value& jarray = root["seg"];
	if (jarray.size() == 0) {
		root["search"]["msg"] = "none";
		return SUB_OK;
	}
	Json::Value result;
	int idx = 0;
	for (int i = 0; i < jarray.size(); i++) {	
		// search_results = _dat->_reversed_search(jarray[i].asString());
		std::map<std::string, std::vector<reverse_index> >::iterator it = 
			_index_dict.find(jarray[i].asString());
		if (it == _index_dict.end()) {
			continue;
		}
		const std::vector<reverse_index>& search_results = it->second; 
		for (int j = 0; j < search_results.size(); j++) {
			idx ++;
			if (idx > 20) {
				break;
			}
			result.append(search_results[j].doc_id);
		}
		if (idx > 20) {
			break;
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
	// _dat = new DatImpl<std::vector<reverse_index> >();
	std::string index_file_path = "";
	SubConfig::_get_instance()->_get_conf_val("index_file", index_file_path);
	std::ifstream fis(index_file_path.c_str());
	std::string line;
	while (getline(fis, line)) {
		std::vector<std::string> splits;
		StringUtil::split(line, "\t", splits);
		if (2 != splits.size()) {
			continue;
		}
		std::vector<std::string> vecs;
		std::vector<reverse_index> index_vec;
		StringUtil::split(splits[1], "\002", vecs);
		for (int j = 0; j < vecs.size(); j++) {
			std::vector<std::string> data_str;
			StringUtil::split(vecs[j], "\001", data_str);
			if (2 != data_str.size()) {
				continue;
			}
			reverse_index t;
			t.doc_id = atoi(data_str[0].c_str());
			t.cnt = atoi(data_str[1].c_str());
			index_vec.push_back(t);
		}
		_index_dict[splits[0]] = index_vec;
	}
	return SUB_OK;
}

}
