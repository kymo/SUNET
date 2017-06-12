
#define private public
#define protected public

#include "gtest/gtest.h"
#include "sub_log.h"

using namespace sub_framework;

class test_log_suite : public testing::Test {
	
protected:
	test_log_suite() {
		sub_log = SubLogger::_get_instance();
        sub_config = SubConfig::_get_instance();
    }

	virtual ~test_log_suite() {
		if (NULL != sub_log) {
			delete sub_log;
			sub_log = NULL;
		}
	}

	virtual void SetUp() {
	}

	virtual void TearDown() {
		
	}
private:
	SubLogger* sub_log;
    SubConfig* sub_config;
};

void* thread_func(void*params) {
	std::string str = "今天天气如何";
	std::vector<std::string> strs;
    char buf[1024];
    for (int i = 0; i < 1022;i ++) {
    buf[i] = 'a';}
    buf[1024] = '\0';
    DEBUG_LOG("jin天塌守空房姐挼看对方接口框架啊劳动纠纷");
    DEBUG_LOG("%s", buf);
}
#define NUM 10000
TEST_F(test_log_suite, test_conf) {
	sub_config->_read_conf_file("../conf/sub.conf");
    sub_log->_init_logger();
    sub_log->_write_log(DEBUG, "tianqibucuo");
    DEBUG_LOG("this is a test!");
    DEBUG_LOG("heloo");
    DEBUG_LOG("%s", "woqu");
	pthread_t threads[NUM];
	for (int i = 0; i < NUM; i++) {
		int tid = pthread_create(&threads[i], NULL, thread_func, NULL); 
	}
	for (int i = 0; i < NUM; i++) {
		pthread_join(threads[i], NULL);
	}

}

GTEST_API_ int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
