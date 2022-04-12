#include <iostream>
#include <cstdlib>
#include <ctime>

int roll_dice()
{
	int dim1 = rand() % 6 + 1;
	int dim2 = rand() % 6 + 1;
	int sum = dim1 + dim2;

	std::cout << "sum = " << dim1 << " + " << dim2 << std::endl;

	return sum;
}

//int main()
//{
//	int flag;
//	srand(time(0));
//	int sum = roll_dice();
//	int selfdim;
//
//	switch (sum)
//	{
//		case 7:
//		case 11:
//			flag = 1;
//			break;
//		case 2:
//		case 3:
//		case 12:
//			flag = 2;
//			break;
//		default:
//			flag = 0;
//			selfdim = sum;
//			break;
//	}
//
//	while (flag == 0)
//	{
//		sum = roll_dice();
//		if (sum == selfdim)
//			flag = 1;
//		else if (sum == 7)
//			flag = 2;
//	}
//
//	if (flag == 1)
//		std::cout << "player wins\n";
//	else
//		std::cout << "player loses\n";
//
//	system("pause");
//	return 0;
//}
