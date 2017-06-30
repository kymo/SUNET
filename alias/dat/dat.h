/*
* @biref a implementation of double array trie 
*/

#ifndef _DAT_H_
#define _DAT_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

using namespace std;

#define CN_STRING_LEN 3
#define INDEX_SEP_LEN 2
#define INDEX_DATA_SEP_LEN 2

namespace sub_framework {

typedef struct node {
    int depth;
    int left;
    int right;
    int code;
    node() {}
    node(int _depth, int _left, int _right, int _code) {
        depth = _depth;
        left = _left;
        right = _right;
        code = _code;
    }
} tree_node;

typedef struct cnstr {
    std::vector<std::string> _words;
    std::string _string;
    cnstr() {
    }
    cnstr(const std::string& _str) {
        StringUtil::split_ch_words(_str, _words);
        _string = _str;
    }
    int length() {
        return _words.size();
    }
    std::string operator [] (int i) {
        return _words[i];
    }
} cn_string;

typedef struct index {
    int doc_id;
    int cnt;
    // std::vector<int> pos_vec;
} reverse_index;

typedef reverse_index* reverse_index_t;

typedef tree_node* tree_node_t;

template <typename value_type>
class DatImpl {
private:
    std::vector<cn_string> _dicts;
    std::vector<value_type> _values;
    int *_base;
    int *_check;
    bool *_used;
    int _size;
	int _next_check_pos;
	int _non_zero_num;
	int _progress;
public:
    
    DatImpl() {
        _size = 65536 * 32;
        _base = new int[_size];
        _check = new int[_size];
        _used = new bool[_size];
        memset(_base, 0, sizeof(_base));
        memset(_check, 0, sizeof(_check));
        memset(_used, false, sizeof(_check));
		_non_zero_num = 0;
		_progress = 0;
		_next_check_pos = 0;
    }

    void resize(int new_size) {
        std::cout << "Resize the size" << new_size << std::endl;
        int* new_base = new int[new_size];
        int* new_check = new int[new_size];
        bool* new_used = new bool[new_size];
        memcpy(new_base, _base, _size);
        memcpy(new_check, _check, _size);
        memcpy(new_used, _used, _size);
        delete _base;
        delete _check;
        delete _used;
        _base = new_base;
        _check = new_check;
        _used = new_used;
        _size = new_size;
    }

    void _load_dict(const char*file_name) {
        std::ifstream fis(file_name);
        std::string line;
        while (getline(fis, line)) {
            _dicts.push_back(cn_string(line));
        }
    }

    int _load_index_dict(const char* file_name) {
        std::ifstream fis(file_name);
        std::string line;
        while (getline(fis, line)) {
            std::vector<std::string> split_strs;
            StringUtil::split(line, "\t", split_strs);
            if (split_strs.size() != INDEX_SEP_LEN) {
                continue;
            }
            std::vector<std::string> index_split_strs;
            std::vector<reverse_index> index_vec;
            StringUtil::split(split_strs[1], "\002", index_split_strs);
            for (int i = 0; i < index_split_strs.size(); i++) {
                std::vector<std::string> doc_infor_splits;
                StringUtil::split(index_split_strs[i], "\001", doc_infor_splits);
                if (doc_infor_splits.size() < INDEX_DATA_SEP_LEN + 1) {
                    continue;
                }
                reverse_index t;
                t.doc_id = atoi(doc_infor_splits[0].c_str());
                t.cnt = atoi(doc_infor_splits[1].c_str());
                //for (int j = 2; j < doc_infor_splits.size(); j++) {
                //    t.pos_vec.push_back(atoi(doc_infor_splits[j].c_str()));
                //}
                index_vec.push_back(t);
            }
            //maps[split_strs[0]] = index_vec;
            _dicts.push_back(cn_string(split_strs[0]));
            _values.push_back(index_vec);
        }
		return 1;
    }

    /*
    *@brief assume that words are encoded by utf-8, we need to transfer it into 
    * unicode style to get the final code that they are presented
    * for example: '汉' the utf8 code is 11100110 10110001 10001001, it use the
    * three bits template(1110xxxx 10xxxxxx 10xxxxxx), the 'x' stands for the 
    * corresponding unicode binary code. in this case, the unicode is 0110-110001-001001
    */
    int get_cn_code(const std::string& cn_str) {
        if (cn_str.length() > CN_STRING_LEN) {
            exit(1);
        }
        int code = 0;
        if ((int)((cn_str[0] - '\0') & 0xf0) == 0xe0) {
            int third = (cn_str[2] - '\0') & (0x7f);
            int second = (cn_str[1] - '\0') & (0x7f) ;
            int first = (cn_str[0] - '\0') & (0x0f);
            code = first * (1 << 12) + second * (1 << 6) + third;
        } else if ((int)((cn_str[0] - '\0') & 0xe0) == 0xd0) {
            int second = (cn_str[1] - '\0') & (0x7f);
            int first = (cn_str[0] - '\0') & (0x1f);
            code = first * (1 << 6) + second;
        } else if ((int)((cn_str[0] - '\0') & 0x80) == 0x00) {
            int first = (cn_str[0] - '\0') & 0x7f;
            code = first;
        }
        return code;
    }
    /*
    *@brief fetch the child_nodes of the parrent node
    */
    int _fetch_child_nodes(tree_node_t parent, std::vector<tree_node_t>& child_nodes) {
        int prev_code = 0;
        for (int i = parent->left; i < parent->right; i++) {
            if (_dicts[i].length() < parent->depth) {
                continue;
            }
            int cur_code = 0;
            if (_dicts[i].length() != parent->depth) {
                cur_code = get_cn_code(_dicts[i][parent->depth]);
            }

            if (prev_code > cur_code) {
                return 0;
            }
            if (prev_code != cur_code || child_nodes.size() == 0) {
                tree_node_t node = new tree_node();
                node->depth = parent->depth + 1;
                node->code = cur_code;
                node->left = i;
                if (child_nodes.size() != 0) {
                    child_nodes[child_nodes.size() - 1]->right = i;
                }
                child_nodes.push_back(node);
            }
            prev_code = cur_code;
        }
        if (child_nodes.size() != 0) {
            child_nodes[child_nodes.size() - 1]->right = parent->right;
        }
        return child_nodes.size();
    }

