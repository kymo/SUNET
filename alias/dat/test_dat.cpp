#include "dat.h"

using namespace sub_framework;

void test(const char*file_name) {
	DatImpl<int> *dat = new DatImpl<int>();
	dat->_load_dict(file_name);
	dat->_build();
	std::cout << "Build Done!" << std::endl;
	while(true) {
		std::string line;
		cin >> line;
		std::cout << "SearchResult:" << dat->_search(line) << std::endl;
	}
}

void test_index() {
	DatImpl<std::vector<reverse_index> > *dat = new DatImpl<std::vector<reverse_index> >();
	dat->_load_index_dict("index.dict");
	dat->_build();
	std::cout << "Build Done!" << std::endl;
	while (true) {
		std::string line;
		cin >> line;
		std::vector<reverse_index> ret;
		ret = dat->_reversed_search(line);
		for (int i = 0; i < ret.size(); i++) {
			std::cout << ret[i].doc_id << " " << ret[i].cnt << std::endl;
		}
	}
}

int main(int argc, char*argv[]) {
	//test(argv[1]);
	test_index();
	return 0;
}
