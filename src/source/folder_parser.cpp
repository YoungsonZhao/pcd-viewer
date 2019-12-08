/**
*  @copyright  Copyright (c) 2019 Institute of Cyber-Systems & Control
*                            Zhejiang University
*                            All rights reserved
*
*  @file     folder_parser.cpp
*  @version  1.0.0
*  @author   Mr. Qu Shuailong & Dr. Zhao Yongsheng
*            Email: zhaoyongsheng@zju.edu.cn
*            Blog: www.zhaoyongsheng.com
*  @date     2019/02/21 21:50:23
*  @details       
*/

#include "folder_parser.h"
// #include <sys/stat.h>

namespace iplusbot
{
    void DirContent::addDir(const std::string& dirPath)
    {
        DirContent dirContent;
        dirContent.m_path = this->m_path;
        dirContent.m_path.append(dirPath);
        dirContent.m_path.append("/");
        this->m_dirs.push_back(dirContent);
    }

    void DirContent::addFile(const std::string& fileName)
    {
        // std::string filePath = this->m_path;
        // filePath.append(fileName);
        this->m_files.push_back(fileName);
    }

    std::vector<std::string> DirContent::filesWithSuffix(const std::string& suffix, const unsigned char maxDepth)
    {
        std::vector<std::string> fileList;
        for(std::size_t i=0; i<this->m_files.size(); i++)
        {
            // std::cout << "[filesWithSuffix] >> " << this->m_files[i] << std::endl;
            if(checkSuffix(this->m_files[i], suffix))
            {
                std::string target = this->m_path;
                target.append(this->m_files[i]);
                // std::cout << "[filesWithSuffix] >> " << target << std::endl;
                fileList.push_back(target);
            }
        }
        if(maxDepth == 0)
        {
            return fileList;
        }
        else
        {
            for(unsigned char i=0; i<maxDepth; i++)
            {
                std::vector<DirContent> dirContent;
                parseDirs(i, dirContent);
                for(std::size_t j=0; j<dirContent.size(); j++)
                {
                    for(std::size_t k=0; k<dirContent[j].m_files.size(); k++)
                    {
                        if(checkSuffix(dirContent[j].m_files[k], suffix))
                        {
                            fileList.push_back(dirContent[j].m_path+dirContent[j].m_files[k]);
                        }
                    }
                }
            }
        }
    }

    std::vector<std::string> DirContent::files(const unsigned char maxDepth)
    {
        std::vector<std::string> fileList;
        for(std::size_t i=0; i<this->m_files.size(); i++)
        {
            fileList.push_back(this->m_files[i]);
        }
        if(maxDepth == 0)
        {
            return fileList;
        }
        else
        {
            for(unsigned char i=0; i<maxDepth; i++)
            {
                std::vector<DirContent> dirContent;
                parseDirs(i, dirContent);
                for(std::size_t j=0; j<dirContent.size(); j++)
                {
                    for(std::size_t k=0; k<dirContent[j].m_files.size(); k++)
                    {
                        fileList.push_back(dirContent[j].m_files[k]);
                    }
                }
            }
        }
    }

    void DirContent::parseDirs(const unsigned char targetDepth, std::vector<DirContent>& dirContent)
    {
        // std::cout << "[parseDirs] >> " << targetDepth << std::endl;
        if(targetDepth == -1)
        {
            return;
        }
        if(targetDepth == 0)
        {
            for(std::size_t i=0; i<this->m_dirs.size(); i++)
            {
                dirContent.push_back(this->m_dirs[i]);
            }
        }
        else
        {
            for(std::size_t i=0; i<this->m_dirs.size(); i++)
            {
                // dirContent.push_back(this->m_dirs[i]);
                if(this->m_dirs[i].is_leaf() == false)
                {
                    this->m_dirs[i].parseDirs(targetDepth-1, dirContent);
                }
            }
        }
    }

