## =========================================================
##                   Dietrich Bollmann, Kamakura, 2015/01/01
## 
## Makefile for nslfem-spring1d
## 
## Copyright (c) 2015 Dietrich Bollmann
##    
## This software may be modified and distributed under the terms
## of the MIT license.  See the LICENSE file for details.
## ---------------------------------------------------------
## 
## Usage:
## 
##   make                 Build
##   make clean           Remove object files
##   make cleanall        Remove all generated files,
##                        the executable included
## 
## ---------------------------------------------------------
## 
## Notes:
## 
##   - BOOST_ROOT has to be set in the environment, for example:
##     export BOOST_ROOT=/usr/local
## 
## ---------------------------------------------------------

## =========================================================
## Settings
## ---------------------------------------------------------

CPP = g++

CFLAGS = \
  -O3 -Wall -Dunix \
  -std=c++11 \
  -Wno-c++11-extensions

LFLAGS = -Wall -I. -lm

rm = rm -f

OBJ_DIR = obj
BIN_DIR = bin

BOOST_INCLUDE_DIR = -isystem$(BOOST_ROOT)/include

## =========================================================
## bin/nslfem-spring1d
## ---------------------------------------------------------

BINARY  = nslfem-spring1d
TARGET  = main
SRC_DIR = src

SOURCES  := $(wildcard $(SRC_DIR)/*.cpp)
INCLUDES := $(wildcard $(SRC_DIR)/*.h)
OBJECTS  := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

$(BIN_DIR)/$(BINARY): $(OBJECTS)
	$(CPP) -o $@ $(LFLAGS) $(OBJECTS)

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CPP) $(CFLAGS) $(BOOST_INCLUDE_DIR) -c $< -o $@

## =========================================================
## Default target
## ---------------------------------------------------------

.PHONEY: all
all: $(TARGET)

## =========================================================
## Run examples
## ---------------------------------------------------------

.PHONEY: examples
examples: example-2-1 example-2-2 example-2-3 example-2-4

.PHONEY: example-2-1
example-2-1: $(BIN_DIR)/$(BINARY)
	@echo '============================================================'
	@echo 'Example 2.1'
	@echo '------------------------------------------------------------'
	@$(BIN_DIR)/$(BINARY) input-files/example-2-1.fem

.PHONEY: example-2-2
example-2-2: $(BIN_DIR)/$(BINARY)
	@echo '============================================================'
	@echo 'Example 2.2'
	@echo '------------------------------------------------------------'
	@$(BIN_DIR)/$(BINARY) input-files/example-2-2.fem

.PHONEY: example-2-3
example-2-3: $(BIN_DIR)/$(BINARY)
	@echo '============================================================'
	@echo 'Example 2.3'
	@echo '------------------------------------------------------------'
	@@$(BIN_DIR)/$(BINARY) input-files/example-2-3.fem

.PHONEY: example-2-4
example-2-4: $(BIN_DIR)/$(BINARY)
	@echo '============================================================'
	@echo 'Example 2.4'
	@echo '------------------------------------------------------------'
	@$(BIN_DIR)/$(BINARY) input-files/example-2-4.fem

## =========================================================
## bin/main-test
## ---------------------------------------------------------

TEST_TARGET  = main-test
TEST_SRC_DIR = test
TEST_LIBS    = -L$(BOOST_ROOT)/lib -lboost_unit_test_framework

TEST_SOURCES  := $(wildcard $(TEST_SRC_DIR)/*.cpp)
TEST_INCLUDES := $(wildcard $(SRC_DIR)/*.h) $(wildcard $(TEST_SRC_DIR)/*.h)
TEST_OBJECTS  := $(TEST_SOURCES:$(TEST_SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

$(BIN_DIR)/$(TEST_TARGET): $(OBJECTS) $(TEST_OBJECTS)
	$(CPP) -o $@ $(LFLAGS) $(filter-out $(OBJ_DIR)/$(TARGET).o, $(OBJECTS)) $(TEST_OBJECTS) $(TEST_LIBS)

$(TEST_OBJECTS): $(OBJ_DIR)/%.o : $(TEST_SRC_DIR)/%.cpp
	$(CPP) $(CFLAGS) -Isrc $(BOOST_INCLUDE_DIR) -c $< -o $@

.PHONEY: test
test: $(BIN_DIR)/$(TEST_TARGET)
	$(BIN_DIR)/$(TEST_TARGET)

## =========================================================
## Cleanup
## ---------------------------------------------------------

.PHONEY: clean
clean:
	$(rm) $(OBJECTS) $(TEST_OBJECTS)

.PHONEY: cleanall
cleanall: clean
	$(rm) $(BIN_DIR)/$(BINARY) $(BIN_DIR)/$(TEST_TARGET)

## =========================================================
## =========================================================

## fin.
