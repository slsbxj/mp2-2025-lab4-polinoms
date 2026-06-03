cmake_minimum_required(VERSION 3.10)
project(PolinomLab4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основное приложение
add_executable(PolinomApp 
    src/main.cpp
    src/polinom.cpp
)

target_include_directories(PolinomApp PRIVATE include)
