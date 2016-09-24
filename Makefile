CPPFLAGS += -Iinclude
CXXFLAGS += -std=c++14 -c -g -MD -Wall -Werror -Wextra
LDFLAGS += -lrt

SRC_DIR := src
TST_DIR := tst
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
DEPENDS = $(OBJECTS:.o=.d)
EXECUTABLE = image

all: $(EXECUTABLE)

justme: $(EXECUTABLE)

.PHONY: test
test:
	$(MAKE) -C $(TST_DIR)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

.cpp.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

-include $(DEPENDS)

clean:
	$(MAKE) clean -C $(TST_DIR)
	rm -f $(SRC_DIR)/*.[do] $(SRC_DIR)/*.rpo $(EXECUTABLE)
