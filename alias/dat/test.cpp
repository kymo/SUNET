#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <map>
using namespace std;


typedef struct {
	int doc_id;
	int cnt;
} reverse_index;

void split(const std::string& str, 
            const std::string& delim,
            std::vector<std::string>& ret) {
        if (str.size() <= 0 || delim.size() <= 0) {
            ret.clear();
            return;
        }
        ret.clear();
        int last = 0;
        int index = str.find_first_of(delim, last);
        while (index != -1) {
            ret.push_back(str.substr(last, index - last));
            last = index + 1;
            index = str.find_first_of(delim, last);
        }
        if (index == -1 && str[str.size() - 1] != '\t') {
            ret.push_back(str.substr(last, str.size() - last));
        }
    }
int main() {
	
	ifstream fis("index.dict");
	string line;

	std::map<std::string, std::vector<reverse_index> > mp;
	while (getline(fis, line)) {
		std::vector<std::string> splits;
		split(line, "\t", splits);
		if (2 != splits.size()) {
			continue;
		}
		std::vector<std::string> vecs;
		std::vector<reverse_index> index_vec;
		split(splits[1], "\002", vecs);
		for (int j = 0; j < vecs.size(); j++) {
			std::vector<std::string> data_str;
			split(vecs[j], "\001", data_str);
			reverse_index t;
			t.doc_id = atoi(data_str[0].c_str());
			t.cnt = atoi(data_str[1].c_str());
			index_vec.push_back(t);
		}
		mp[splits[0]] = index_vec;
	}

	while (1);
	return 0;
}
