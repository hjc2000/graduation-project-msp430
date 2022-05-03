/*软件定时器*/
#ifndef TIMER_H
#define TIMER_H

#include <list>
#include "stdint.h"
#include <functional>
using namespace std;

class Task
{
public:
    uint16_t add(std::function<void(void)> fun)
    {
        m_id++;
        TaskStruct task = {m_id, fun};
        taskList.push_back(task);
        return m_id;
    }
    void deleteTask(uint16_t id)
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

    void handle(void)
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
extern Task task;

class TimerBase
{
public:
    virtual void handle(void) = 0;
    uint16_t addTask(uint16_t interval, function<void(void)> fun)
    {
        m_id++;
        Task task = {m_id, interval, interval, fun};
        taskList.push_back(task);
        return m_id;
    }
    void deleteTask(uint16_t id)
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

protected:
    uint16_t m_id = 0;
    struct Task
    {
        uint16_t id;
        uint16_t interval = 0; //任务的时间间隔
        uint16_t count = 0;    //递减，初值等于interval，为0时就执行fun
        function<void(void)> fun;
    };
    list<Task>::iterator m_itNext;
    list<Task>::iterator m_itNow;
    list<Task> taskList; //任务列表
};

class Timer : public TimerBase
{
public:
    bool m_bTimeUp = false; //在定时器中断中赋值为真
    void handle(void)       //放到死循环中
    {
        if (m_bTimeUp)
        {
            m_bTimeUp = false;
            /*在这里取出任务列表中的任务执行*/
            m_itNow = taskList.begin();
            while (m_itNow != taskList.end())
            {
                if ((*m_itNow).count)
                {
                    //时间还没到
                    (*m_itNow).count--;
                    m_itNow++;
                }
                else
                {
                    //时间到了
                    (*m_itNow).fun(); //如果fun把迭代器指向的项删除了，it就会变成野指针
                    if (m_itNow == taskList.end())
                    {
                        //迭代器失效了
                        m_itNow = m_itNext;
                    }
                    else
                    {
                        //迭代器没有失效
                        (*m_itNow).count = (*m_itNow).interval;
                        m_itNow++;
                    }
                }
            }
        }
    }
};
extern Timer timer;

class Ticker : public TimerBase
{
public:
    Ticker(void)
    {
        auto fun = [this](void) -> void
        {
            handle();
        };
        timer.addTask(1, fun); //每100us执行一次
    }

    void handle(void)
    {
        m_itNow = taskList.begin();
        while (m_itNow != taskList.end())
        {
            if ((*m_itNow).count)
            {
                //时间还没到
                (*m_itNow).count--;
                m_itNow++;
            }
            else
            {
                //时间到了
                (*m_itNow).fun(); //如果fun把迭代器指向的项删除了，it就会变成野指针
                if (m_itNow == taskList.end())
                {
                    //迭代器失效了
                    m_itNow = m_itNext;
                }
                else
                {
                    //迭代器没有失效
                    m_itNow = taskList.erase(m_itNow);
                }
            }
        }
    }
};
extern Ticker ticker;
#endif // TIMER_H
