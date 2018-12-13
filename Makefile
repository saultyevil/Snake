# Helpful macros
TARGET_EXEC ?= snake
OBJ_DIR ?= ./objs
SRC_DIR ?= ./src
BIN_DIR ?= ./bin

# Macros for CC and FCC
CC = gcc
FC = gfortran
CFLAGS = -pedantic -Wall -O2
CLIBS = -lm -lgsl -lgslcblas -DDEBUG
FFLAGS = -Wall -O2
FLIBS = -lgsl -lgslcblas

# Useful macros
MKDIR_P ?= mkdir -p

# Create file paths for the source and object files
SRCS := $(shell find $(SRC_DIR) -name *.c -or -name *.f)
OBJS := $(SRCS:%=$(OBJ_DIR)/%.o)

# Compile the source and move to the bin directory
$(TARGET_EXEC): $(OBJS) 
	$(FC) $(OBJS) $(FFLAGS) $(FLIBS) -o $@
	$(MKDIR_P) $(BIN_DIR)
	cp $@ $(OBJ_DIR)/$@
	mv $@ $(BIN_DIR)/$@

# Create object files: note that the C and F object files are created separately
$(OBJ_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) $(CLIBS) -c $< -o $@

$(OBJ_DIR)/%.f.o: %.f
	$(MKDIR_P) $(dir $@)
	$(FC) $(FFLAGS) $(FLIBS) -c $< -o $@

all: clean $(TARGET_EXEC)

# Clean up commands
clean:
	$(RM) -r $(OBJ_DIR)

clean-all:
	$(RM) -r $(OBJ_DIR)
	$(RM) $(BIN_DIR)/$(TARGET_EXEC)

.PHONY: clean clean-all
