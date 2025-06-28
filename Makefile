# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yitani <yitani@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/28 01:31:15 by yitani            #+#    #+#              #
#    Updated: 2025/06/28 01:32:10 by yitani           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME      = minishell

CC        = cc

CFLAGS    = -Wall -Wextra -Werror

SRC		  = 

HDR		  = 
			
OBJ		  = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@echo "┌────────────────────────────────┐"
	@echo "│   🚀  Building minishell...    │"
	@echo "└────────────────────────────────┘"
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "✅  Build complete! Enjoy."

%.o: %.c
	@echo "[🛠️ ] Compiling $<…"
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "[✔️ ] Created $@"

clean:
	@echo "[🧹] Cleaning object files…"
	@rm -f $(OBJ)
	@echo "[✔️ ] Objects removed."

fclean: clean
	@echo "[💣] Nuking $(NAME)"
	@rm -f $(NAME)
	@echo "[✔️ ] $(NAME) has been nuked."

re: fclean all

norminette:
	@echo "🔍 Running Norminette on all listed files…"
	@norminette $(SRC) $(HDR)

.PHONY: all clean fclean re norminette