gcc -xc++ \
 code/src/Main.cpp code/src/render/*.cpp code/src/Camera.cpp code/src/Image.cpp code/src/MathUtils.cpp code/src/model/*.cpp \
 3rd/src/glad.c \
 -I3rd/include \
 -Icode/include \
 -Icode/include/render \
 -L3rd/lib \
 -lglfw.3 \
 -lstdc++ \
 -ldl -lm -lX11 -lpthread \
 -std=c++11 \
 -o bin/Render
