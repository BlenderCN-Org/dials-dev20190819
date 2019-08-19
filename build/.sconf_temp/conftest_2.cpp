#include <typeinfo>
#include <iostream>
#include <stddef.h>

int
main()
{
  std::cout
    << static_cast<int>(typeid(unsigned short) == typeid(size_t))
    << static_cast<int>(typeid(unsigned) == typeid(size_t))
    << static_cast<int>(typeid(unsigned long) == typeid(size_t))
    << static_cast<int>(typeid(unsigned long long) == typeid(size_t))
    << std::endl;
  return 0;
}
