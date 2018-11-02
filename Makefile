# Helpful macros
TARGET_EXEC ?= snake
OBJ_DIR ?= ./src/objs
SRC_DIRS ?= ./src
BIN_DIR ?= ./bin

# Macros for CC
CC = mpicc
CFLAGS = -pedantic -Wall -O2 -DMPI_ON
LIBS = -lm -lgsl -lgslcblas

# Directories
SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(SRCS:%=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Include Macros
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Compile the source
$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LIBS) -o $@
	$(MKDIR_P) $(BIN_DIR)
	mv $@ $(BIN_DIR)/$@

# Create object files
$(OBJ_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

# clean stuff up
clean:
	$(RM) -r $(OBJ_DIR)

clean-all:
	$(RM) -r $(OBJ_DIR)
	$(RM) $(TARGET_EXEC)

MKDIR_P ?= mkdir -p
