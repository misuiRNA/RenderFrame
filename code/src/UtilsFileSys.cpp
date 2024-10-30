#include "Utils.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <iostream>
#include <vector>


std::string GetCurPath() {
    std::string path;
    char buffer[FILENAME_MAX];
    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
        path = buffer;
    } else {
        std::cerr << "get cur path error!" << std::endl;
    }
    return path;
}

std::string ReadFile(const std::string& path) {
    std::string content;
    std::ifstream fileStream;
    fileStream.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try  {
        fileStream.open(path.c_str());
        std::stringstream contentStream;
        contentStream << fileStream.rdbuf();
        fileStream.close();
        content = contentStream.str();
    }
    catch(std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << path << std::endl;
    }
    return content;
}

