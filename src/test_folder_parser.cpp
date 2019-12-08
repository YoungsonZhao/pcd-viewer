/**
*  @copyright  Copyright (c) 2019 Institute of Cyber-Systems & Control
*                            Zhejiang University
*                            All rights reserved
*
*  @file     test_folder_parser.cpp
*  @version  1.0.0
*  @author   Dr. Zhao Yongsheng
*            Email: zhaoyongsheng@zju.edu.cn
*            Blog: www.zhaoyongsheng.com
*  @date     2019/02/22 17:25:56
*  @details       
*/

#include <iostream>
#include "folder_parser.h"

int main(int argc, char** argv)
{
    if(argc == 1)
    {
        std::cout << "[main] >> Please input the directory path." << std::endl;
        return -1;
    }
    iplusbot::DirContent dirContent;
    iplusbot::folder_parser(argv[1], dirContent);
    dirContent.print();
    std::vector<std::string> files = dirContent.filesWithSuffix("cmake", 10);
    for(size_t i=0; i<files.size(); i++)
    {
        std::cout << files[i] << std::endl;
    }
}