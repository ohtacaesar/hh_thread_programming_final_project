COMPILER    = gcc-4.8
CFLAGS      = -Wextra -g -MMD -MP
LDFLAGS     =
LIBS        =
INCLUDE     = -I./include
TARGET      = ./bin/final_project
TARGET_DIR  = ./bin
SRCDIR      = ./src
OBJDIR      = ./obj
ifeq "$(strip $(OBJDIR))" ""
  OBJDIR    = .
endif

TEST_SRCDIR = ./test
TEST_OBJDIR = ./test_obj
TEST_TARGET = ./bin/test

SOURCES     = $(wildcard $(SRCDIR)/*.c)
OBJECTS     = $(subst $(SRCDIR), $(OBJDIR), $(SOURCES:.c=.o))
	DEPENDS = $(OBJECTS:.o=.d)

TEST_SOURCES = $(wildcard $(TEST_SRCDIR)/*.c)
TEST_OBJECTS = $(subst $(TEST_SRCDIR), $(TEST_OBJDIR), $(TEST_SOURCES:.c=.o))
	TEST_DEPENDS = $(TEST_OBJECTS:.o=.d)

default: $(TARGET) $(TEST_TARGET)

$(TARGET): $(OBJECTS) $(LIBS) 
	@[ -d $(TARGET_DIR) ] || mkdir -p $(TARGET_DIR)
	$(COMPILER) -o $@ $^ $(LDFLAGS)

$(TEST_TARGET): $(TEST_OBJECTS) $(filter-out $(OBJDIR)/main.o, $(OBJECTS)) $(LIBS) 
	@[ -d $(TARGET_DIR) ] || mkdir -p $(TARGET_DIR)
	$(COMPILER) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@[ -d $(OBJDIR) ] || mkdir -p $(OBJDIR)
	$(COMPILER) $(CFLAGS) $(INCLUDE) -o $@ -c $<


$(TEST_OBJDIR)/%.o: $(TEST_SRCDIR)/%.c
	@[ -d $(TEST_OBJDIR) ] || mkdir -p $(TEST_OBJDIR)
	$(COMPILER) $(CFLAGS) $(INCLUDE) -o $@ -c $<

-include $(DEPENDS)
