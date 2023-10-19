#include <dirent.h>
// #include "dirent.h"
#include <iostream>
#include <string>
#include <vector>

class FileNode{
protected:
    std::string fileName;
public:
    FileNode(const std::string&name):fileName(name){}
    virtual void add(FileNode* fileNode)=0;
    virtual void display(int depth)=0;
};

class File:public FileNode{
public:
    File(const std::string& name):FileNode(name){}
    void add(FileNode* fileNode) override{
         printf("Cannot add to a File");
    }
    void display(int depth){
        // std::string out=std::string(depth,'-');
        // std::cout<<out<<fileName<<std::endl;
        // std::string out=std::string(depth/2,' ');
        // out+='|';
        // out+=std::string(2,'-');
        // std::cout<<out<<fileName<<std::endl;
        std::string indentation(depth, ' ');
        std::cout << indentation << "|-- " << fileName << std::endl;
    }
};

class Folder: public FileNode{
private:
    std::vector<FileNode*>children;
public:
    Folder(const std::string& name):FileNode(name){}
    void add(FileNode* fileNode) override{
         children.emplace_back(fileNode);
    }
    void display(int depth){
        // std::string out=std::string(depth/2,' ');
        // out+='|';
        // out+=std::string(2,'-');
        // std::cout<<out<<fileName<<std::endl;
        // for(auto it:children){
        //     it->display(depth+2);
        // }


        std::string indentation(depth, ' ');
        std::cout << indentation << "|-- " << fileName << "/" << std::endl;
        for (auto it : children) {
            it->display(depth + 3); // Adjust the depth for sub-items
        }
    }
};

class FileTree{
private:

    void getDirList(const std::string& directoryPath,FileNode*& curFolder);
public:
    void tree(std::string folderPath="."){
        FileNode* root=new Folder(folderPath);
    
        getDirList(folderPath,root);
        root->display(0);
    }
};



void FileTree::getDirList(const std::string& directoryPath,FileNode*& curFolder){
    DIR* dir = opendir(directoryPath.c_str());
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_REG) {
                //文件
                // std::cout << "文件名: " << entry->d_name << std::endl;
                curFolder->add(new File(std::string(entry->d_name)));
            }else{
                //文件夹
                auto folder_name=std::string(entry->d_name);
                if(folder_name==".."||folder_name=="."){
                    continue;
                }
                // std::cout << "文件夹名: " << folder_name<< std::endl;
                FileNode* thisFolder=new struct Folder(folder_name);
                curFolder->add(thisFolder);
                getDirList(directoryPath+'\\'+folder_name,thisFolder);
            }
        }
        closedir(dir);
    } else {
        std::cerr << "无法打开目录" << std::endl;
    }
}

int main(int argc,char**argv){
    FileTree* fileTree=new FileTree();
    if(argc<2){
        fileTree->tree("..");
    }else{
        fileTree->tree(std::string(argv[1]));
    }
    
    
}