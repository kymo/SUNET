/**
 * @file strategy_mgr.h
 * @author kymowind@gmail.com
 * @date 2017/02/14 20:17:42
 * @brief 
 *  
 **/

#ifndef  __STRATEGY_H_
#define  __STRATEGY_H_

#define SUB_OK 1
#define SUB_FAIL 0

namespace sub_framework {

class SubStrategyMgr {

private:
	SubStrategyMgr();
	static SubStrategyMgr* _strategy_mgr_instance;
	std::map<std::string, std::vector<SubStrategy*> > _url_strategies_map;

public:
	~SubStrategyMgr();
	static SubStrategyMgr* _get_instance() {
		if (NULL == _strategy_mgr_instance) {
			_strategy_mgr_instance = new SubStrategyMgr();
		}
		return _strategy_mgr_instance;
	}

	/*
	 * @brief init the strategies accroding to the strategy conf
	 * @return
	 * 		SUB_OK init ok
	 * 		SUB_FAIL init failure
	 *
	 */
	int _init_strategies(SubStrategyConf* sub_conf);


	
	// SubStrategy* _get_strategy_instance(const std::string stragey_name);

	
	/*
	 * @brief run the strategy binded to the uri
	 * @return
	 * 		SUB_OK run ok
	 * 		SUB_FAIL run failue
	 */
	int _run_uri(const std::string uri, const HttpRequest& req, std::string& ret_json);	


};


}
	
#endif
