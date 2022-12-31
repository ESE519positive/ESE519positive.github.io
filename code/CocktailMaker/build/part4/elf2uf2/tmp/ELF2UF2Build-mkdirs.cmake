# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/app/519/pico/pico-sdk/tools/elf2uf2"
  "D:/app/519/pico/CocktailMaker/build/elf2uf2"
  "D:/app/519/pico/CocktailMaker/build/part4/elf2uf2"
  "D:/app/519/pico/CocktailMaker/build/part4/elf2uf2/tmp"
  "D:/app/519/pico/CocktailMaker/build/part4/elf2uf2/src/ELF2UF2Build-stamp"
  "D:/app/519/pico/CocktailMaker/build/part4/elf2uf2/src"
  "D:/app/519/pico/CocktailMaker/build/part4/elf2uf2/src/ELF2UF2Build-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/app/519/pico/CocktailMaker/build/part4/elf2uf2/src/ELF2UF2Build-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/app/519/pico/CocktailMaker/build/part4/elf2uf2/src/ELF2UF2Build-stamp${cfgdir}") # cfgdir has leading slash
endif()
