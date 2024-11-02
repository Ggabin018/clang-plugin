#!/bin/sh
usr=`whoami`
echo clang-tidy "$@" --config-file="/home/$usr/clang-plugin/.clang-tidy" -load ~/clang-plugin/libEpitaClangTidyPlugin.so -- -I"."
clang-tidy "$@" --config-file ~/clang-plugin/.clang-tidy  --load ~/clang-plugin/build/libEpitaClangTidyPlugin.so -- -I"."
