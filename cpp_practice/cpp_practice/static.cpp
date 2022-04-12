#include <iostream>

int i = 1;

//int main()
//{
//	static int a;
//	int b = -10;
//	int c = 0;
//
//	void other(void);
//
//	std::cout << "-------Main--------" << std::endl;
//	std::cout << "i=" << i << ", a=" << a << ", b=" << b << ", c=" << c << std::endl;
//	
//	c += 8;
//	other();
//
//	std::cout << "-------Main--------" << std::endl;
//	std::cout << "i=" << i << ", a=" << a << ", b=" << b << ", c=" << c << std::endl;
//	
//	i += 10;
//	other();
//	other();
//
//	system("pause");
//}

void other(void)
{
	static int a = 2;
	static int b;
	int c = 10;

	a += 2;
	i += 32;
	c += 5;
	
	std::cout << "-------Other--------" << std::endl;
	std::cout << "i=" << i << ", a=" << a << ", b=" << b << ", c=" << c << std::endl;

	b = a;
}