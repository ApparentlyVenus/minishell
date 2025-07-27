# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: odana <odana@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/25 07:58:09 by odana             #+#    #+#              #
#    Updated: 2025/07/27 13:49:17 by odana            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME      = minishell
CC        = cc
CFLAGS    = -Wall -Wextra -Werror -g
RM        = rm -f

SRCDIR    = src
INCDIR    = inc
OBJDIR    = obj
LIBFT_DIR = libft

LIBFT     = $(LIBFT_DIR)/libft.a
INCLUDES  = -I$(INCDIR) -I$(LIBFT_DIR)
LIBS      = -lreadline -lhistory

LIBFT_SOURCES = ft_atoi.c ft_atoll.c ft_bzero.c ft_calloc.c ft_isalnum.c \
                ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c \
                ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c \
                ft_lstiter.c ft_lstlast.c ft_lstmap.c ft_lstnew.c ft_lstsize.c \
                ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c \
                ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c \
                ft_split.c ft_strchr.c ft_strcmp.c ft_strdup.c ft_striteri.c \
                ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c \
                ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c \
                ft_tolower.c ft_toupper.c

BUILTIN_SOURCES = general_helpers.c A_parsing_environment.c builtin_cd.c \
                  builtin_echo.c builtin_env.c builtin_exit.c builtin_export.c \
                  builtin_pwd.c builtin_unset.c export_helpers.c

TOKENIZATION_SOURCES = validation.c validation_utils.c tokenize_input.c \
                       tokenization_helpers.c tokenization_helpers_2.c bool_helpers.c

EXPANSION_SOURCES = expand_cmd.c expansion.c word_split_helpers.c collect_matches.c \
                    wildcard_expansion.c cmd_utils.c copy_split.c expand_exit_code.c \
                    expand_variables.c quote_removal.c

PARSER_SOURCES = node_creation.c utils.c free.c parse.c helpers.c \
                 heredoc.c heredoc_helpers.c

EXECUTION_SOURCES = execution.c free.c redir.c utils.c utils2.c \
                    pipes.c execution_helpers.c operators.c

SHELL_SOURCES = init.c cleanup.c error_handle.c wrappers.c

MAIN_SOURCES = art.c extra.c signals.c main.c

LIBFT_SRCS = $(addprefix $(LIBFT_DIR)/, $(LIBFT_SOURCES))
BUILTIN_SRCS = $(addprefix $(SRCDIR)/builtin_functions/, $(BUILTIN_SOURCES))
TOKENIZATION_SRCS = $(addprefix $(SRCDIR)/tokenization/, $(TOKENIZATION_SOURCES))
EXPANSION_SRCS = $(addprefix $(SRCDIR)/new_expansion/, $(EXPANSION_SOURCES))
PARSER_SRCS = $(addprefix $(SRCDIR)/parser/, $(PARSER_SOURCES))
EXECUTION_SRCS = $(addprefix $(SRCDIR)/execution/, $(EXECUTION_SOURCES))
SHELL_SRCS = $(addprefix $(SRCDIR)/shell/, $(SHELL_SOURCES))
MAIN_SRCS = $(addprefix $(SRCDIR)/main/, $(MAIN_SOURCES))

SOURCES = $(BUILTIN_SOURCES) $(TOKENIZATION_SOURCES) $(EXPANSION_SOURCES) \
          $(PARSER_SOURCES) $(EXECUTION_SOURCES) $(SHELL_SOURCES) $(MAIN_SOURCES)
SRCS = $(BUILTIN_SRCS) $(TOKENIZATION_SRCS) $(EXPANSION_SRCS) $(PARSER_SRCS) \
       $(EXECUTION_SRCS) $(SHELL_SRCS) $(MAIN_SRCS)

