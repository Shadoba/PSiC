CXX = g++
LD = g++ 
DEP_FLAGS = -MMD
CXX_FLAGS = -g -Wall
LFLAGS = -Wall -lm $(HOME)/ZAJECIA/PSiC/PSiC/zeroInstall/lib/libzmq.a -pthread
INC_FLAG = -Iinc -I$(HOME)/ZAJECIA/PSiC/PSiC/zeroInstall/include

ifndef $(LOG_LEVEL)
LOG_LEVEL = 0
endif
CXXFLAGS += $(DEP_FLAGS) $(INC_FLAG) -DLOG_LEVEL=$(LOG_LEVEL)
LFLAGS += $(DEP_FLAGS) $(INC_FLAG)

#App name
TARGET=ServerProxy

BINDIR = ./bin
BUILDDIR = ./build
SRCDIR = ./src

SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(addprefix $(BUILDDIR)/, $(notdir $(SRC:.cpp=.o)))
DEP = $(addprefix $(BUILDDIR)/, $(notdir $(SRC:.cpp=.d)))

all: $(BINDIR)/$(TARGET)
	@echo "LOG_LEVEL set to "$(LOG_LEVEL)"!"
#$(TARGET): $(OBJ)
#	$(CXX) $(CXXFLAGS) $(OBJ) -o build/$@

$(BINDIR)/$(TARGET): $(OBJ)
	@$(LD) $(OBJ) $(LFLAGS) -o $@
	@echo "Linking complete!"

$(OBJ): $(BUILDDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

clean:
	@rm -f $(BINDIR)/$(TARGET) $(OBJ) $(DEP)

-include $(DEP)

.PHONY: clean run mem gdb

mem: 
	valgrind --leak-check=full $(BINDIR)/$(TARGET)
gdb:
	gdb $(BINDIR)/$(TARGET)
run:
	$(BINDIR)/$(TARGET)
