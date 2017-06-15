 /**
 * @file test_wordseg.cpp
 * @date 2017/06/10 17:16:39
 * @version $Revision$ 
 * @brief 
 *  
 **/



#define private public
#define protected public

#include "gtest/gtest.h"
#include "wordseg.h"

using namespace sub_framework;

#define NUM 100
class test_wordseg_suite : public testing::Test {
    
protected:
    test_wordseg_suite() {
        wordseg = WordSeg::_get_instance();
    }

    virtual ~test_wordseg_suite() {
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {        
    }
private:
    WordSeg* wordseg;
};

void* thread_func(void*params) {
    SEGRES* segres = (SEGRES*)params;
    std::string str = "今天天气如何";
    std::vector<std::string> strs;
    WordSeg::_get_instance()->segment(str, strs);
    for (int i = 0; i < strs.size(); i++) {
        segres->seg_result += strs[i] + "|";
    }
}

TEST_F(test_wordseg_suite, test_wordseg) {
    int ret = wordseg->_init_tagger("model");
    ASSERT_EQ(ret, OK);

    std::string str = "今天天气如何";
    std::string segstr = "今天|天气|如何|";
    
    std::vector<std::string> seg_res;
    ret = wordseg->segment(str, seg_res);
    ASSERT_EQ(ret, OK);
    std::string true_res = "";
    for (int i = 0; i < seg_res.size(); i++) {
        true_res += seg_res[i] + "|";
    }
    ASSERT_EQ(true_res, segstr);
    
    // multi-thread test
    SEGRES res[NUM];
    pthread_t threads[NUM];
    for (int i = 0; i < NUM; i++) {
        int tid = pthread_create(&threads[i], NULL, thread_func, (void*)&res[i]); 
    }
    for (int i = 0; i < NUM; i++) {
        pthread_join(threads[i], NULL);
    }
    for (int i = 0; i < NUM; i ++) {
        ASSERT_EQ(res[i].seg_result, segstr);
    }
    str = "我爱北京天安门，天安门上太阳升";
    segstr = "我|爱|北京|天安门|，|天安门|上|太阳|升|";
    seg_res.clear();
    ret = wordseg->segment(str, seg_res);
    ASSERT_EQ(ret, OK);
    true_res = "";
    for (int i = 0; i < seg_res.size(); i++) {
        true_res += seg_res[i] + "|";
    }
    ASSERT_EQ(true_res, segstr);
}

GTEST_API_ int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/* vim: set ts=4 sw=4 sts=4 tw=100 */
