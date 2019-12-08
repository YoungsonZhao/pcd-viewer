/**
*  @copyright  Copyright (c) 2019 Institute of Cyber-Systems & Control
*                            Zhejiang University
*                            All rights reserved
*
*  @file     folder_parser.hpp
*  @version  1.0.0
*  @author   Mr. Qu Shuailong & Dr. Zhao Yongsheng
*            Email: zhaoyongsheng@zju.edu.cn
*            Blog: www.zhaoyongsheng.com
*  @date     2019/02/21 21:50:23
*  @details       
*/
#ifndef FOLDER_PARSER_H
#define FOLDER_PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
// #include <sys/stat.h>

namespace iplusbot
{
    /**
    *  @struct   DirContent
    *  @author   Dr. Zhao Yongsheng
    *  @date     2019/03/04 17:18:33
    *  @details  This struct stores the hierarchical contents of a directory, including all the files and folders hierarchically. 
    */
    struct DirContent
    {
        public:
        DirContent() =default;
        ~DirContent() =default;

        /**
        *  @var       m_name
        *  @details   m_name stores the name of a directory.
        *  @see       m_path
        */
        std::string m_name;

        /**
        *  @var       m_path
        *  @details   m_path stores the path of a directory.
        *  @see       m_name
        */
        std::string m_path;

        /**
        *  @var       m_files
        *  @details   m_files lists all the files of a directory.
        *  @see       m_dirs
        */
        std::vector<std::string> m_files;

        /**
        *  @var       m_dirs
        *  @details   m_dirs stores the sub-folders of a directory.
        *  @see       m_files
        */
        std::vector<DirContent> m_dirs;

        /**
        *  @fn          addDir
        *  @details     This function adds a directory path of a sub-folder in the current folder to m_dirs 
        *  @param[in]   dirPath The directory path of the sub-folder that needs to be added
        *  @return      void
        *  @see         m_dirs
        */
        void addDir(const std::string& dirPath);

        /**
        *  @fn          addFile
        *  @details     This function adds a file in the current folder to m_files 
        *  @param[in]   fileName The name of the file that needs to be added
        *  @return      void
        *  @see         m_files
        */
        void addFile(const std::string& fileName);

        /**
        *  @fn          is_empty
        *  @details     This function check whether the current folder is empty or not
        *  @return      bool true for empty and false for non-empty
        *  @see         m_files and m_dirs
        */
        inline bool is_empty() const
        {
            return (this->m_dirs.size() + this->m_files.size() == 0);
        }

        /**
        *  @fn          is_leaf
        *  @details     This function check whether the current folder is a leaf folder or not. A leaf folder is defined as a folder that doesn't have a sub-folder.
        *  @return      bool true for leaf folder and false for non-lead folder
        *  @see         m_dirs
        */
        inline bool is_leaf() const
        {
            return (m_dirs.size() == 0);
        }

        /**
        *  @fn         filesWithSuffix
        *  @details    This function returns all the files in the current folder with a specific suffix and a specific hierarchical depth
        *  @param[in]  suffix the file suffix
        *  @param[in]  maxDepth the maximum hierarchichal depth, the default value is 0 which represents the current folder
        *  @return     std::vector<std::string> the files names 
        */
        std::vector<std::string> filesWithSuffix(const std::string& suffix, const unsigned char maxDepth = 0);

        /**
        *  @fn         files
        *  @details    This function returns all the files in a specific hierarchical depth
        *  @param[in]  maxDepth the maximum hierarchichal depth, the default value is 0 which represents current folder
        *  @return     std::vector<std::string> the files names 
        */
        std::vector<std::string> files(const unsigned char maxDepth = 0);

        /**
        *  @fn         parseDirs
        *  @details    This function parses the current folder and outputs the contens
        *  @param[in]  targetDepth the target hierarchical depth, 0 represents the current folder
        *  @param[out] the directory contents in the current folder
        *  @return     void 
        */
        void parseDirs(const unsigned char targetDepth, std::vector<DirContent>& dirContent);

        /**
        *  @fn         leafDirs
        *  @details    This function parses the current folder and returns all the leaf directories
        *  @return     std::vector<DirContent> the leaf directories
        *  @see        is_leaf
        */
        std::vector<DirContent> leafDirs() const;

        /**
        *  @fn         noneLeafDirs
        *  @details    This function parses the current folder and returns all the non-leaf directories
        *  @return     std::vector<DirContent> the non-leaf directories
        *  @see        is_leaf
        */
        std::vector<DirContent> noneLeafDirs() const;

        /**
        *  @fn         print
        *  @details    This function print the hierarchical content of the current folder
        *  @param[in]  maxDepth the maximum hierarchichal depth, the default value is 0 which represents current folder
        *  @return     void
        */
        void print(const unsigned char maxDepth=0) const;

        /**
        *  @fn         printRelative
        *  @details    This function print the relative hierarchical content of the current folder
        *  @param[in]  currtDepth the currtDepth hierarchichal depth with respect to root directory
        *  @return     void
        */
        void printRelative(const unsigned char currtDepth) const;
        private:
        /**
        *  @fn         checkSuffix
        *  @details    This function checks whether the file have the specific suffix
        *  @param[in]  filePath the file path that needs to be check
        *  @param[in]  suffix the specific suffix
        *  @return     bool true for the file path contains the suffix and false for not
        */
        bool checkSuffix(const std::string& filePath, const std::string& suffix);
    };

    bool folder_parser(const std::string& _dirPath, DirContent& dirContent);
}
#endif /* FOLDER_PARSER_H */
