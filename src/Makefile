PROJECT_NAME	=Eye_of_Verter
ODIR 			= obj
LIB_INC_DIR		= ./include
LIB_SRC_DIR		= ./src
LIB_O_DIR		= $(ODIR)/$(LIB_SRC_DIR)

TESTS_DIR		= tests
BUILD_DIR		= ../src/lib
UNAME_S 		:= $(shell uname -s)
TEST_SRC		= $(TESTS_DIR)/${PROJECT_NAME}_test.cc
Flags 			= -std=c++17 -lstdc++ -Werror -Wall -Wextra $(DEBUG)
gcc 			= gcc $(Flags)
LIB_SOURCES		= $(shell find $(LIB_SRC_DIR) -name '*.cc')
REPORT_FILES	= $(shell find . -name '*.gcno') $(shell find . -name '*.gcda')
INC				:= $(shell find $(LIB_INC_DIR) -name '*.h')
LIB_OBJ 		= $(patsubst %.cc,$(ODIR)/%.o, $(LIB_SOURCES))
SHARED_LIB_INCLUDE = ./include
SHARED_LIB_BIN = /Users/evverenn/Desktop/Projects/Junk/Eye_of_Verter/src/lib
ifeq ($(UNAME_S),Linux)
	gcc += -D OS_LINUX
	TEST_CHECK_F= -lcheck
	OPEN_CMD = xdg-open
	ADD_LIB = -lm -lrt -lpthread
endif

ifeq ($(UNAME_S),Darwin)
	gcc += -D OS_MAC
	TEST_CHECK_F = -lgtest -lgmock -pthread
	OPEN_CMD = open
	ADD_LIB =
endif

all: clear $(BUILD_DIR)/$(PROJECT_NAME).a

$(LIB_O_DIR)/%.o: $(LIB_SRC_DIR)/%.cc
	$(gcc) -lcheck $(ADD_LIB) -I${LIB_INC_DIR} -I${SHARED_LIB_INCLUDE}  $< -c -o $@ -L./lib
clear:
	clear

clean:
	@rm -rf $(BUILD_DIR)/lib$(PROJECT_NAME).a test *.out
	@rm -rf $(ODIR)/*.o $(ODIR)/*.gc*
	@rm -rf $(ODIR)/*/*.o $(ODIR)/*/*.gc*
	@rm -rf $(ODIR)/*/*/*.o $(ODIR)/*/*/*.gc*
	@rm -rf ./gcov/*.o ./gcov/*.gc*
	@rm -rf ./*.gc*
	@rm -rf ./*.dSYM
	@rm -rf report*
	@rm -rf main.app

obj: $(LIB_OBJ)

no_warns:
	$(eval gcc = gcc)

clang:
	clang-format --style=Google -n $(shell find . -name '*.cc') $(shell find . -name '*.h')

clang-replace:
	clang-format --style=Google -i $(shell find . -name '*.cc') $(shell find . -name '*.h')

$(BUILD_DIR)/$(PROJECT_NAME).a: obj
	ar rcs $(BUILD_DIR)/lib$(PROJECT_NAME).a $(LIB_O_DIR)/*.o
$(PROJECT_NAME).a: $(BUILD_DIR)/$(PROJECT_NAME).a
debug:
	$(eval DEBUG = -g)
	@echo $(PROJECT_NAME):Debug build

set_report_gcc:
	$(eval gcc += -fprofile-arcs -ftest-coverage -fPIC)
	@echo $(PROJECT_NAME):Check build

test: $(TEST_SRC) $(BUILD_DIR)/$(PROJECT_NAME).a
	$(gcc) -I${LIB_INC_DIR} $^ $(TEST_CHECK_F) -o $@
	./test
gcov_report: clean set_report_gcc test
	./test
	@gcov *.c ${LIB_SRC_DIR}/*.c -o $(ODIR)/$(LIB_SRC_DIR)
	@mv $(ODIR)/$(LIB_SRC_DIR)/*.gc*  ./gcov
	@mv *.gc* ./gcov
	@mv ./gcov/*test* ./
	@lcov -t ./gcov -o report.info -c -d ./gcov
	@genhtml -o report report.info
	$(OPEN_CMD) ./report/index.html
#-Wl,-rpath,${SHARED_LIB_BIN} 
main: $(PROJECT_NAME).a
	$(gcc) -I${LIB_INC_DIR} $@.cc -o $@ -L./lib -l$(PROJECT_NAME) -lssl -lcrypto -Wl,-rpath,${SHARED_LIB_BIN} -lgit2
test_main: $(PROJECT_NAME).a
	$(gcc) -I${LIB_INC_DIR} $@.cc -o $@ -L./lib -l$(PROJECT_NAME) -lssl -lcrypto -Wl,-rpath,${SHARED_LIB_BIN} -lgit2
.PHONY: clean obj clear $(PROJECT_NAME) test clang-replace all $(BUILD_DIR)/$(PROJECT_NAME).a set_debug_gcc set_report_gcc gcov_report