CC := gcc
LDFLAGS := -ldl -lGLEW -lglfw -lGL -lEGL -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon -lm -lpthread -lassimp -lstdc++
CFLAGS := -Wall -g

SRC_DIR := src
BUILD_DIR := build
IMGUI_LIB_DIR := dependencies/imgui/lib

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:%.c=%.o)
EXECUTABLE = out

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	g++ -o $@ $(addprefix $(BUILD_DIR)/,$(notdir $^)) $(IMGUI_LIB_DIR)/cimgui.a $(IMGUI_LIB_DIR)/imgui_impl_glfw.o $(IMGUI_LIB_DIR)/imgui_impl_opengl3.o $(LDFLAGS)

$(OBJS): %.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_DIR)/,$(notdir $@)) -Idependencies/imgui/include

run:
	./out

debug:
	gdb ./out

leak-check:
	#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=leak.txt ./build/app
	valgrind --leak-check=full --log-file=leak.txt ./out

clean:
	rm -rf $(BUILD_DIR)/*
