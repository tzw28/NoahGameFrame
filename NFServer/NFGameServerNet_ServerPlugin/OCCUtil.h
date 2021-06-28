#pragma once
#include <string>
#include <vector>

void getFiles(std::string path, std::vector<std::string>& files);

bool endswith(const std::string& str, const std::string& end);


int loadModelFileNames(std::vector<std::string>& aModels);
