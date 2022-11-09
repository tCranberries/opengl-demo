//
// Created by wzw on 22-11-3.
//

#include <filesystem>
#include <iostream>

int main_1() {


    std::filesystem::path currentPath = std::filesystem::current_path();
    std::cout << "current path: " << currentPath.c_str() << std::endl;
    if (currentPath.has_root_directory()) {
        std::cout << "root_dir" << currentPath.root_directory() << std::endl;
    }
    if (currentPath.has_parent_path()) {
        std::cout << "parent_path: " << currentPath.parent_path() << std::endl;
    }

    if (currentPath.has_relative_path()) {
        std::cout << "relative path: " << currentPath.relative_path() << std::endl;
    }
    if (currentPath.has_root_name()) {
        std::cout << "root name: " << currentPath.root_name() << std::endl;
    }

    if (currentPath.has_root_path()) {
        std::cout << "root path: " << currentPath.root_path() << std::endl;
    }

    return 0;
}