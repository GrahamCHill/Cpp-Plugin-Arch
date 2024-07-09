// libmylibrary.cpp
// g++ -dynamiclib -o libmylibrary.dylib libmylibrary.cpp # macOS
// g++ -shared -o libmylibrary.so libmylibrary.cpp # Linux
#include <iostream>

extern "C" void execute() {
    std::cout << "Hello from libmylibrary!" << std::endl;
}
