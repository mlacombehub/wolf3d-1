# SOURCES

SRC_ENGINE := engine.c draw.c raycaster.c
SRC_GAME := hook.c player.c main.c
SRC_UTILS := ft_bzero.c ft_memcpy.c

# PATH

SRC_PATH := src
OBJ_PATH := obj
ENGINE_PATH := engine
GAME_PATH := game
UTILS_PATH := utils
HEADERS_PATHs := includes /usr/include/SDL2
FRAMEWORK_PATH := frameworks
FRAMEWORK := SDL2 SDL2_ttf SDL2_image

INCLUDE := $(HEADERS_PATHs:%=-I%) $(FRAMEWORK:%=-I$(FRAMEWORK_PATH)/%.framework/Headers/)

# LINK

SRC_ENGINE := $(addprefix $(ENGINE_PATH)/,$(SRC_ENGINE))
SRC_GAME := $(addprefix $(GAME_PATH)/,$(SRC_GAME))
SRC_UTILS := $(addprefix $(UTILS_PATH)/,$(SRC_UTILS))


# OBJECTS AND DEPENDENCY

OBJ_ENGINE := $(SRC_ENGINE:%.c=$(OBJ_PATH)/%.o)
OBJ_GAME := $(SRC_GAME:%.c=$(OBJ_PATH)/%.o)
OBJ_UTILS := $(SRC_UTILS:%.c=$(OBJ_PATH)/%.o)
DEP := $(OBJ_ENGINE:%.o=%.d)
DEP += $(OBJ_GAME:%.o=%.d)
DEP += $(OBJ_UTILS:%.o=%.d)

# GLOBAL

CC = gcc
CFLAGS = -g3 -Wall -Wextra -Werror
NAME = wolf3d

ifeq ($(shell uname 2>/dev/null),Darwin) # Mac OS X
	LIB := -rpath @loader_path/$(FRAMEWORK_PATH) $(addprefix -framework ,$(FRAMEWORK)) $(FRAMEWORK_PATH:%=-F %) -lm
endif
ifeq ($(shell uname 2>/dev/null),Linux)
	LIB := $(addprefix -l,$(FRAMEWORK)) -lm
endif

# RULES

.PHONY: all
all:
	@echo "\033[32mCompilation's flags: \033[36m$(CFLAGS)"
	@echo "\033[32mFrameworks | Libs : \033[36m$(FRAMEWORK)"
	@echo "\033[32mGenerating objects :\033[0m"
	@$(MAKE) -j $(NAME)


$(NAME): $(OBJ_UTILS) $(OBJ_GAME) $(OBJ_ENGINE)
	@echo "\033[34mGenerated : \033[5;36m$(NAME)\033[0m"
	@$(CC) $(CFLAGS) $(INCLUDE) $(LIB) $^ -o $@

-include $(DEP)
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDE) -MMD -c $< -o $@
	@echo "\033[1;31m  >>  \033[4;34m$@\033[0m"

.PHONY: clean
clean:
	@echo "\033[1;31mObject's removed\033[0m"
	@$(RM) -r $(OBJ_PATH)

.PHONY: fclean
fclean:
	@echo "\033[1;31m$(NAME) removed\033[0m"
	@$(MAKE) clean
	@$(RM) $(NAME)

.PHONY: re
re:
	@$(MAKE) fclean
	@$(MAKE) all
