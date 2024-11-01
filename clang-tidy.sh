#!/bin/sh

echo clang-tidy "$@" -load ~/clang-plugin/libEpitaClangTidyPlugin.so -- -I"."
clang-tidy "$@" -load ~/clang-plugin/build/libEpitaClangTidyPlugin.so -- -I"."
