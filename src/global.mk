.PHONY: all debug profile clean
.DEFAULT_GOAL = debug

OBJECTDIR = ./objects
OBJECTDIR_PROFILE = ./$(OBJECTDIR)/profile
OBJECTDIR_DEBUG = ./$(OBJECTDIR)/debug

TARGET_EXT =

CC = gcc-13
CFLAGS =\
	-c\
	-std=c2x\
	-Wall\
	-Wextra\
	-Wpedantic\
	-fsanitize=undefined\
	-fsanitize-undefined-trap-on-error
CFLAGS_PROFILE =\
	-DNDEBUG\
	-pg\
	-O1
CFLAGS_DEBUG =\
	-g\
	-O0

LD = gcc
LDFLAGS =\
	-fsanitize=undefined\
	-fsanitize-undefined-trap-on-error\
	-lm
LDFLAGS_PROFILE =\
	-pg
LDFLAGS_DEBUG =\
	-Wl,-Map -Wl,debug/$(TARGET_NAME).map

OBJECTS_PROFILE = $(addprefix $(OBJECTDIR_PROFILE)/, $(SOURCES:.c=.o) )
OBJECTS_DEBUG = $(addprefix $(OBJECTDIR_DEBUG)/, $(SOURCES:.c=.o) )
DEPENDS_PROFILE = $(addprefix $(OBJECTDIR_PROFILE)/, $(SOURCES:.c=.d) )
DEPENDS_DEBUG = $(addprefix $(OBJECTDIR_DEBUG)/, $(SOURCES:.c=.d) )

-include $(DEPENDS_PROFILE)
-include $(DEPENDS_DEBUG)

$(OBJECTDIR_PROFILE)/%.o: %.c
	-mkdir -p $(@D)
	$(CC) $(INCLUDES) $(CFLAGS) $(CFLAGS_PROFILE) $(DEFINES) -MMD -MP -o $@ -l $(@D) $<

$(OBJECTDIR_DEBUG)/%.o: %.c
	-mkdir -p $(@D)
	$(CC) $(INCLUDES) $(CFLAGS) $(CFLAGS_DEBUG) $(DEFINES) -MMD -MP -o $@ -l $(@D) $<

debug/$(TARGET_NAME)$(TARGET_EXT): $(OBJECTS_DEBUG)
	mkdir -p debug
	$(LD) $(OBJECTS_DEBUG) $(LDFLAGS) $(LDFLAGS_DEBUG) -o $@

profile/$(TARGET_NAME)$(TARGET_EXT): $(OBJECTS_PROFILE)
	mkdir -p profile
	$(LD) $(OBJECTS_PROFILE) $(LDFLAGS) $(LDFLAGS_PROFILE) -o $@

debug: debug/$(TARGET_NAME)$(TARGET_EXT)
profile: profile/$(TARGET_NAME)$(TARGET_EXT)
all: clean debug profile

clean:
	rm -rfd $(OBJECTDIR)
	rm -rfd debug
	rm -rfd profile
