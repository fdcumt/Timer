#pragma once
#include <iostream>
using namespace std;

class FBaseTimerTrigger
{
public:
	FBaseTimerTrigger(int FrameNum)
		:m_FrameNum(FrameNum)
	{

	}
	virtual void Invoke() = 0;

	int GetFrameNun() { return m_FrameNum; }

protected:
	int m_FrameNum;
};

template <class T>
class FTimerTrigger : public FBaseTimerTrigger
{
public:
	typedef void (T::*FuncPtr)();
	FTimerTrigger(T *pObj, FuncPtr pFunc, int FrameNum)
		: FBaseTimerTrigger(FrameNum)
		, pObj(pObj)
		, pFunc(pFunc)
	{

	}

	virtual void Invoke() override
	{
		(pObj->*pFunc)();
	}

private:
	T *pObj;
	FuncPtr pFunc;
};

class FTimerManager
{
public:
	FTimerManager()
	{
		for (int i = 0; i < TriggerMaxNum; ++i)
		{
			m_pTriggers[i] = nullptr;
		}
	}

	template <class T>
	void SetTimer(T *pObj, void (T::*pFunc)(), int FrameNum, int TriggerIndex)
	{
		m_pTriggers[TriggerIndex] = new FTimerTrigger<T>(pObj, pFunc, FrameNum);
	}

	void Tick(int FrameNum)
	{
		for (int i = 0; i < TriggerMaxNum; ++i)
		{
			if (m_pTriggers[i] != nullptr && m_pTriggers[i]->GetFrameNun() == FrameNum)
			{
				m_pTriggers[i]->Invoke();
				m_pTriggers[i] = nullptr;
			}
		}
	}

private:
	static const int TriggerMaxNum = 10;
	FBaseTimerTrigger *m_pTriggers[TriggerMaxNum];
};

class A
{
public:
	void Delegate1()
	{
		cout << "A1" << endl;
	}

	void Delegate2()
	{
		cout << "A2" << endl;
	}
};

class B
{
public:
	void Delegate1()
	{
		cout << "B1" << endl;
	}

	void Delegate2()
	{
		cout << "B2" << endl;
	}
};

class C
{
public:
	void Delegate1()
	{
		cout << "C1" << endl;
	}

	void Delegate2()
	{
		cout << "C2" << endl;
	}
};

int main()
{
	FTimerManager timerManager;
	A a;
	B b;
	C c;

	timerManager.SetTimer(&a, &A::Delegate1, 3, 1);
	timerManager.SetTimer(&a, &A::Delegate2, 6, 2);
	timerManager.SetTimer(&b, &B::Delegate1, 5, 3);
	timerManager.SetTimer(&b, &B::Delegate2, 6, 4);
	timerManager.SetTimer(&c, &C::Delegate2, 200, 5);
	timerManager.SetTimer(&c, &C::Delegate2, 60, 6);

	for (int framNum = 1; framNum < 100000; ++framNum)
	{
		timerManager.Tick(framNum);
	}

	system("Pause");
	return 0;
}

