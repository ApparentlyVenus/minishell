# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: odana <odana@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/17 22:14:34 by yitani            #+#    #+#              #
#    Updated: 2025/07/25 07:05:09 by odana            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME      = minishell

CC        = cc

CFLAGS    = -Wall -Wextra -Werror

# Libraries
LIBFT_DIR = minishell_libft
LIBFT     = $(LIBFT_DIR)/libft.a

# Libft source files
LIBFT_SRCS = $(LIBFT_DIR)/ft_atoi.c \
             $(LIBFT_DIR)/ft_atoll.c \
             $(LIBFT_DIR)/ft_bzero.c \
             $(LIBFT_DIR)/ft_calloc.c \
             $(LIBFT_DIR)/ft_isalnum.c \
             $(LIBFT_DIR)/ft_isalpha.c \
             $(LIBFT_DIR)/ft_isascii.c \
             $(LIBFT_DIR)/ft_isdigit.c \
             $(LIBFT_DIR)/ft_isprint.c \
             $(LIBFT_DIR)/ft_itoa.c \
             $(LIBFT_DIR)/ft_lstadd_back.c \
             $(LIBFT_DIR)/ft_lstadd_front.c \
             $(LIBFT_DIR)/ft_lstclear.c \
             $(LIBFT_DIR)/ft_lstdelone.c \
             $(LIBFT_DIR)/ft_lstiter.c \
             $(LIBFT_DIR)/ft_lstlast.c \
             $(LIBFT_DIR)/ft_lstmap.c \
             $(LIBFT_DIR)/ft_lstnew.c \
             $(LIBFT_DIR)/ft_lstsize.c \
             $(LIBFT_DIR)/ft_memchr.c \
             $(LIBFT_DIR)/ft_memcmp.c \
             $(LIBFT_DIR)/ft_memcpy.c \
             $(LIBFT_DIR)/ft_memmove.c \
             $(LIBFT_DIR)/ft_memset.c \
             $(LIBFT_DIR)/ft_putchar_fd.c \
             $(LIBFT_DIR)/ft_putendl_fd.c \
             $(LIBFT_DIR)/ft_putnbr_fd.c \
             $(LIBFT_DIR)/ft_putstr_fd.c \
             $(LIBFT_DIR)/ft_split.c \
             $(LIBFT_DIR)/ft_strchr.c \
             $(LIBFT_DIR)/ft_strcmp.c \
             $(LIBFT_DIR)/ft_strdup.c \
             $(LIBFT_DIR)/ft_striteri.c \
             $(LIBFT_DIR)/ft_strjoin.c \
             $(LIBFT_DIR)/ft_strlcat.c \
             $(LIBFT_DIR)/ft_strlcpy.c \
             $(LIBFT_DIR)/ft_strlen.c \
             $(LIBFT_DIR)/ft_strmapi.c \
             $(LIBFT_DIR)/ft_strncmp.c \
             $(LIBFT_DIR)/ft_strnstr.c \
             $(LIBFT_DIR)/ft_strrchr.c \
             $(LIBFT_DIR)/ft_strtrim.c \
             $(LIBFT_DIR)/ft_substr.c \
             $(LIBFT_DIR)/ft_tolower.c \
             $(LIBFT_DIR)/ft_toupper.c

LIBFT_OBJS = $(LIBFT_SRCS:.c=.o)

# Include directories
INCLUDES  = -I inc -I $(LIBFT_DIR)

# External libraries
LIBS      = -lreadline -lhistory

# Source files organized by module
BUILTIN_SRCS = src/builtin_functions/general_helpers.c \
               src/builtin_functions/A_parsing_environment.c \
               src/builtin_functions/builtin_cd.c \
               src/builtin_functions/builtin_echo.c \
               src/builtin_functions/builtin_env.c \
               src/builtin_functions/builtin_exit.c \
               src/builtin_functions/builtin_export.c \
               src/builtin_functions/builtin_pwd.c \
               src/builtin_functions/builtin_unset.c \
               src/builtin_functions/export_helpers.c

TOKENIZATION_SRCS = src/tokenization/validation.c \
                    src/tokenization/validation_utils.c \
                    src/tokenization/tokenize_input.c \
                    src/tokenization/tokenization_helpers.c \
                    src/tokenization/tokenization_helpers_2.c \
                    src/tokenization/bool_helpers.c

NEW_EXPANSION_SRCS = src/new_expansion/expand_cmd.c \
                     src/new_expansion/expansion.c \
                     src/new_expansion/word_split_helpers.c \
                     src/new_expansion/collect_matches.c \
                     src/new_expansion/wildcard_expansion.c \
                     src/new_expansion/cmd_utils.c \
                     src/new_expansion/copy_split.c \
                     src/new_expansion/expand_exit_code.c \
                     src/new_expansion/expand_variables.c 
                     

PARSER_SRCS = src/parser/node_creation.c \
              src/parser/utils.c \
              src/parser/free.c \
              src/parser/parse.c \
              src/parser/helpers.c \
              src/parser/heredoc.c \
              src/parser/heredoc_helpers.c

EXECUTION_SRCS = src/execution/execution.c \
                 src/execution/free.c \
                 src/execution/redir.c \
                 src/execution/utils.c \
                 src/execution/utils2.c \
                 src/execution/pipes.c \
                 src/execution/israel.c

SHELL_SRCS = src/shell/init.c \
             src/shell/cleanup.c \
             src/shell/error_handle.c \
             src/shell/wrappers.c

MAIN_SRCS = src/main/art.c \
             src/main/extra.c \
            src/main/signals.c \
            src/main/main.c

# All source files
SRC = $(BUILTIN_SRCS) \
      $(TOKENIZATION_SRCS) \
      $(EXPANSION_SRCS) \
      $(NEW_EXPANSION_SRCS) \
      $(PARSER_SRCS) \
      $(EXECUTION_SRCS) \
      $(SHELL_SRCS) \
      $(MAIN_SRCS)

# Header files
HDR = inc/minishell.h \
      inc/tokenizer.h \
      inc/parser.h \
      inc/expansion.h \
      inc/new_expansion.h \
      inc/env.h \
      inc/execution.h \
      inc/shell.h

# Object files
OBJ = $(SRC:.c=.o)

all: $(NAME)

# Build libft first
$(LIBFT): $(LIBFT_OBJS)
	@echo "[📚] Creating libft archive..."
	@ar rcs $(LIBFT) $(LIBFT_OBJS)
	@echo "[✅] Libft ready!"

$(LIBFT_DIR)/%.o: $(LIBFT_DIR)/%.c
	@echo "[🔧] Compiling libft: $<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(LIBFT) $(OBJ)
	@echo "┌────────────────────────────────┐"
	@echo "│   🚀  Building minishell...    │"
	@echo "└────────────────────────────────┘"
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJ) $(LIBFT) $(LIBS)
	@echo "✅  Build complete! Enjoy your shell."

%.o: %.c $(HDR)
	@echo "[🛠️ ] Compiling $<…"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "[✔️ ] Created $@"

clean:
	@echo "[🧹] Cleaning object files…"
	@rm -f $(OBJ) $(LIBFT_OBJS)
	@echo "[✔️ ] Objects removed."

fclean: clean
	@echo "[💣] Nuking $(NAME) and libft"
	@rm -f $(NAME) $(LIBFT)
	@echo "[✔️ ] $(NAME) has been nuked."

re: fclean all

norminette:
	@echo "🔍 Running Norminette on all files…"
	@norminette $(SRC) $(HDR) $(LIBFT_DIR)

.PHONY: all clean fclean re norminette