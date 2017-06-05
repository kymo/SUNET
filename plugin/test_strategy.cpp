
#define private public
#define protected public

#include "gtest/gtest.h"
#include "text_strategy.h"
#include "strategy_mgr.h"
#include "json/json.h"

using namespace sub_framework;

class test_strategy_suite : public testing::Test {
	
protected:
	test_strategy_suite() {
		strategy_mgr = SubStrategyMgr::_get_instance();

	}

	virtual ~test_strategy_suite() {
	}

	virtual void SetUp() {
	}

	virtual void TearDown() {
		
	}
private:
	SubStrategyMgr* strategy_mgr;
};

TEST_F(test_strategy_suite, test_strategy) {
	SubStrategyConfig::_get_instance()->_read_conf_file("../conf/strategy.conf");
	strategy_mgr->_init_strategies();
	Json::Value root;
	std::string uri = "search";
	Request req;
	strategy_mgr->_run_uri(uri, req, root);
	
	root.clear();
	uri = "/";
	strategy_mgr->_run_uri(uri, req, root);
	
	uri = "searchs";
	root.clear();
	Json::Value js;
	Json::Value empty;
	empty.clear();
	strategy_mgr->_run_uri(uri, req, js);
	ASSERT_EQ(js.toStyledString(), empty.toStyledString());

}

GTEST_API_ int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
