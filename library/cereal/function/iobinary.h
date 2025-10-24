#pragma once
#include<fstream>
#include<string>
#include<cassert>
#include"cereal/cereal.hpp"
#include"cereal/archives/binary.hpp"
#include"cereal/archives/xml.hpp"

template<typename T>
void SaveBinary(const T& arg_value, const std::string& arg_fileName) {
	std::ofstream ofs(arg_fileName, std::ios::binary);
	cereal::BinaryOutputArchive archive(ofs);
	archive(arg_value);
}

template<typename T>
T LoadBinary(const std::string& arg_fileName) {
	std::ifstream ifs(arg_fileName, std::ios::binary);
	cereal::BinaryInputArchive archive(ifs);
	T temporary;
	archive(temporary);
	return temporary;
}



