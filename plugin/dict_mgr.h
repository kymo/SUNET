 /**
 * @file dict_mgr.h
 * @date 2017/06/29 20:51:48
 * @version $Revision$ 
 * @brief 
 *  
 **/
#ifndef DICT_MGR_H
#define DICT_MGR_H

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include "config.h"
#include "sub_log.h"
#include "dat.h"
#include "trie.h"

namespace sub_framework {

#define REFER_DICT(dict_name) \
    dict_name = DictMgr::_get_instance()->_get##dict_name();

class DictMgr {

private:
    std::map<std::string, std::vector<reverse_index> > _index_dict;
    std::map<std::string, int> _word_freq_dict;
    std::map<std::string, int> _word_doc_freq_dict;
    std::map<int, int> _doc_length_dict;
    Trie<int>* _suggest_dict;
    static DictMgr* _instance;
    DictMgr() {}

public:
    ~DictMgr() {

    }

    static DictMgr* _get_instance() {
        if (NULL == _instance) {
            _instance = new DictMgr();
        }
        return _instance;
    }

    /*
     * @brief get index dict
     */
    const std::map<std::string, std::vector<reverse_index> >& _get_index_dict() const {
        return _index_dict;
    }
    
    const std::map<std::string, int>& _get_word_freq_dict() {
        return _word_freq_dict;
    }

    const std::map<std::string, int>& _get_word_doc_freq_dict() {
        return _word_doc_freq_dict;
    }
    
    const std::map<int, int>& _get_doc_length_dict() {
        return _doc_length_dict;
    }

    Trie<int>* _get_suggest_dict() {
        return _suggest_dict;
    }

    void _load_dict() {
        _load_index_dict();
        //_load_word_freq_dict();
        _load_word_doc_freq_dict();
        _load_doc_length_dict();
        _load_suggest_dict();
    }

    void _load_suggest_dict() {
        _suggest_dict = new Trie<int>();
        std::string suggest_file_path = "";
        SubConfig::_get_instance()->_get_conf_val("suggest_file", suggest_file_path);
        _suggest_dict->build(suggest_file_path.c_str());
    }
    
    void _load_doc_length_dict() {
        std::string doc_length_file_path = "";
        SubConfig::_get_instance()->_get_conf_val("doc_length_file", doc_length_file_path);
        std::ifstream fis(doc_length_file_path.c_str());
        std::string line;
        while (getline(fis, line)) {
            std::vector<std::string> splits;
            StringUtil::split(line, "\t", splits);
            if (2 != splits.size()) {
                continue;
            }
            _doc_length_dict[atoi(splits[0].c_str())] = atoi(splits[1].c_str());
        }
    }
    
    void _load_word_doc_freq_dict() {
        std::string word_doc_freq_file_path = "";
        SubConfig::_get_instance()->_get_conf_val("word_doc_freq_file", word_doc_freq_file_path);
        std::ifstream fis(word_doc_freq_file_path.c_str());
        std::string line;
        while (getline(fis, line)) {
            std::vector<std::string> splits;
            StringUtil::split(line, "\t", splits);
            if (2 != splits.size()) {
                continue;
            }
            _word_doc_freq_dict[splits[0]] = atoi(splits[1].c_str());
        }
    }

    void _load_word_freq_dict() {
        std::string word_freq_file_path = "";
        SubConfig::_get_instance()->_get_conf_val("word_freq_file", word_freq_file_path);
        std::ifstream fis(word_freq_file_path.c_str());
        std::string line;
        while (getline(fis, line)) {
            std::vector<std::string> splits;
            StringUtil::split(line, "\t", splits);
            if (2 != splits.size()) {
                continue;
            }
            _word_freq_dict[splits[0]] = atoi(splits[1].c_str());
        }
    }

    void _load_index_dict() {
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
                if (data_str.size() < 3) {
                    continue;
                }
                reverse_index t;
                t.doc_id = atoi(data_str[0].c_str());
                t.cnt = atoi(data_str[1].c_str());
                index_vec.push_back(t);
            }
            _index_dict[splits[0]] = index_vec;
        }
        DEBUG_LOG("Load Index Dict Okay!" );
    }

};

}

#endif  // DICT_MGR_H

/* vim: set ts=4 sw=4 sts=4 tw=100 */
