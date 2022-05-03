#include "Task.h"

Task *pTask = nullptr;
Task::Task(void)
{
    pTask = this;
}
uint16_t Task::add(std::function<void(void)> fun)
{
    m_id++;
    TaskStruct task = {m_id, fun};
    taskList.push_back(task);
    return m_id;
}
void Task::deleteTask(uint16_t id)
{
    for (auto it = taskList.begin(); it != taskList.end(); it++)
    {
        if ((*it).id == id)
        {
            m_itNext = taskList.erase(it); //找到了就把它删除
            if (it == m_itNow)             //如果删除的是handle中正在使用的迭代器
            {
                m_itNow = taskList.end(); //迭代器失效后把它赋值为end
            }
            break;
        }
    }
}
void Task::handle(void)
{
    m_itNow = taskList.begin();
    while (m_itNow != taskList.end())
    {
        (*m_itNow).fun();
        if (m_itNow == taskList.end())
        {
            //迭代器失效了
            m_itNow = m_itNext;
        }
        else
        {
            //迭代器没有失效
            m_itNow++;
        }
    }
}
