gcc -xc++ \
 code/test/Main.cpp code/test/TestActivity.cpp code/test/DrawData.cpp \
 code/src/*.cpp code/src/render/*.cpp code/src/shader/*.cpp code/src/object/*.cpp \
 3rd/src/glad.c \
 -I3rd/include \
 -Icode/include \
 -Icode/include/render \
 -L3rd/lib \
 -rpath 3rd/lib \
 -lassimp.5 \
 -lglfw.3 \
 -lstdc++ \
 -std=c++11 \
 -o bin/Render


# TODO: 优化, 运行时可能找不到 assimp.5 和 glfw.3 等额外安装的动态库, 有以下几种解决方案:
# - 动态库拷贝到 /usr/local/lib 中 (目前使用的方式, 待优化)
# - 考虑使用静态链接
# - 动态库随包发布, 运行前执行 export DYLD_LIBRARY_PATH=`pwd`/3rd/lib:$DYLD_LIBRARY_PATH
