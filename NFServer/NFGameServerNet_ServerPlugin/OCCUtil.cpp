#include "OCCUtil.h"
#include <io.h>
#include <iostream> 
#if false // If the version of C++ is less than 17
#include <experimental/filesystem>
// It was still in the experimental:: namespace
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

void getFiles(std::string path, std::vector<std::string>& files)
{

#if 0 // _findfirst not working properly in Centos

    // 文件句柄
    long hFile = 0;
    // 文件信息
    struct _finddata_t fileinfo;

    std::string p;

    std::cout << "start to loop" << std::endl;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
        do {
            // 保存文件的全路径
            std::cout << fileinfo.name << std::endl;
            if (fileinfo.name[0] == '.')
            {
                continue;
            }
            // files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            //  files.push_back(fileinfo.name);
        } while (_findnext(hFile, &fileinfo) == 0); //寻找下一个，成功返回0，否则-1

        _findclose(hFile);
    }
    std::cout << "end of model file names" << std::endl;
#endif // 0
}

int loadModelFileNames(std::vector<std::string>& aModels)
{
    aModels.clear();
    std::string aModelPath = "../Models";
    std::cout << "start to load model file names" << std::endl;
    fs::directory_iterator files(aModelPath);
    for (auto& itoa : files)
    {
        if (itoa.status().type() != fs::file_type::directory)
            continue;
        std::cout << itoa.path().filename().string() << " ";
        aModels.push_back(itoa.path().filename().string());
    }
    std::cout << std::endl;
    // aModels.push_back("cube.stl");
    // aModels.push_back("test.step");
    // aModels.push_back("valve203.step");
    // aModels.push_back("airforce1.stl");
    // aModels.push_back("Assembly.stp");
    // aModels.push_back("goldwind.stp");
    // TODO 这个函数不行
    // getFiles(aModelPath, aModels);
    return 0;
}

bool endswith(const std::string& str, const std::string& end)
{
    int srclen = str.size();
    int endlen = end.size();
    if (srclen >= endlen)
    {
        std::string temp = str.substr(srclen - endlen, endlen);
        if (temp == end)
            return true;
    }

    return false;
}

long long GetSystemTime()
{
    timeb t;
    ftime(&t);
    return t.time * 1000 + t.millitm;
}
