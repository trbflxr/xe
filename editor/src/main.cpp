//
// Created by FLXR on 8/31/2019.
//

#include <cstdio>
#include <xe/scoped_array.hpp>
#include <xe/vector.hpp>
#include <xe/common.hpp>
#include <xe/string.hpp>
#include <unordered_map>
#include <xe/utils/logger.hpp>

int main() {
  xe::init();

  printf("hello\n");

  xe::scoped_array<xe::string> a;
  a.alloc(10);
  a.memSet("pidorass");

  a[0] = "1";
  a[1] = "2";

  printf("size: %zu\n", a.size());
  printf("size in bytes: %zu\n", a.sizeInBytes());

  for (const auto &i : a) {
    printf("%s, ", i.c_str());
  }

  xe::string str = "peedorass";
  const char *pos = xe::string::findChar(str.c_str(), 'e');
  printf("\n\npos:%s\n", pos);

  std::unordered_map<xe::string, xe::string> map;
  map["0"] = "peedorass0";
  map["1"] = "peedorass1";
  map["2"] = "peedorass2";

  for (const auto &v : map) {
    printf("first: %s, second: %s\n", v.first.c_str(), v.second.c_str());
  }

  xe::vector<uint> uvec;
  uvec.emplace_back(0);
  uvec.emplace_back(1);
  uvec.emplace_back(2);

  for (const auto &i : uvec) {
    printf("%u\n", i);
  }

  xe::string str0 = "жопа";

  XE_CORE_TRACE("test: {}, {}/{}", "peedorass", 14, 88);
  XE_INFO("Welcome to spdlog!");
  XE_ERROR("Some error message with arg: {}", 1);

  XE_WARN("Easy padding in numbers like {:08d}", 12);
  XE_CRITICAL("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
  XE_INFO("Support for floats {:03.2f}", 1.23456);
  XE_INFO("Positional args are {1} {0}..", "too", "supported");
  XE_INFO("{:<30}", "left aligned");
  XE_INFO("{:<30}", "залупа");
  XE_INFO("{}", str0);

}
