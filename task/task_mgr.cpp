 
/**
 * @file task_mgr.cpp
 * @author kymowind@gmail.com
 * @date 2016/10/10 20:48:26
 * @brief 
 *  
 **/


namespace sub_framework {

TaskMgr::TaskMgr() {

}

TaskMgr::~TaskMgr() {
    // empty the task query
    _clear_task_queue();
    pthread_mutex_destroy(&_add_mutex);
    pthread_mutex_destroy(&_get_mutex);
}

void TaskMgr::_init() {
   pthread_mutex_init(&_add_mutex, NULL);
   pthread_mutex_init(&_get_mutex, NULL);
}

void TaskMgr::_get_call_back_proc(const std::string& _task_name) {

}

void TaskMgr::_clear_task_queue() {
    while (! _task_queue.empty()) {
        Task* task = _task_queue.front();
        _task_queue.pop();
        if (task != NULL) {
            delete task;
            task = NULL;
        }
    }
}

void TaskMgr::_add_task(Task* task) {
    
    if (task == NULL) {
        return ;
    }
    pthread_mutex_lock(&_add_mutex);
    _task_queue.push(task);
    pthread_mutex_unlock(&_add_mutex);
}

Task* TaskMgr::_get_task() {

    if (_task_queue.empty()) {
        return NULL;
    }

}

}






















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
