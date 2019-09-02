//
// Created by FLXR on 8/31/2019.
//

#include <cstdio>
#include <memory>
#include <xe/scoped_array.hpp>
#include <xe/common.hpp>

int main() {
  xe::init();

  printf("hello\n");

  xe::scoped_array<std::string> a;
  a.alloc(10);
  a.memSet("pidorass");

  a[0] = "1";
  a[1] = "2";

  printf("size: %llu\n", a.size());
  printf("size in bytes: %llu\n", a.sizeInBytes());

  for (const auto &i : a) {
    printf("%s, ", i.c_str());
  }

}
