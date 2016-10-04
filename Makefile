# MacWarmer

OBJS_DIR = .objs

# define all the student executables
EXES = warmer

# list object file dependencies for each
OBJS=$(EXES).o smc.o pid.o monitor.o calculatepi.o

# set up compiler
CC = cc
WARNINGS = -Wall -Wextra -Werror -Wno-error=unused-parameter
CFLAGS_DEBUG   = $(INCLUDES) -O0 $(WARNINGS) -g -std=c99 -c -MMD -MP -D_GNU_SOURCE -DDEBUG
CFLAGS_RELEASE = $(INCLUDES) -O2 $(WARNINGS) -g -std=c99 -c -MMD -MP -D_GNU_SOURCE

# set up linker
LD = cc
LDFLAGS = -pthread -fPIC -framework IOKit

.PHONY: all
all: clean release

# build types
# run clean before building debug so that all of the release executables
# disappear
.PHONY: debug
.PHONY: release

release: $(EXES)

# include dependencies
-include $(OBJS_DIR)/*.d

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

# patterns to create objects
# keep the debug and release postfix for object files so that we can always
# separate them correctly

$(OBJS_DIR)/%-release.o: %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS_RELEASE) $< -o $@

# exes
# you will need a triple of exe and exe-debug and exe-tsan for each exe (other
# than provided exes)

$(EXES): $(OBJS:%.o=$(OBJS_DIR)/%-release.o)
	$(LD) $^ $(LDFLAGS) -o $@


.PHONY: clean
clean:
	-rm -rf .objs $(EXES)
