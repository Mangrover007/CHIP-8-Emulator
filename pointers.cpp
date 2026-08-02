/*
 * var: 0x00F0 and holds 0x000A (10)
 * LD Vx, var --> load the value at var into Vx
 * registers[Vx] = memory[var]
 *
 * var: 0x00F0 and holds 0x000A (10)
 * LD Vx, &var --> load the var itself into Vx
 * registers[Vx] = var
 *
 * Let's say that a variable "ptr" holds, in its value, the address of var.
 * ptr = var ;; instead of ptr = memory[var]
 * LD ptr, var
 * ptr = var
 *
 * Then, when dereferencing, I guess the assembly looks something like this:
 * LD Vx, [ptr]
 * registers[Vx] = memory[memory[ptr]]
 *
 * A double pointer would be:
 * LD Vx, [[dptr]]
 * registers[Vx] = memory[memory[memory[dptr]]]
 *
 * That is my understanding
 */

template<typename T>
constexpr bool islvalue(T&)
{
    return true;
}

template<typename T>
constexpr bool islvalue(T&&)
{
    return false;
}

#include <iostream>

void print(int y)
{
    std::cout << islvalue(y) << '\n';
}

int foo();
double goo();
void hoo(const int& x)
{
    std::cout << x << '\n';
}

void SelectionSort(int* array, int size, bool (*comp)(int, int))
{
    if (!array)
	return;

    for (int start_index{ 0 }; start_index < size - 1; ++start_index)
    {
	int swap_index = start_index;

	for (int i{ start_index + 1 }; i < size; ++i)
	{
	    if (comp(array[swap_index], array[i]))
		swap_index = i;
	}

	std::swap(array[swap_index], array[start_index]);
    }
}

bool ascending(int x, int y)
{
    return x > y;
}

bool descending(int x, int y)
{
    return x < y;
}

void printArray(int* array, int size)
{
    for (int i{ 0 }; i < size; ++i)
    {
	std::cout << array[i] << ' ';
    }
    std::cout << '\n';
}

int main()
{
    int array[] = {3, 1, 4, 2, 5};
    int size = 5;
    using sort_fn = bool (*)(int, int);
    sort_fn fn_ptr[]{ &ascending, &descending };

    for (int i{ 0 }; i < 10; ++i)
    {
        SelectionSort(array, size, fn_ptr[i & 1]);
	printArray(array, size);
    }

    return 0;

    // The only problem remaining now is holding a table and also pointers to functions in the
    // same "master table"
    //
    // I don't know how the guide does it, but I know that (void *) in C++ is something close
    // to "unknown type"

    // My solution
    int array2[] = {3, 1, 4, 2, 5};
    int size2 = 5;
    using sort_fn = bool (*)(int, int);
    void* fn_ptr2[]{ (void *)&ascending, (void *)&descending, (void *)&SelectionSort };

    for (int i{ 0 }; i < 10; ++i)
    {
        SelectionSort(array, size, (sort_fn)fn_ptr[i & 1]);
	printArray(array, size);
    }

    return 0;

    // Better yet, just make the other tables functions themselves
    // I am stupid

    /*
    int x = 10;
    std::cout << islvalue(&print) << '\n';
    print(*(&x));

    void (*fn_ptr)(const int&) { &hoo };
    std::cout << islvalue(fn_ptr) << '\n';
    fn_ptr(2);

    return 0;
    */
}

