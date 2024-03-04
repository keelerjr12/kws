#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

#ifndef NDEBUG
#define LOG_DEBUG(str) 
#else
#define LOG_DEBUG(str)  std::cout << "DEBUG: " << str  << "\t(" << __FILE__ << ":" << __LINE__ << ")\n";
#endif // NDEBUG

#endif //LOGGER_H