    /*
    * @brief child_nodes are the child nodes of the parent node.
    * _base[i] i stands for the state, _base[i] stands for the corresponding value
    * new_child_nodes stands for the corresponding states
    *    _base[begin + child_nodes[i]->code] = _insert(new_child_nodes)
    * _check[i] i stands for the state, _check[i] stands for the previus state
    *     _check[begin + child_nodes[i]->code ] = begin
    */
    int _insert(const std::vector<tree_node_t>& child_nodes) {
        int begin = 0;
		int pos = ((child_nodes[0]->code + 1 > _next_check_pos) ? 
				child_nodes[0]->code + 1 : _next_check_pos) - 1;
		int first = 0;
		int _non_zero_num = 0;
		if (_size <= pos) {
			std::cout << "HEHE1" << pos << std::endl;
			resize(pos + 1);
		}
        while (true) {
            pos ++;
			if (_size <= pos) {
				std::cout << "HEHE1" << std::endl;
				resize(pos + 1);
			}
            if (_check[pos] != 0) {
				_non_zero_num++;
                continue;
            } else if (first == 0) {
				_next_check_pos = pos;
				first = 1;
			}
            begin = pos - child_nodes[0]->code;
            if (child_nodes.back()->code + begin >= _size) {
				double l = (1.05 > 1.0 * _dicts.size() / (_progress + 1)) ? 1.05 : 1.0
					* _dicts.size() / (_progress + 1);
				std::cout << "HEHE" << std::endl;
				resize((int) (_size * l));
            }
            if (_used[begin]) {
                continue;
            }
            bool tag = false;
            for (int i = 0; i < child_nodes.size(); i++) {
                if (_check[begin + child_nodes[i]->code] != 0) {
                    tag = true;
                    break;
                }
            }
            if (!tag) {
                break;
            }
        }
        _used[begin] = true;
		if (1.0 * _non_zero_num / (pos - _next_check_pos + 1) >= 0.95) {
			_next_check_pos = pos;
		}
        for (int i = 0; i < child_nodes.size(); i++) {
            _check[begin + child_nodes[i]->code] = begin;
        }
        for (int i = 0; i < child_nodes.size(); i++) {
            std::vector<tree_node_t> new_child_nodes;
            if (_fetch_child_nodes(child_nodes[i], new_child_nodes) == 0) {
                _base[begin + child_nodes[i]->code] = - child_nodes[i]->left - 1;
				_progress ++;
            } else {
                int sub_h = _insert(new_child_nodes);
                _base[begin + child_nodes[i]->code] = sub_h;
            }
        }
        return begin;
    }

    value_type _reversed_search(const std::string& key) {
        std::vector<std::string> cn_words;
        StringUtil::split_ch_words(key, cn_words);
        int b = _base[0];
        int p;
        int result = -1;
        value_type t;
        for (int i = 0; i < cn_words.size(); i++) {
            int code = get_cn_code(cn_words[i]);
            p = b + code;
            if (b == _check[p]) {
                b = _base[p];
            } else {
                return t;
            }
        }
        p = b;
        if (b == _check[p] && _base[p] < 0) {
            result = - _base[p] - 1;
            return _values[result];
        } 
        return t;
    }
    
    int _search(const std::string& key) {
        std::vector<std::string> cn_words;
        StringUtil::split_ch_words(key, cn_words);
        int b = _base[0];
        int p;
        int result = -1;
        for (int i = 0; i < cn_words.size(); i++) {
            int code = get_cn_code(cn_words[i]);
            p = b + code;
            if (b == _check[p]) {
                b = _base[p];
            } else {
                return result;
            }
        }
        p = b;
        if (b == _check[p] && _base[p] < 0) {
            result = - _base[p] - 1;
        }
        return result;
    }

    /*
    *@brief
    */
    void _build() {
        tree_node_t node = new tree_node();
        node->left = 0;
        node->code = 0;
        node->right = _dicts.size();
        node->depth = 0;
        std::vector<tree_node_t> child_nodes;
        _fetch_child_nodes(node, child_nodes);
        _base[0] = _insert(child_nodes);
    }
};

}

#endif
