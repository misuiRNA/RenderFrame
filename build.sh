gcc -xc++ \
 code/src/Main.cpp code/src/Shader.cpp code/src/StbImage.cpp code/src/model/Rectangle.cpp code/src/model/Cuboid.cpp\
 3rd/src/glad.c \
 -I3rd/include \
 -Icode/include \
 -L3rd/lib \
 -lglfw.3 \
 -lstdc++ \
 -std=c++11 \
 -o bin/Render