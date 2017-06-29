/**
 * @file wordseg_strategy.h
 * @author kymowind@gmail.com
 * @date 2017/02/14 20:17:42
 * @brief abstract class of strategy 
 *  
 **/

#include "search_strategy.h"
#include "sub_log.h"
#include "dict_mgr.h"
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
	Json::Value& jarray = root["seg"];
	if (jarray.size() == 0) {
		root["search"]["msg"] = "none";
		return SUB_OK;
	}
	
    Json::Value result;
    std::vector<reverse_index> search_results_all;
    // TODO cache
    int idx = 0;
    std::map<int, int> doc_map;
    // search the result
	for (int i = 0; i < jarray.size(); i++) {	
		std::map<std::string, std::vector<reverse_index> >::iterator it = 
			_index_dict.find(jarray[i].asString());
		if (it == _index_dict.end()) {
			continue;
		}
		const std::vector<reverse_index>& search_results = it->second;
		for (int j = 0; j < search_results.size(); j++) {
            search_results_all.push_back(search_results[j]);
        }
	}

    // get the params
    int tot_cnt = search_results_all.size();
    int page_cnt = (tot_cnt - 1) / PAGE_RESULT_CNT + 1;
    if (page > page_cnt) {
        page = page_cnt;
    }
    int nxt_page = page >= page_cnt ? page_cnt : page + 1;
    int bef_page = page > 1 ? page -1 : 1;
    int begin_record_index = (page - 1) * PAGE_RESULT_CNT;
    int end_record_index = page >= page_cnt ? tot_cnt : (page) * PAGE_RESULT_CNT;

    // fit the result 
    for (std::vector<reverse_index>::iterator it = 
            search_results_all.begin() + begin_record_index;
            it != end_record_index + search_results_all.begin() 
            && it != search_results_all.end(); it++) {
        result.append(it->doc_id);
    }

	if (result.size() == 0) {
		root["search"]["msg"] = "none";
		return SUB_OK;
	}
	root["search"]["msg"] = "ok";
	root["search"]["result"] = result;
    root["search"]["tot_cnt"] = tot_cnt;
	root["search"]["tot_page"] = page_cnt;
    root["search"]["bef_page"] = bef_page;
    root["search"]["nxt_page"] = nxt_page;
    return SUB_OK;
}

int SearchStrategy::_init() {
	// _dat = new DatImpl<std::vector<reverse_index> >();
    REFER_DICT(_index_dict);
    REFER_DICT(_word_freq_dict);
    REFER_DICT(_word_doc_freq_dict);
    return SUB_OK;
}

}
