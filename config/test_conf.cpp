
#define private public
#define protected public

#include "config.h"
#include "strategy_conf.h"
#include "gtest/gtest.h"

using namespace sub_framework;

class test_config_suite : public testing::Test {
	
protected:
	test_config_suite() {
		sub_config = SubConfig::_get_instance();
		sub_strategy_config = SubStrategyConfig::_get_instance();
	}

	virtual ~test_config_suite() {
		if (NULL != sub_config) {
			delete sub_config;
			sub_config = NULL;
		}
		if (NULL != sub_strategy_config) {
			delete sub_strategy_config;
			sub_strategy_config = NULL;
		}
	}

	virtual void SetUp() {
	}

	virtual void TearDown() {
		
	}
private:
	SubConfig* sub_config;
	SubStrategyConfig* sub_strategy_config;
};

TEST_F(test_config_suite, test_conf) {
	sub_config->_read_conf_file("../conf/sub.conf");
	ASSERT_EQ(sub_config->_get_thread_cnt(), 10);
}

TEST_F(test_config_suite, test_config_conf) {
	sub_strategy_config->_read_conf_file("../conf/strategy.conf");
}

GTEST_API_ int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
