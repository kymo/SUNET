 /**
 * @file wordseg.h
 * @date 2017/06/10 17:13:42
 * @version $Revision$ 
 * @brief 
 *  
 **/
#ifndef WORDSEG_H
#define WORDSEG_H

#include "crfpp.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <pthread.h>

namespace sub_framework {

typedef struct seg_res {
    std::string seg_result;
} SEGRES;

enum {
    FAIL = 0,
    OK
};

class WordSeg {
private:
    CRFPP::Tagger *_tagger;
    static WordSeg* _instance;
    WordSeg() {}
    pthread_mutex_t seg_mutex;
public:
    ~WordSeg() {}
    static WordSeg* _get_instance() {
        if (NULL == _instance) {
            _instance = new WordSeg();
        }
        return _instance;
    }
    int _init_tagger(const char* model_path);
    int split_ch_words(const std::string& word, std::vector<std::string>& cn_words);
    int segment(const std::string& str, std::vector<std::string>& seg_results);
};
}

#endif  // WORDSEG_H
/* vim: set ts=4 sw=4 sts=4 tw=100 */
