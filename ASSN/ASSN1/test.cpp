#pragma once
#include "MainHeader.h"
#include <queue>
#include <mutex>

template <class T>
class CMessageQueue
{
    queue<T> queue_;
    mutex mutex_;
    condition_variable cond_;

public:

    CMessageQueue()
    {
    }
    ~CMessageQueue()
    {

    }

    static map<int, CMessageQueue*> V_Multiton;
    static CMessageQueue* M_GetSingletone(int n)
    {
        if (V_Multiton.count(n)) return V_Multiton[n];
        V_Multiton[n] = new CMessageQueue();
        return V_Multiton[n];
    }
    T M_Pop()
    {
        unique_lock<mutex> mlock(mutex_); //acquire lock //http://en.cppreference.com/w/cpp/thread/unique_lock/unique_lock
        while (queue_.empty())
        {
            cond_.wait(mlock);  //  release lock and block thread and wait https://en.cppreference.com/w/cpp/thread/condition_variable/wait
        }
        auto item = queue_.front();
        queue_.pop();
        return item; //automatic unlock when block ends
    }
    void M_Push(const T& item)
    {
        unique_lock<mutex> mlock(mutex_);
        queue_.push(item);
        mlock.unlock();
        cond_.notify_one();
    }
    void M_Push(T&& item)
    {
        unique_lock<mutex> mlock(mutex_);
        queue_.push(std::move(item));
        mlock.unlock();
        cond_.notify_one();
    }
    bool M_Empty(void)
    {
        unique_lock<mutex> mlock(mutex_);
        return queue_.empty();
    }


};