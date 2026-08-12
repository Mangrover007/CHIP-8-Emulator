#include <fstream>
#include <iostream>

int main()
{
    std::fstream file("input.txt", std::ios::ate | std::ios::in);

    if (!file)
    {
	std::cout << "Uh oh, input.txt could not be opened for modification!\n";
    }

    int size = file.tellg();
    std::cout << "input.txt is " << size << " bytes in size\n";

    return 0;
}

