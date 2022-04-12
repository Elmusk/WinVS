#include <iostream>

double arctan(double x)
{
	double head = x;
	int tail = 1;
	double art = 0;

	while (double(head / tail) > 1e-15)
	{
		art = (tail % 4 == 1) ? (art + head/tail) : (art - head/tail);
		head *= x * x;
		tail += 2;
		std::cout << "----------------\n";
		std::cout << tail << std::endl; 
		std::cout << "----------------\n";
	}

	return art;
}

//int main()
//{
//	double a = 16.0 * arctan(1.0/5.0);
//	double b = 4.0 * arctan(1.0/239.0);
//	double pi = a - b;
//
//	std::cout << pi << std::endl;
//	system("pause");
//	return 0;
//}