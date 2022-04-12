#include <iostream>

struct Vector2
{
	float x, y;
};

struct Vector4
{
	union
	{
		struct
		{
			float x, y, z, w;
		};

		struct
		{
			Vector2 a, b;
		};
	};

	/*Vector2& get_a()
	{
		return *(Vector2*)&x;
	}

	Vector2& get_b()
	{
		return *(Vector2*)&z;
	}*/
};  

void print_vector2(const Vector2& vector)
{
	std::cout << vector.x << ", " << vector.y << std::endl;
}

//int main()
//{
//	Vector4 vector = { 1.0f, 2.0f, 3.0f, 4.0f };
//	print_vector2(vector.a);
//	print_vector2(vector.b);
//
//	std::cout << "-------------------------\n";
//	vector.z = 500.0f;
//	print_vector2(vector.a);
//	print_vector2(vector.b);
//
//	std::cin.get();
//}