BUILTIN_OBJS = $(BUILTIN_SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TOKENIZATION_OBJS = $(TOKENIZATION_SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
EXPANSION_OBJS = $(EXPANSION_SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
PARSER_OBJS = $(PARSER_SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
EXECUTION_OBJS = $(EXECUTION_SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
SHELL_OBJS = $(SHELL_SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
MAIN_OBJS = $(MAIN_SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
LIBFT_OBJS = $(LIBFT_SRCS:$(LIBFT_DIR)/%.c=$(OBJDIR)/libft/%.o)

OBJS = $(BUILTIN_OBJS) $(TOKENIZATION_OBJS) $(EXPANSION_OBJS) $(PARSER_OBJS) \
       $(EXECUTION_OBJS) $(SHELL_OBJS) $(MAIN_OBJS)



HDR = $(INCDIR)/minishell.h $(INCDIR)/tokenizer.h $(INCDIR)/parser.h \
      $(INCDIR)/expansion.h $(INCDIR)/env.h $(INCDIR)/execution.h \
      $(INCDIR)/shell.h $(INCDIR)/structs.h

all: $(NAME)

# Build libft - UPDATED to use new object paths
$(LIBFT): $(LIBFT_OBJS)
	@echo "[📚] Creating libft archive..."
	@ar rcs $(LIBFT) $(LIBFT_OBJS)
	@echo "[✅] Libft ready!"

$(OBJDIR)/libft/%.o: $(LIBFT_DIR)/%.c | $(OBJDIR)/libft
	@echo "[🔧] Compiling libft: $(notdir $<)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/libft:
	@mkdir -p $(OBJDIR)/libft

$(NAME): $(LIBFT) $(OBJS)
	@echo "┌────────────────────────────────┐"
	@echo "│   🚀  Building minishell...    │"
	@echo "└────────────────────────────────┘"
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJS) $(LIBFT) $(LIBS)
	@echo "✅  Build complete! Enjoy your shell."

BUILTIN_STARTED = $(OBJDIR)/.builtin_started
TOKENIZATION_STARTED = $(OBJDIR)/.tokenization_started
EXPANSION_STARTED = $(OBJDIR)/.expansion_started
PARSER_STARTED = $(OBJDIR)/.parser_started
EXECUTION_STARTED = $(OBJDIR)/.execution_started
SHELL_STARTED = $(OBJDIR)/.shell_started
MAIN_STARTED = $(OBJDIR)/.main_started

$(OBJDIR)/builtin_functions/%.o: $(SRCDIR)/builtin_functions/%.c $(HDR) | $(BUILTIN_STARTED)
	@echo "[🔨] Builtin module: $(notdir $<)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILTIN_STARTED):
	@mkdir -p $(OBJDIR)/builtin_functions
	@echo "┌────────────────────────────────┐"
	@echo "│   🔨  Building builtin module  │"
	@echo "└────────────────────────────────┘"
	@touch $@

$(OBJDIR)/tokenization/%.o: $(SRCDIR)/tokenization/%.c $(HDR) | $(TOKENIZATION_STARTED)
	@echo "[🎯] Tokenization module: $(notdir $<)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TOKENIZATION_STARTED):
	@mkdir -p $(OBJDIR)/tokenization
	@echo "┌────────────────────────────────┐"
	@echo "│  🎯  Building tokenizer module │"
	@echo "└────────────────────────────────┘"
	@touch $@

$(OBJDIR)/new_expansion/%.o: $(SRCDIR)/new_expansion/%.c $(HDR) | $(EXPANSION_STARTED)
	@echo "[🔍] Expansion module: $(notdir $<)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(EXPANSION_STARTED):
	@mkdir -p $(OBJDIR)/new_expansion
	@echo "┌────────────────────────────────┐"
	@echo "│  🔍  Building expansion module │"
	@echo "└────────────────────────────────┘"
	@touch $@

$(OBJDIR)/parser/%.o: $(SRCDIR)/parser/%.c $(HDR) | $(PARSER_STARTED)
	@echo "[🌳] Parser module: $(notdir $<)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(PARSER_STARTED):
	@mkdir -p $(OBJDIR)/parser
	@echo "┌────────────────────────────────┐"
	@echo "│   🌳  Building parser module   │"
	@echo "└────────────────────────────────┘"
	@touch $@

$(OBJDIR)/execution/%.o: $(SRCDIR)/execution/%.c $(HDR) | $(EXECUTION_STARTED)
	@echo "[⚡] Execution module: $(notdir $<)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(EXECUTION_STARTED):
	@mkdir -p $(OBJDIR)/execution
	@echo "┌────────────────────────────────┐"
	@echo "│  ⚡  Building execution module │"
	@echo "└────────────────────────────────┘"
	@touch $@

$(OBJDIR)/shell/%.o: $(SRCDIR)/shell/%.c $(HDR) | $(SHELL_STARTED)
	@echo "[🐚] Shell module: $(notdir $<)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(SHELL_STARTED):
	@mkdir -p $(OBJDIR)/shell
	@echo "┌────────────────────────────────┐"
	@echo "│   🐚  Building shell module    │"
	@echo "└────────────────────────────────┘"
	@touch $@

$(OBJDIR)/main/%.o: $(SRCDIR)/main/%.c $(HDR) | $(MAIN_STARTED)
	@echo "[🎮] Main module: $(notdir $<)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(MAIN_STARTED):
	@mkdir -p $(OBJDIR)/main
	@echo "┌────────────────────────────────┐"
	@echo "│    🎮  Building main module    │"
	@echo "└────────────────────────────────┘"
	@touch $@

clean:
	@echo "[🧹] Cleaning object files…"
	@$(RM) -r $(OBJDIR)
	@echo "[✔️ ] Objects removed."

fclean: clean
	@echo "[💣] Nuking $(NAME) and libft..."
	@$(RM) $(NAME) $(LIBFT)
	@echo "[✔️ ] $(NAME) has been nuked."

re: fclean all

.PHONY: all clean fclean re
