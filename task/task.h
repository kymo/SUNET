 
/**
 * @file task.h
 * @author kymowind@gmail.com
 * @date 2016/10/09 20:46:07
 * @brief 
 *  
 **/




#ifndef  __TASK_H_
#define  __TASK_H_

#include <iostream>
#include <map>
#include <string>

namespace sub_framework {


class SubTask {

private:
    std::string _task_name;

public:
    SubTask();
    virtual ~SubTask();
    virtual int _run() = 0;
    virtual int _call_back() = 0;


}





}










#endif  //__TASK_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
