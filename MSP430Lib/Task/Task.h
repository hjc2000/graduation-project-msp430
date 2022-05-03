#ifndef Task_h
#define Task_h

#include <functional>
#include <list>
using namespace std;

class Task
{
public:
    Task(void);
    uint16_t add(std::function<void(void)> fun);
    void deleteTask(uint16_t id);
    void handle(void);

protected:
    uint16_t m_id = 0;
    struct TaskStruct
    {
        uint16_t id;
        function<void(void)> fun;
    };
    list<TaskStruct>::iterator m_itNext;
    list<TaskStruct>::iterator m_itNow;
    list<TaskStruct> taskList; //任务列表
};
extern Task *pTask;

#endif // Task_h
