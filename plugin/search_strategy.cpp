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

bool cmp(const std::pair<int, float>& lhs, const std::pair<int, float>& rhs) {  
  return lhs.second > rhs.second;
} 

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
        root["search"]["msg"] = "seg_error";
        return SUB_OK;
    }
    
    Json::Value result;
    std::vector<reverse_index> search_results_all;
    // TODO cache
    int idx = 0;
    std::map<int, int> doc_map;
    // search the resul
    std::map<int, float> doc_score;
    
    for (int i = 0; i < jarray.size(); i++) {
        const std::string& term = jarray[i].asString();
        std::map<std::string, std::vector<reverse_index> >::iterator it = 
            _index_dict.find(term);
        if (it == _index_dict.end()) {
            std::cout << "END!" << " " << term << std::endl;
            continue;
        }
        std::cout << "YED!" << " " << term << std::endl;
        const std::vector<reverse_index>& search_results = it->second;
        std::cout << search_results.size() << std::endl;
        int term_cnt = 0;
        if (_word_doc_freq_dict.find(term) != _word_doc_freq_dict.end()) {
            term_cnt = _word_doc_freq_dict[term];
        }
        float idf = log((230000 - term_cnt + 0.5) / (term_cnt + 0.5));
        for (int j = 0; j < search_results.size(); j++) {
            if (doc_score.find(search_results[j].doc_id) == doc_score.end()) {
                doc_score[search_results[j].doc_id] = 0.0;
            }
            float rel = (search_results[j].cnt * 2) * 1.0 / 
                (search_results[j].cnt + 0.25 + 0.75 * _doc_length_dict[search_results[j].doc_id] / 8.0);
            std::cout << idf << " " << rel << " " << _doc_length_dict[search_results[j].doc_id] << std::endl;
            doc_score[search_results[j].doc_id] += idf * rel;
        }
    }
    std::vector<std::pair<int, float> > doc_score_vec(doc_score.begin(), doc_score.end());
    sort(doc_score_vec.begin(), doc_score_vec.end(), cmp);

    // get the params
    int tot_cnt = doc_score_vec.size();
    int page_cnt = (tot_cnt - 1) / PAGE_RESULT_CNT + 1;
    if (page > page_cnt) {
        page = page_cnt;
    }
    int nxt_page = page >= page_cnt ? page_cnt : page + 1;
    int bef_page = page > 1 ? page -1 : 1;
    int begin_record_index = (page - 1) * PAGE_RESULT_CNT;
    int end_record_index = page >= page_cnt ? tot_cnt : (page) * PAGE_RESULT_CNT;

    // fit the result 
    for (std::vector<std::pair<int, float> >::iterator it = 
            doc_score_vec.begin() + begin_record_index;
            it != end_record_index + doc_score_vec.begin() 
            && it != doc_score_vec.end(); it++) {
        Json::Value doc_info;
        doc_info["id"] = it->first;
        doc_info["score"] = it->second;
        result.append(doc_info);
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
    REFER_DICT(_doc_length_dict);
    return SUB_OK;
}

}
