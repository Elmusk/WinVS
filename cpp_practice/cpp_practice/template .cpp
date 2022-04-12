#include <iostream>
#include <cstring>

template<class T>
void sort(T* a, int n)
{
	for (int i = 0; i < n; i++)
	{
		int p = i;
		for (int j = i; j < n; j++)
		{
			if (a[p] < a[j])
				p = j;
		}

		T t = a[i];
		a[i] = a[p];
		a[p] = t;
	}
}

template<class T>
void display(T& a, int n)
{
	for (int i = 0; i < n; i++)
		std::cout << a[i] << "\t\n";
}

template<class T>
T max(T a, T b)
{
	return (a > b) ? a : b;
}

template<>char* max<char*>(char* a, char* b)
{
	return (strcmp(a, b) >= 0) ? a: b;
}



template<typename T, int MAXSIZE>
class Stack
{
private:
	T elem[MAXSIZE];
	int top;
public:
	Stack() { top = 0; }

	void push(T e)
	{
		if (full())
		{
			std::cout << "栈已满，无法添加新元素！" << std::endl;
			return;
		}

		elem[++top] = e;
	}

	T pop()
	{
		if (empty())
		{
			std::cout << "Stack is empty, can't pop elements!" << std::endl;
			return 0;
		}

		return elem[top--];
	}

	bool empty()
	{
		if (top <= -1)
			return 1;
		else
			return 0;
	}

	void set_empty()
	{
		top = -1;
	}

	bool full()
	{
		if (top >= MAXSIZE + 1)
			return 1;
		else
			return 0;
	}
};

//int main()
//{
//	Stack<int, 10> istack;
//	Stack<char, 10> cstack;
//
//	istack.set_empty();
//	cstack.set_empty();
//
//	std::cout << "-----int stack------\n";
//	int i;
//	for (i = 0; i < 11; i++)
//		istack.push(i);
//
//	for (i = 0; i < 11; i++)
//		std::cout << istack.pop() << "\t";
//
//	std::cout << "-----char stack------\n";
//	cstack.push('A');
//	cstack.push('B');
//	cstack.push('C');
//	cstack.push('E');
//	cstack.push('O');
//	cstack.push('P');
//
//	for (i = 0; i < 7; i++)
//		std::cout << cstack.pop() << "\t";
//	std::cout << std::endl;
//
//	system("pause");
//}





//int main()
//{
//	float c = 5.1, d = 3.2;
//	std::cout << "2, 3的最大值：" << max(2, 3) << std::endl;
//	std::cout << "c, d的最大值：" << max(c, d) << std::endl;
//	std::cout << max("xbv", "xyce") << std::endl;
//
//	system("pause");
//}

//int main()
//{
//	int a[] = {1,41,3,5,8,22,15};
//	char b[] = {'a', 'd', 't', 'd', 's', 'i', 'e', 'g', 'l'};
//	sort(a, 7);
//	sort(b, 9);
//	display(a, 7);
//	display(b, 9);
//
//	system("pause");
//}