#include "crfpp.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <pthread.h>

typedef struct seg_res {
	std::string seg_result;
} SEGRES;

#define NUM 10

enum {
	FAIL = 0,
	OK
};

class WordSeg {
	private:
		CRFPP::Tagger *_tagger;
		static WordSeg* _instance;
		pthread_mutex_t seg_mutex;
	public:
		~WordSeg() {}
		WordSeg() {}
		static WordSeg* _get_intance() {

			if (NULL == _instance) {
				_instance = new WordSeg();
				_instance->_init_tagger("model");
			}
			return _instance;
		}

		int _init_tagger(const char* model_path) {
			char _tagger_conf[128];
			sprintf(_tagger_conf, "-m %s -v 3 -n2", model_path);
			_tagger = CRFPP::createTagger(_tagger_conf);
			pthread_mutex_init(&seg_mutex, NULL);
			if (NULL == _tagger) {
				return FAIL;
			}
			return OK;
		}
		int split_ch_words(const std::string& word, std::vector<std::string>& cn_words)  {
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
			return 1;
		}


		int seg_by_char(const std::string& q, std::vector<std::string>& words) {
			std::string last_c = "";
			for (int i = 0; i < q.length(); i++) {
				if ((int)(q[i]) >= 0 && (int)(q[i]) <= 127) {
					last_c += q[i];
				} else {
					if (last_c != "") {
						words.push_back(last_c);
						last_c = "";
					}
					words.push_back(q.substr(i, 2));
					i += 1;
				}
			}
			if (last_c != "") {
				words.push_back(last_c);
			}
			return OK;
		}

		int segment(const std::string& str, std::vector<std::string>& seg_results) {
			if (NULL == _tagger) {
				return FAIL;
			}
			std::vector<std::string> words;
			split_ch_words(str, words);
			//seg_by_char(str, words);
			pthread_mutex_lock(&seg_mutex);
			std::cout << "fuck segment!" << std::endl;
			std::cout << "hello" << std::endl;
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
};

void* thread_func(void*params) {
	SEGRES* segres = (SEGRES*)params;
	std::string str = "今天天气很好我很喜欢怎么办?";
	std::vector<std::string> strs;
	//WordSeg* seg = new WordSeg();
	//seg->_init_tagger("model");

	if (WordSeg::_get_intance()->segment(str, strs) == OK) {
	//if (seg->segment(str, strs) == OK) {
	} else {
	}
	for (int i = 0; i < strs.size(); i++) {
		segres->seg_result += strs[i] + "|";
	}
}

WordSeg* WordSeg::_instance = NULL;

int main() {
	//std::string str ;
	//std::cin >> str;
	for (int i = 0; i < NUM; i++) {
		std::string tmp_str = "今天天气很好";
		std::vector<std::string> seg_results;
		WordSeg::_get_intance()->segment(tmp_str, seg_results);
		for (int i = 0; i < seg_results.size(); i++) {
			std::cout << seg_results[i] << "\t";
		}
	}
	//std::cout << std::endl;
	SEGRES res[NUM];
	pthread_t threads[NUM];
	for (int i = 0; i < NUM; i++) {
		std::cout << "thread " << i << std::endl;
		int tid = pthread_create(&threads[i], NULL, thread_func, (void*)&res[i]); 
	}
	for (int i = 0; i < NUM; i++) {
		pthread_join(threads[i], NULL);
	}

	for (int i = 0; i < NUM; i ++) {
		std::cout << res[i].seg_result << std::endl; 
	}
	return 0;

}
