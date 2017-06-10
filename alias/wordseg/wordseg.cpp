
#include "wordseg.h"
#define NUM 10

namespace sub_framework {

WordSeg* WordSeg::_instance = NULL;

int WordSeg::_init_tagger(const char* model_path) {
    char _tagger_conf[128];
    sprintf(_tagger_conf, "-m %s -v 3 -n2", model_path);
    _tagger = CRFPP::createTagger(_tagger_conf);
    pthread_mutex_init(&seg_mutex, NULL);
    if (NULL == _tagger) {
        return FAIL;
    }
    return OK;
}

int WordSeg::split_ch_words(const std::string& word, std::vector<std::string>& cn_words)  {
    std::string cn_word = "";
    std::string not_cn_word = "";
    int i = 0;
    cn_words.clear();
    int len = word.length();
    while ( i < len ) {
        if ( (int) (word[i] & 0xf0 ) == 0xe0 ) {
            if ( (int) (word[i + 1] & 0xc0 ) == 0x80 && \
                    (int) (word[i + 2] & 0xc0 ) == 0x80 ) {
                cn_words.push_back(word.substr(i, 3));
                i += 3;
            } else {		
                i += 1;
            }
        } else if ( word[i] != ' ') {
            if ((int)(word[i]) <= 127) {
                cn_words.push_back(std::string(1, word[i]));
                i ++;
            } else {
                int j = i;
                while ( j <= word.length() && !(word[j] >= 0 && word[j] <= 127 ) ) {
                    j ++;
                }
                cn_words.push_back(word.substr(i, j - i));
                i = j;
            }
        } else {
            i += 1;
        }
    }	
    return OK;
}

int WordSeg::segment(const std::string& str, std::vector<std::string>& seg_results) {
    if (NULL == _tagger) {
        return FAIL;
    }
    std::vector<std::string> words;
    split_ch_words(str, words);
    pthread_mutex_lock(&seg_mutex);
    _tagger->clear();
    for (int i = 0; i < words.size(); i++) {
        char context[64];
        sprintf(context, "%s\to\tB", words[i].c_str());
        _tagger->add(context);
    }
    if (! _tagger->parse()) {
        pthread_mutex_unlock(&seg_mutex);
        return FAIL;
    }
    int rows = _tagger->size();
    int cols = _tagger->xsize();
    seg_results.clear();
    std::string seg_str = "";
    for (int i = 0; i < rows; i++) {
        const char* tag_res = _tagger->y2(i);
        seg_str += _tagger->x(i, 0);
        if (strcmp(tag_res, "E") == 0 || strcmp(tag_res, "S") == 0) {
            seg_results.push_back(seg_str);
            seg_str = "";
        }
    }
    pthread_mutex_unlock(&seg_mutex);
    return OK;
}

}

