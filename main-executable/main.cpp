#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace fs = std::filesystem;

void loadAndExecuteLibraries(const std::string& directory) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        std::string path = entry.path().string();

#if defined(_WIN32) || defined(_WIN64)
        if (entry.path().extension() == ".dll") {
            HINSTANCE handle = LoadLibrary(path.c_str());
            if (!handle) {
                std::cerr << "Warning: Could not load " << path << ": " << GetLastError() << std::endl;
                continue;
            }

            typedef void (*execute_t)();
            execute_t execute = (execute_t)GetProcAddress(handle, "execute");
            if (!execute) {
                std::cerr << "Warning: Could not find execute in " << path << ": " << GetLastError() << std::endl;
                FreeLibrary(handle);
                continue;
            }

            // Call the execute function
            execute();

            // Free the library
            FreeLibrary(handle);
        }
#elif defined(__APPLE__) || defined(__linux__)
        if (entry.path().extension() == ".dylib" || entry.path().extension() == ".so") {
            void* handle = dlopen(path.c_str(), RTLD_LAZY);
            if (!handle) {
                std::cerr << "Warning: Could not load " << path << ": " << dlerror() << std::endl;
                continue;
            }

            dlerror(); // Clear any existing error
            void (*execute)() = (void (*)())dlsym(handle, "execute");
            const char* dlsym_error = dlerror();
            if (dlsym_error) {
                std::cerr << "Warning: Could not find execute in " << path << ": " << dlsym_error << std::endl;
                dlclose(handle);
                continue;
            }

            // Call the execute function
            execute();

            // Close the handle
            dlclose(handle);
        }
#endif
    }
}

int main() {
    std::string directory = "./"; // Directory containing .dylib files
    loadAndExecuteLibraries(directory);

    std::cout << "Continuing execution of the program..." << std::endl;

    // Your code here

    return 0;
}
