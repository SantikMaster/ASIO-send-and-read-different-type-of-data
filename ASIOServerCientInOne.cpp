// ASIOServerCientInOne.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#define ASIO_STANDALONE
#define _WIN32_WINNT 0x0601

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <queue>
#include "net.h"
#include <type_traits>
enum class CustomMsg : uint32_t
{
    IntInt,
    Double,
};


using asio::ip::tcp;

template <class T>
struct MyMess
{
private:
	std::uint32_t ID;
	std::uint32_t size = 0;
	std::uint32_t elements = 0;
public:
	MyMess()
	{
		if (std::is_same<T, char>())ID = 0;
		if (std::is_same<T, int>())ID = 1;
		if (std::is_same<T, float>())ID = 2;
		if (std::is_same<T, bool>())ID = 3;
		if (std::is_same<T, double>())ID = 4;
		if (std::is_same<T, std::string>())ID = 5;
	}
	void push(const T& message)
	{
		A.push_back(message);
		elements++;
		size = sizeof(*this);
	}
	void print()
	{
		for (auto it : A)
		{
			std::cout << it << '\t';
		}
		std::cout << '\n';
	}
	std::vector<T> A;
	const char End[5] = "end$";
};
struct Header
{
	
	std::uint32_t ID;
	std::uint32_t size = 0;
	std::uint32_t elements = 0;
	
};

template <class T>
void HandleMessage(MyMess<T> &InputMess, asio::streambuf &buf, int elements)
{
	memcpy(&InputMess, asio::buffer_cast<const void*>(buf.data()), sizeof(InputMess));
	InputMess.print();
	buf.consume(sizeof(InputMess));
}


int main()
{
	MyMess<int> my_ms1;

	my_ms1.push(1222);
	my_ms1.push(567);
	my_ms1.push(21222);
	my_ms1.push(12222);

	MyMess<int> Mint;
	MyMess<float> Mfloat;

	MyMess<int> IIntMess;
	MyMess<char> ICharMess;
	MyMess<float> IFloatMess;
	MyMess<bool> IBoolMess;
	MyMess<double> IDoubleMess;
	MyMess<std::string> IStringMess; 

    asio::io_context Context;
    tcp::acceptor Acceptor(Context, tcp::endpoint(tcp::v4(), 1337));

    asio::streambuf buf(65536);
    tcp::socket Socket(Context);
  
    std::error_code ec;
    std::string str = "message\n", str2;

    tcp::socket Socket2(Context);
    tcp::resolver Resolver(Context);
    auto endpoint = Resolver.resolve("127.0.0.1", "1337", ec);
    asio::connect(Socket2, endpoint, ec);
    Acceptor.accept(Socket);
	
	int temp = 0;
    while (true)
    {
  

		if (Socket.is_open())
		{
			asio::write(Socket, asio::buffer(&my_ms1, sizeof(my_ms1)), ec);
			asio::read_until(Socket2, buf, "end$", ec);


			Header head;
			memcpy(&head, asio::buffer_cast<const void*>(buf.data()), 16);
			std::cout << head.ID << '\t' << head.size << '\t' << head.elements << '\t' << "\n";

			switch (head.ID)
			{
				case 0:
					HandleMessage(ICharMess, buf, head.elements);
					break;
				case 1:
					HandleMessage(IIntMess, buf, head.elements);
					break;
				case 2:
					HandleMessage(IFloatMess, buf, head.elements);
					break;
				case 3:
					HandleMessage(IBoolMess, buf, head.elements);
					break;
				case 4:
					HandleMessage(IDoubleMess, buf, head.elements);
					break;
				case 5:
					HandleMessage<std::string>(IStringMess, buf, head.elements);
					break;
		
			}
		}
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Hello World!\n";
    
}

// Use this for simplier data

/*      if (Socket.is_open())
		{
			asio::write(Socket, asio::buffer(str), ec);

			asio::read_until(Socket2, buf, "\n", ec);
			if (ec) std::cout << ec;

			std::stringstream ss;
			ss << std::istream(&buf).rdbuf();

			str2 = ss.str();
		}
		std::cout << str2 <<"\n";


		data1.a += 1;
		data1.b-=1;
		auto mutable_buffer = asio::buffer(data2);
		if (Socket.is_open())
		{
			asio::write(Socket, asio::buffer(&data1, sizeof(data1)), ec);
			asio::read(Socket2, asio::buffer(&data2, sizeof(data2)), ec);
		}
		std::cout << data2[0].a <<"\t"<< data2[0].b << "\n";


		if (Socket.is_open())
		 {
			 asio::write(Socket, asio::buffer(&data1, sizeof(data1)), ec);
			 asio::read_until(Socket2, buf, "\n", ec);

			 object o;
			 memcpy(&o, asio::buffer_cast<const void*>(buf.data()), sizeof(o));
			 buf.consume(sizeof(o));
			 std::cout << o.a << "\t" << o.b << "\n";

			 o.a = 0;
			 o.b = 0;
		 }
		 	my_ms1.A.clear();
			 temp++;
			 my_ms1.push("fghgfh0"+ std::to_string(temp));*/

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
