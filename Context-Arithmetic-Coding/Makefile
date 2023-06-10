CXX 	?= g++
TARGET 	:= context-arithmetic-coding.exe
SRCDIR 	:= ./src
_BINDIR := ./bin
_OBJDIR := ./obj
CXXFLAGS := -std=c++17 -Wall -Wextra -Wshadow -Wconversion
RC_FLAGS := -Ofast
DC_FLAGS := -g -DDEBUG -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG_PEDANTIC_ASSERT

ifeq ($(DEBUG), 1)
	CXXFLAGS := $(CXXFLAGS) $(DC_FLAGS)
	BINDIR := $(_BINDIR)/debug
	OBJDIR := $(_OBJDIR)/debug
else
	CXXFLAGS := $(CXXFLAGS) $(RC_FLAGS)
	BINDIR := $(_BINDIR)/release
	OBJDIR := $(_OBJDIR)/release
endif

TARGET := $(BINDIR)/$(TARGET)
SRCS := $(notdir $(wildcard ${SRCDIR}/*.cpp))
OBJS := $(addprefix $(OBJDIR)/, $(patsubst %.cpp, %.o, $(SRCS)))
DEPS := $(addprefix $(OBJDIR)/, $(patsubst %.cpp, %.d, $(SRCS)))

$(shell [ -d $(BINDIR) ] || mkdir -p $(BINDIR))
$(shell [ -d $(OBJDIR) ] || mkdir -p $(OBJDIR))

.PHONY: all
all: build run

.PHONY: run
run: build
	./$(TARGET)

.PHONY: build
build: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

-include $(DEPS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -MP -MMD -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(_BINDIR) $(_OBJDIR) $(TARGET)
