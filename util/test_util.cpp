 
 /**
 * @file test_util.cpp
 * @date 2017/06/29 23:24:38
 * @version $Revision$ 
 * @brief 
 *  
 
**/

#include "utils.h"

using namespace sub_framework;
using namespace std;
int main() {


    std::string in;
    cin >> in;
    std::vector<std::string> vec;
    StringUtil::split_ch_words(in, vec);
    for (int i = 0; i <vec.size(); i++) {
        std::cout << vec[i] << std::endl;    
    }
    return 0;
}


/* vim: set ts=4 sw=4 sts=4 tw=100 */
