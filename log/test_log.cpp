
#define private public
#define protected public

#include "gtest/gtest.h"
#include "sub_log.h"

using namespace sub_framework;

class test_log_suite : public testing::Test {
	
protected:
	test_log_suite() {
		sub_log = SubLogger::_get_instance();
        sub_log->_init_logger();
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

TEST_F(test_log_suite, test_conf) {
	sub_config->_read_conf_file("../conf/sub.conf");
    sub_log->_write_log(DEBUG, "tianqibucuo");
    DEBUG_LOG("this is a test!");
    DEBUG_LOG("heloo");
    DEBUG_LOG("%s", "woqu");
}

GTEST_API_ int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