    std::vector<DirContent> DirContent::leafDirs() const
    {
        std::vector<DirContent> childDir;
        for(std::size_t i=0; i<this->m_dirs.size(); i++)
        {
            if(this->m_dirs[i].is_leaf())
            {
                childDir.push_back(this->m_dirs[i]);
            }
        }

        return childDir;
    }


    std::vector<DirContent> DirContent::noneLeafDirs() const
    {
        std::vector<DirContent> childDir;
        for(std::size_t i=0; i<this->m_dirs.size(); i++)
        {
            if(this->m_dirs[i].is_leaf() == false)
            {
                childDir.push_back(this->m_dirs[i]);
            }
        }

        return childDir;
    }

    void DirContent::print(const unsigned char maxDepth) const
    {
        std::cout << m_path << std::endl;
        this->printRelative(0);
    }

    void DirContent::printRelative(const unsigned char currtDepth) const
    {
        if(currtDepth > 5)
        {
            std::cout << "|  |  |  |  |  |  |--Warning: The contents of directories whose depth is more that 5 are not printed." << std::endl;
            return;
        }
        std::string tabHead;
        for(unsigned char i=0; i<currtDepth; i++)
        {
            tabHead.append("|  ");
        }
        tabHead.append("|--");
        for(size_t i=0; i<m_files.size(); i++)
        {
            std::cout << tabHead << m_files[i] << std::endl;
        }
        for(size_t i=0; i<m_dirs.size(); i++)
        {
            std::cout << tabHead << m_dirs[i].m_name << std::endl;
            m_dirs[i].printRelative(currtDepth+1);
        }
    }

    bool DirContent::checkSuffix(const std::string& filePath, const std::string& suffix)
    {
        std::size_t length = filePath.length();
        std::size_t suffix_length = suffix.length();
        if(suffix_length >= length)
        {
            return false;
        }
        std::string fileSuffix = filePath.substr(length-suffix_length);
        // std::cout << "[checkSuffix] >> " << fileSuffix << std::endl;


        return (suffix.compare(fileSuffix) == 0);
    }

    bool folder_parser(const std::string& _dirPath, DirContent& dirContent)
    {
        std::string dirPath = _dirPath;
        std::string dirPath_clean;
        std::string::size_type lastBacklash = dirPath.find_last_of('/');
        if(lastBacklash == dirPath.size()-1)
        {
            dirPath_clean = dirPath.substr(0, lastBacklash);
            lastBacklash = dirPath_clean.find_last_of('/');
        }
        else
        {
            dirPath_clean = dirPath;
        }

        std::string dirName = dirPath_clean.substr(lastBacklash+1, dirPath_clean.size()-1);
        dirContent.m_name = dirName;
        dirContent.m_path = dirPath_clean.append("/");

        DIR *directoryPtr;
        struct dirent *directoryEntry;
        if ((directoryPtr = opendir(dirContent.m_path.c_str())) == NULL)
        {
            std::cout << "[folder_parser] >> Could not open directory: " << dirContent.m_path << std::endl;
            return false;
        }
        
        while ((directoryEntry = readdir(directoryPtr)) != NULL)
        {
            if (directoryEntry->d_name[0] == '.')
            {
                continue;
            }
            std::string entry(directoryEntry->d_name);
            std::string fullPath = dirContent.m_path;
            fullPath.append(entry);
            // std::cout << "[folder_parder] >> entry: " << entry << std::endl; 
            if(opendir(fullPath.c_str()) == NULL)
            {
                dirContent.addFile(entry);
            }
            else
            {
                dirContent.addDir(entry);
                folder_parser(fullPath, dirContent.m_dirs.back());
            }
        }
        closedir(directoryPtr);

        // if (dirContent.is_empty())
        // {
        //     std::cout << "[folder_parser] >> There is no file in the directory: " << dirPath << std::endl;
        //     return false;
        // }
        return true;
    }
}