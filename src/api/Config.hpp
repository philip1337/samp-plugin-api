#ifndef SRC_API_CONFIG_HPP
#define SRC_API_CONFIG_HPP

#pragma once

#include <Config.hpp>
#include <string>

#include <map>
#include <vector>

SAMP_API_BEGIN_NS

class Config
{
public:
	bool Load(std::string path);
	bool Get(const std::string& index, std::string &dest, uint32_t pos = 0);
	void Get(const std::string& index, std::string &dest, std::string def, uint32_t pos = 0);
	bool Get(const std::string& index, std::vector<std::string>& args);
private:
	std::map<std::string, std::vector<std::string>> entries_;
};

extern Config* sampConfig;

SAMP_API_END_NS
#endif
