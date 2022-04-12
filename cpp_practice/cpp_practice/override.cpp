#include <iostream>

class Time
{
private:
	int hh, mm, ss;
public:
	Time(int h, int m, int s) : hh(h), mm(m), ss(s) {}

	void operator()(int h, int m, int s)
	{
		hh = h;
		mm = m;
		ss = s;
	}

	void operator++()
	{
		ss += 1;
		if (ss == 60)
		{
			ss = 0;
			++mm;
			if (mm == 60)
			{
				mm = 0;
				++hh;
				if (hh == 24)
				{
					hh = 0;
				}
			}
		}
	}

	void show_time()
	{
		std::cout << hh << ":" << mm << ":" << ss << std::endl;
	}
};

//int main()
//{
//	/*Time t1{12, 10, 11};
//	t1.show_time();
//	t1.operator()(23, 20, 34);
//	t1.show_time();
//	t1(10, 10, 10);
//	t1.show_time();*/
//
//	//Time t {23, 59, 59};
//	//++t;
//	//t.show_time();
//
//	//system("pause");
//}