/*
2026-02-15
data.txt 파일 읽기
*/

#include <iostream>
#include <fstream>
#include <string>

int main(void)
{
    // 파일 열기
    std::ifstream file("data.txt");
    if (!file.is_open())
    {
        std::cerr << "Failed to open file" << std::endl;
        return 1;
    }
    
    // 전체 읽기
    std::string content;
    std::string line;
    while (std::getline(file, line))
    {
        content += line + "\n";
    }
    
    // 출력
    std::cout << "File data:" << std::endl;
    std::cout << content;
    
    file.close();
    return 0;
}