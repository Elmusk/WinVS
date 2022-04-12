#include <iostream>

void swap(int& a, int& b)
{
	int t;
	t = a;
	a = b;
	b = t;
}

//int main()
//{
//	int x1(4);
//	int x2(2);
//	swap(x1, x2);
//	std::cout << x1 << ", " << x2 << std::endl;
//	system("pause");
//}