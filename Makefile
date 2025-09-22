NAME = minishell

# Directories
SRC_DIR = src
LIBFT_DIR = libft
OBJ_DIR = obj
INCLUDE_DIR = includes

# Sources
SRC := \
	builtins/builtins.c \
	builtins/builtin_cd.c \
	builtins/env.c \
	builtins/update_env.c \
	builtins/export.c \
	builtins/unset.c \
	cmd_management/exec.c \
	cmd_management/fork.c \
	cmd_management/path.c \
	debug/debug.c \
	main/init.c \
	main/main.c \
	main/signals.c \
	main/token_management.c \
	parsing/expand.c \
	parsing/parse_pipe.c \
	parsing/parse.c \
	parsing/parsing_errors.c \
	parsing/parsing_quotes.c \
	parsing/redirect.c \
	redirection/heredoc.c \
	redirection/pipes.c \
	redirection/redirection.c \
	utils/free.c \
	utils/free2.c \
	utils/utils.c \
	utils/utils2.c \
	utils/utils3.c \
	utils/utils4.c \

#Path to sources
VPATH = $(SRC_DIR) 

# Objects
OBJ_FILES := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC)) \

# Compilation
CC := cc
CFLAGS := -Wall -Wextra -Werror -I$(INCLUDE_DIR) -I$(LIBFT_DIR)

#Libraries
LIBFT = $(LIBFT_DIR)/libft.a

#Objects directory
OBJDIRS := $(sort $(dir $(OBJ_FILES)))

# Rules
all: $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJ_FILES) $(LIBFT)
	$(CC) $(OBJ_FILES) -o $(NAME) -L$(LIBFT_DIR) -lft -lreadline

$(OBJ_DIR)/%.o: %.c | $(OBJDIRS)
	$(CC) $(CFLAGS) -c $< -o $@

#Create object directory if doesn't exist
$(OBJDIRS):
	@mkdir -p $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug : CFLAGS += -g3 -D PROMPT="\"\\001\033[1;36m\002debugshell \$ \001\033[1;34m\002\""
debug : re

.PHONY: all clean fclean re debug