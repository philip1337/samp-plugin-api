#include "Config.hpp"

#include <fstream>
#include <algorithm>
#include <vector>

#include <boost/algorithm/string.hpp>

SAMP_API_BEGIN_NS

Config* sampConfig = nullptr;

bool Config::Load(std::string path)
{
	std::ifstream config(path);
	if (!config) {
		return false;
	} else {
		std::string line;
		while (std::getline(config, line)) {

			std::vector<std::string> strs;
			boost::split(strs, line, boost::is_any_of("\t "));

			if (strs.size() > 1) {
				auto index = strs[0];
				strs.erase(strs.begin());
				entries_.emplace(index, strs);	// We just support 1 argument per value
			}
		}
	}
	return true;
}

bool Config::Get(const std::string& index, std::string &dest, uint32_t pos)
{
	auto it = entries_.find(index);
	if (it != entries_.end()) {
		if (pos >= it->second.size())
			return false;

		dest = it->second[pos];
		return true;
	}
	return false;
}

void Config::Get(const std::string& index, std::string &dest, std::string def, uint32_t pos)
{
	if (Get(index, dest, pos))
		return;
	dest = def;
}

SAMP_API_END_NS
