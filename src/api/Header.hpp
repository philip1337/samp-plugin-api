#ifndef SRC_API_HEADER_HPP
#define SRC_API_HEADER_HPP

#pragma once

#include <Config.hpp>

#include <string>

SAMP_API_BEGIN_NS

struct Header {
  std::string name;
  std::string value;
};

SAMP_API_END_NS
#endif
