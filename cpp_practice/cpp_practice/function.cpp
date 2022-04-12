#include <iostream>

const float PI = 3.14159;
const float fence_price = 35;
const float concrete_price = 20;

class Circle
{
private:
	float radius;
public:
	Circle(float r) : radius(r) {}
	float circumference() const
	{
		return 2 * PI * radius;
	}
	float area() const
	{
		return PI * radius * radius;
	}
};

//int main()
//{
//	float radius = 4;
//	float fence_cost, concrete_cost;
//
//	Circle pool(radius);
//	Circle pool_rim(radius + 3);
//
//	fence_cost = pool_rim.circumference() * fence_price;
//	std::cout << "Fencing cost is гд" << fence_cost << std::endl;
//
//	concrete_cost = (pool_rim.area() - pool.area()) * concrete_price;
//	std::cout << "Concrete cost is гд" << concrete_cost << std::endl;
//
//	system("pause");
//}
