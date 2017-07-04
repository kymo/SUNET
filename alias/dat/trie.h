
#ifndef _TRIE_H
#define _TRIE_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include "utils.h"
#include "dat.h"

using namespace std;

namespace sub_framework {

template <typename type_val>
class trie_node {
public:
	map<std::string, trie_node<type_val>* > _child_nodes_map;
	std::string _val;
	int _status;
	type_val tval;
	trie_node(){}
} ;

template <typename type_val>
class Trie {
private:
	trie_node<type_val> * root;
public:
	Trie() {
		root = new trie_node<type_val>();
	}

	void insert(trie_node<type_val>* root, const cn_string& str, const type_val& val) {
		if (str.length() == 0) {
			return ;
		}
		for (int idx = 0; idx < str.length(); idx++) {
			if (root->_child_nodes_map.find(str[idx]) == root->_child_nodes_map.end()) {
				trie_node<type_val>* t = new trie_node<type_val>();
				t->_val = str[idx];
				t->_status = 0;
				root->_child_nodes_map[str[idx]] = t;
			}
			root = root->_child_nodes_map[str[idx]];
		}
		root->_status = 1;
		root->tval = val;
	}

	trie_node<type_val>* extract_search(trie_node<type_val>* root, const cn_string& str) {
		for (int i = 0; i < str.length(); i++) {
			if (root->_child_nodes_map.find(str[i]) == root->_child_nodes_map.end()) {
				return NULL;
			} else {
				root = root->_child_nodes_map[str[i]];
			}
		}
		return root;
	}

	void dfs_search(trie_node<type_val> *root, std::string begin_str, 
			std::vector<std::pair<std::string, type_val> >& search_results) {
		if (root->_status == 1) {
			search_results.push_back(std::make_pair<std::string, type_val> (begin_str, root->tval));
		}
		typename std::map<std::string, trie_node<type_val>* >::iterator it;
		for (it = root->_child_nodes_map.begin(); it != root->_child_nodes_map.end(); it++) {
			dfs_search(it->second, begin_str + it->first, search_results);
		}
	}

	void search(const cn_string& str, std::vector<std::pair<std::string, type_val> >& search_results) {
		trie_node<type_val>* target_node = extract_search(root, str);
		if (NULL == target_node) {
			return ;
		}
		std::string begin_str = str._string;
		dfs_search(target_node, begin_str, search_results);
	}

	void build(const char* file_name) {
		ifstream fis(file_name);
		std::string line;
		while (getline(fis, line)) {
			std::vector<std::string> vals;
			StringUtil::split(line, "\t", vals), 
			insert(root, cn_string(vals[1]), atoi(vals[0].c_str()));
		}
	}
};


}
#endif

	/*
	int main(int argc, char*argv[]) {
		ifstream fis(argv[1]);
		std::string line;
		trie_node*root = new trie_node();
		while (getline(fis, line)) {
			insert(root, cn_string(line));
		}
		while (1) {
			int choose;
			string str;
			cout << "1: exact search 2: prefix search" << std::endl;
			cin >> choose;
			cout << "input the string:";
			cin >> str;
			std::cout << "search result:" << std::endl;
			if (choose == 1) {
				trie_node* t = extract_search(root, cn_string(str));
				if (t != NULL) {
					cout << "success!";
				} else {
					cout << "failed!";
				}
				cout << endl;
			} else if (choose == 2) {
				std::vector<std::string> res = search(root, cn_string(str));
				for (int i = 0; i < res.size(); i++) {
					std::cout << res[i] << std::endl;
				}
			}
		}
		return 0;
	}
	*/
