/*
 * build index cpp
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include "wordseg.h"
#include "utils.h"

using namespace sub_framework;
using namespace std;

typedef struct rever {
    int doc_id;
    int cnt;
} reverse_index;

void build_index(const char*file_name) {
    // init wordseg
    WordSeg::_get_instance()->_init_tagger("./model");
    std::ifstream fis(file_name);
    std::string line;

    std::map<std::string, std::vector<reverse_index> > mps;
    std::map<std::string, int> _word_freq_dict;
    std::map<std::string ,int> _word_doc_freq_dict;
    std::map<int, int> _doc_length_dict;

    while (getline(fis, line)) {
        std::vector<std::string> vals;
        StringUtil::split(line, "\t", vals);
        if (vals.size() != 2) {
            continue;
        }
        int id = atoi(vals[0].c_str());
        _doc_length_dict[id] = line.length(); 
        std::vector<std::string> seg_results;
        WordSeg::_get_instance()->segment(vals[1], seg_results);
        std::map<std::string, int> cnt_map;
        // std::map<std::string, std::vector<int> > pos_map;
        int pos = 0;
        for (int i = 0; i < seg_results.size(); i++) {        
            if (cnt_map.find(seg_results[i]) == cnt_map.end()) {
                cnt_map[seg_results[i]] = 0;
            }
            //if (pos_map.find(seg_results[i]) == pos_map.end()) {
            //    pos_map[seg_results[i]] = std::vector<int>();
            //}
            cnt_map[seg_results[i]] += 1;
            //pos_map[seg_results[i]].push_back(pos);
            pos += seg_results[i].length();
            if (_word_freq_dict.find(seg_results[i]) == _word_freq_dict.end()) {
                _word_freq_dict[seg_results[i]] = 0;
            }
            _word_freq_dict[seg_results[i]] += 1;
        }
        for (std::map<std::string, int>::iterator it = 
                cnt_map.begin(); it != cnt_map.end(); it++) {            
            if (mps.find(it->first) == mps.end()) {
                mps[it->first] = std::vector<reverse_index>();
            }
            reverse_index t;
            t.doc_id = id;
            t.cnt = it->second;
            // t.pos_vec = pos_map[it->first];
            mps[it->first].push_back(t);
            if (_word_doc_freq_dict.find(it->first) == _word_doc_freq_dict.end()) {
                _word_doc_freq_dict[it->first] = 0;
            }
            _word_doc_freq_dict[it->first] += 1;
        }
    }

    // output index.dict
    ofstream os_index("index.dict");
    for (std::map<std::string, std::vector<reverse_index> >::iterator it = mps.begin(); 
            it != mps.end(); it++) {
        if (it->first.length() < 3) {
            continue;
        }
        os_index << it->first << "\t";
        for (int i = 0; i < it->second.size(); i++) {
            os_index << it->second[i].doc_id << "\001" << it->second[i].cnt << "\001" << "0";
            if (i + 1 < it->second.size()) {
                os_index << "\002";
            }
        }
        os_index << std::endl;
    }
    os_index.close();
    // output word_freq_dict
    ofstream os_word_freq("word_freq.dict");
    for (std::map<std::string, int>::iterator it = _word_freq_dict.begin(); it != _word_freq_dict.end(); it++) {
        os_word_freq << it->first << "\t" << it->second << std::endl;
    }
    os_word_freq.close();
    ofstream os_word_doc_freq("word_doc_freq.dict");
    for (std::map<std::string, int>::iterator it = _word_doc_freq_dict.begin();
            it != _word_doc_freq_dict.end(); it++) {
        os_word_doc_freq << it->first << "\t" << it->second << std::endl;
    }
    os_word_doc_freq.close();
    
    ofstream os_doc_length("doc_length.dict");
    for (std::map<int, int>::iterator it = _doc_length_dict.begin();
            it != _doc_length_dict.end(); it++) {
        os_doc_length << it->first << "\t" << it->second << std::endl;
    }
    os_doc_length.close();

}

int main(int argc, char*argv[]) {
    
    if (argc < 2) {
        std::cout << "Usage: ./build_index sql_file " << std::endl;
        exit(1);
    }
    build_index(argv[1]);    

}
