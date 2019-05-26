LIB = lib/libtristripper.a
TEST = test/test

CC = g++

CFLAGS = -Wall -O2 -DNDEBUG -DUNIX 

LFLAGS = -lGL -lGLU -lglut

INCLUDES = -Iinclude/


LIB_SRCS =							\
	src/connectivity_graph.cpp		\
	src/policy.cpp					\
	src/tri_stripper.cpp			\

TEST_SRCS =							\
	test/gl_mesh.cpp				\
	test/gl_model.cpp				\
	test/gl_renderer.cpp			\
	test/high_res_timer.cpp			\
	test/main.cpp					\
	test/varrays_normalizer.cpp		\

LIB_OBJS = $(LIB_SRCS:.cpp=.o)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

all: $(TEST)

$(LIB): $(LIB_OBJS)
	@echo "Creating $(LIB)"
	@ar rc $(LIB) $(LIB_OBJS)

$(TEST) : $(LIB) $(TEST_OBJS)
	@echo "Linking $(TEST)"
	@$(CC) -o $(TEST) $(LFLAGS) $(TEST_OBJS) $(LIB) 

.cpp.o:
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(<:.cpp=.o) -c $<

clean_lib:
	@echo -n "Cleaning $(LIB)... "
	@rm -f $(LIB) $(LIB_OBJS)
	@echo "Done."

clean_test:
	@echo -n "Cleaning $(TEST)... "
	@rm -f $(TEST) $(TEST_OBJS)
	@echo "Done."

clean: clean_lib clean_test

rebuild: clean all

