#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <filesystem>
#include <array>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

namespace fs = std::filesystem;

constexpr size_t MAX_PATH_LENGTH = 4096;

void setEnvironmentVariable(const std::string& name, const std::string& value) {
#ifdef _WIN32
    _putenv_s(name.c_str(), value.c_str());
#else
    setenv(name.c_str(), value.c_str(), 1);
#endif
}

std::string getExecutablePath() {
    std::array<char, MAX_PATH_LENGTH> buffer{};

#ifdef _WIN32
    DWORD path_len = GetModuleFileNameA(NULL, buffer.data(), static_cast<DWORD>(buffer.size()));
    if (path_len == 0 || path_len == buffer.size()) {
        throw std::runtime_error("Failed to get executable path");
    }
#else
    ssize_t path_len = readlink("/proc/self/exe", buffer.data(), buffer.size() - 1);
    if (path_len <= 0) {
        // Fallback method
        std::ifstream maps("/proc/self/maps");
        std::string line;
        while (std::getline(maps, line)) {
            if (line.find(" r-xp ") != std::string::npos) {
                size_t pos = line.find('/');
                if (pos != std::string::npos) {
                    return line.substr(pos);
                }
            }
        }
        throw std::runtime_error("Failed to get executable path");
    }
    buffer[path_len] = '\0';
#endif

    return std::string(buffer.data());
}

void setEnvironmentVariables() {
    try {
        fs::path basePath = fs::path(getExecutablePath()).parent_path();
        std::string baseDir = basePath.string();

        setEnvironmentVariable("BASE_DIR", baseDir);
        setEnvironmentVariable("PATH", baseDir + "/bin/:" + (std::getenv("PATH") ? std::getenv("PATH") : ""));
        setEnvironmentVariable("LD_LIBRARY_PATH", baseDir + "/lib/:" + baseDir + ":" + (std::getenv("LD_LIBRARY_PATH") ? std::getenv("LD_LIBRARY_PATH") : ""));
        setEnvironmentVariable("QML_IMPORT_PATH", baseDir + "/qml/:" + (std::getenv("QML_IMPORT_PATH") ? std::getenv("QML_IMPORT_PATH") : ""));
        setEnvironmentVariable("QML2_IMPORT_PATH", baseDir + "/qml/:" + (std::getenv("QML2_IMPORT_PATH") ? std::getenv("QML2_IMPORT_PATH") : ""));
        setEnvironmentVariable("QT_PLUGIN_PATH", baseDir + "/plugins/:" + (std::getenv("QT_PLUGIN_PATH") ? std::getenv("QT_PLUGIN_PATH") : ""));
        setEnvironmentVariable("QTWEBENGINEPROCESS_PATH", baseDir + "/bin/QtWebEngineProcess");
        setEnvironmentVariable("QTDIR", baseDir);
        setEnvironmentVariable("CQT_PKG_ROOT", baseDir);
        setEnvironmentVariable("CQT_RUN_FILE", baseDir + "/FrameFlow.sh");
        setEnvironmentVariable("QT_QPA_PLATFORM_PLUGIN_PATH", baseDir + "/plugins/platforms:" + (std::getenv("QT_QPA_PLATFORM_PLUGIN_PATH") ? std::getenv("QT_QPA_PLATFORM_PLUGIN_PATH") : ""));
    } catch (const std::exception& e) {
        std::cerr << "Error setting environment variables: " << e.what() << std::endl;
        throw;
    }
}

void launchApplication() {
    try {
        fs::path basePath = fs::path(getExecutablePath()).parent_path();
        std::string appPath = (basePath / "bin" / "FrameFlow").string();

        // Add executable permissions
        fs::permissions(appPath, fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec,
                        fs::perm_options::add);

        std::cout << "Added executable permissions to: " << appPath << std::endl;

        std::cout << "Launching FrameFlow application from: " << appPath << std::endl;
        // Launch the main application
        int result = std::system(appPath.c_str());

        if (result != 0) {
            throw std::runtime_error("Failed to launch FrameFlow application. Error code: " + std::to_string(result));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error launching application: " << e.what() << std::endl;
        throw;
    }
}

int main() {
    try {
        std::cout << "FrameFlow Launcher starting..." << std::endl;
        setEnvironmentVariables();
        launchApplication();
    } catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
