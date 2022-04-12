#include <iostream>

//int f(int n)
//{
//	if (n == 0)
//		return 1;
//	else
//		return n * f(n-1);
//}

int f(int n, int k)
{
	if ((n == k) || (k == 0))
		return 1;
	else
		return f(n-1, k-1) + f(n-1, k);
}

//int main()
//{
//	std::cout << f(2, 1) << std::endl;
//	system("pause");
//}