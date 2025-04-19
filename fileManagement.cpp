#include <bits/stdc++.h>
using namespace std;
/*
    File
    Properties:
    - fileName
    - body
    Methods:
    + write()
    + append()
    + showFileInformation()
*/
class File {
    string fileName;
    string body;
    public:
    File(string name): fileName(name), body(""){}
    
    void write(string content) {
        body = content;
    }
    void append(string content) {
        body += content;
    }
    void showFileInformation() {
        cout<<"File name: "<<fileName<<endl;
        cout<<"File content: \n";
        cout<<body<<endl;
    }
};
/*
    Folder:
    Properties:
    - folderName
    - unordered_map<name,Folder*>folders
    - unordered_map<name,File*>files
    Properties:
    + createFolder()
    + deleteFolder()
    + createFile()
    + deleteFile()
    + getInto() -> Gets inside of a folder under it
    + getFile()
    + folderContent()
*/
class Folder {
    string folderName;
    unordered_map<string, Folder*>folders;
    unordered_map<string, File*>files;
    public:
    Folder(string name) {
        folderName = name;
    }
    Folder* getInto(string name ) {
        if(folders.find(name)!= folders.end()) {
            return folders[name];
        }
        cout<<"No such folder"<<endl;
        return NULL;
    }
    bool createFolder(string name) {
        if(folders.find(name)!=folders.end()) {
            cout<<"Folder with same name exists, try different name"<<endl;
            return false;
        }
        cout<<"Folder "<<name<<" created"<<endl;
        folders[name] = new Folder(name);
        return true;
    }
    bool deleteFolder(string name) {
        if(folders.find(name)==folders.end()) {
            cout<<"Folder does not exists"<<endl;
            return false;
        }
        cout<<"Folder "<<name<<" deleted"<<endl;
        Folder *folder = folders[name];
        // Can consider deleting all folders, files inside folder
        delete folder;
        folders.erase(name);
        return true;
    }
    bool createFile(string name) {
        if(files.find(name)!=files.end()) {
            cout<<"File with same name exists, try different name"<<endl;
            return false;
        }
        cout<<"File "<<name<<" created"<<endl;
        files[name] = new File(name);
        return true;
    }
    bool deleteFile(string name) {
        if(files.find(name)==files.end()) {
            cout<<"File does not exists"<<endl;
            return false;
        }
        cout<<"File "<<name<<" deleted"<<endl;
        File *file = files[name];
        delete file;
        files.erase(name);
    }
    File* getFile(string name) {
        if(files.find(name)!=files.end()) {
            return files[name];
        }
        cout<<"No such file exists"<<endl;
        return NULL;
    }
    void folderContent() {
        cout<<"Content of "<<folderName<<": \n";
        if(folders.size() == 0 && files.size() == 0) {
            cout<<"Empty folder"<<endl;
            return;
        }
        for(auto x:folders) {
            (x.second)->folderContent();
        }
        for(auto x:files) {
            cout<<x.first<<endl;
        }
    }
};
/*
    FileManager
    Properties:
    - static mutex
    - static obj -> Singleton
    - Private constructor
    - Disabled copy constructor and operator overloading
    - splitPath() -> Split incoming path into folder
    Methods:
    + createFolder()
    + createFile()
    + folderContent()
    + writeInFile()
    + showFileContent()
    + deleteFolder()
*/
class FileManager {
    Folder *folder;
    static mutex mtx;
    static FileManager* obj;
    FileManager() {
        folder = new Folder("home");
    }
    /*
        splitPath() - Split the incoming path
        "home/hello/hello2/" -> {"home","hello","hello2"};
        "home/hello"         -> {"home","hello"}
    */
    vector<string> splitPath(string path) {
        int idx = 0;
        vector<string>ans;
        int x = path.find("/",idx);
        while(x != string::npos) {
            string s = path.substr(idx,x-idx);
            ans.push_back(s);
            idx = x+1;
            x = path.find("/",idx);
        }
        string s = path.substr(idx);
        if(s.length() > 0)
            ans.push_back(s);
        return ans;
    }
    FileManager(const FileManager&) = delete;
    void operator=(const FileManager&) = delete;
    public:
    static FileManager* getInstance() {
        lock_guard<mutex>lock(mtx);
        if(obj == NULL) {
            obj = new FileManager();
        }
        return obj;
    }
    // Starts from index 1, since index 0 = home always
    // f=folder (which is "home")
    void createFolder(string path, string name) {
        vector<string>split = splitPath(path);
        Folder *f=folder;
        for(int i=1; i<split.size(); i++) {
            string fold = split[i];
            // cout<<fold<<endl;
            f = f->getInto(fold);
            if(!f) {
                return;
            }
        }
        f->createFolder(name);
    }
    void folderContent(string path) {
        vector<string>split = splitPath(path);
        Folder *f=folder;
        for(int i=1; i<split.size(); i++) {
            string fold = split[i];
            // cout<<fold<<endl;
            f = f->getInto(fold);
            if(!f) {
                return;
            }
        }
        f->folderContent();
    }
    void createFile(string path, string name) {
        vector<string>split = splitPath(path);
        Folder *f=folder;
        for(int i=1; i<split.size(); i++) {
            string fold = split[i];
            // cout<<fold<<endl;
            f = f->getInto(fold);
            if(!f) {
                return;
            }
        }
        f->createFile(name);
    }
    void writeInFile(string path, string content) {
        vector<string>split = splitPath(path);
        Folder *f=folder;
        int n = split.size();
        for(int i=1; i<n-1; i++) {
            string fold = split[i];
            // cout<<fold<<endl;
            f = f->getInto(fold);
            if(!f) {
                return;
            }
        }
        string fileName = split[n-1];
        f->getFile(fileName)->write(content);
    }
    void showFileContent(string path) {
        vector<string>split = splitPath(path);
        Folder *f=folder;
        int n = split.size();
        for(int i=1; i<n-1; i++) {
            string fold = split[i];
            // cout<<fold<<endl;
            f = f->getInto(fold);
            if(!f) {
                return;
            }
        }
        string fileName = split[n-1];
        f->getFile(fileName)->showFileInformation();
    }
    // Append to file method
    void deleteFolder(string path) {
        vector<string>split = splitPath(path);
        Folder *f=folder;
        int n = split.size();
        for(int i=1; i<n-1; i++) {
            string fold = split[i];
            // cout<<fold<<endl;
            f = f->getInto(fold);
            if(!f) {
                return;
            }
        }
        string folderName = split[n-1];
        f->deleteFolder(folderName);
    }
};
mutex FileManager::mtx;
FileManager* FileManager::obj = NULL;
int main() {
    FileManager* a = FileManager::getInstance();
    // Create folders
    a->createFolder("home", "hello");
    a->createFolder("home/hello/", "hello2");
    
    // Create files
    a->createFile("home/hello", "file1");
    a->createFile("home/hello/hello2", "file2");
    
    // Folder content show
    a->folderContent("home");
    
    // Write in files
    a->writeInFile("home/hello/file1","hey this is file1");
    a->writeInFile("home/hello/hello2/file2", "hey this is file2");
    
    // Display file content
    a->showFileContent("home/hello/file1");
    a->showFileContent("home/hello/hello2/file2");
    
    // Delete hello2
    a->deleteFolder("home/hello/hello2");
    
    // Access file content inside hello2
    a->showFileContent("home/hello/hello2/file2");
}