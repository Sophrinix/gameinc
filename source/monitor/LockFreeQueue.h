#ifndef LOCKFREEQUEUE_H
#define LOCKFREEQUEUE_H
#include <list>

namespace nrp
{
const size_t MAX_QUEUE_SIZE = 100;
/*
	Очеред элементов типа T без блокировки
*/
template <typename T>
struct LockFreeQueue
{
	LockFreeQueue()
	{
		queue_.push_back(T());
		iHead = queue_.begin();
		iTail = queue_.end();
	}

	void Produce(const T& t)
	{
		queue_.push_back(t);
		iTail = queue_.end();
		queue_.erase(queue_.begin(), iHead);
	}

	bool Consume(T& t)
	{
		typename T_List::iterator iNext = iHead;
		++iNext;
		if (iNext != iTail)
		{
			iHead = iNext;
			t = *iHead;
			return true;
		}
		return false;
	}

    size_t size()
    {
        return queue_.size(); 
    }
private:
	typedef std::list<T> T_List;
	T_List queue_;
	typename T_List::iterator iHead, iTail;
};

};
#endif