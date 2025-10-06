# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/04 18:59:16 by jarregui          #+#    #+#              #
#    Updated: 2025/10/06 23:14:13 by jarregui         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# INCLUDE COLOR VARIABLES DECLARATION:
include libs/colors.mk

# VARIABLES DECLARATION:
CC = gcc
CFLAGS = -Wall -Wextra -Werror
DEBUG_FLAGS = -g -DDEBUG=1
SANITIZE_FLAGS = -g3 -fsanitize=address
VALGRIND_FLAGS = -g
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log
# VALGRIND = valgrind --leak-check=summary
PROY_NAME =	MINISHELL
EXEC_FILE_NAME = minishell
EXEC_FILE_NAME_BONUS = minishell_bonus
SYS_LIBS = -lreadline
RM = rm -f

#LIBRARIES
LIBFT_DIR = libs/libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a

# SOURCES
SRCS 	= builtins/echo.c \
		builtins/environment.c \
		builtins/export.c \
		builtins/history.c \
		builtins/unset.c \
		builtins/cd.c \
		srcs/execute_builtin.c \
		srcs/execute.c \
		srcs/main.c \
		srcs/readline.c \
		srcs/redirect.c \
		srcs/shell_exit.c \
		srcs/shell.c \
		srcs/signals.c \
		srcs/token.c \
		srcs/utils_print.c \
		srcs/utils_quotes.c \
		srcs/utils_redirect.c \
		srcs/utils_token.c \
		srcs/utils.c \
		
SRCS_BONUS 	= srcs/shell_bonus.c
OBJS = $(SRCS:.c=.o)
OBJS_BONUS = ${SRCS_BONUS:.c=.o}

# RULES DECLARATION:
all: subsystems shell

# CREACION ARCHIVOS OBJETO
%.o : %.c
	@echo "${ORANGE}${PROY_NAME} compiling file: $(BROWN)[$<]...${DEF_COLOR}"
	@$(CC) $(CFLAGS) -c -o $@ $< 

# LIBFT
subsystems: $(LIBFT_LIB)

$(LIBFT_LIB):
	@$(MAKE) -C $(LIBFT_DIR) all

# MINISHELL
$(EXEC_FILE_NAME): $(OBJS) $(LIBFT_LIB)
	@$(CC) ${CFLAGS} $(OBJS) ${LIBFT_LIB} $(SYS_LIBS) -o $(EXEC_FILE_NAME)
	@echo "$(GREEN)✓ Created $(EXEC_FILE_NAME) File$(DEF_COLOR)\n"
shell: $(EXEC_FILE_NAME)

$(EXEC_FILE_NAME_BONUS): $(OBJS_BONUS) $(LIBFT_LIB)
	@$(CC) ${CFLAGS} srcs/server_bonus.o ${LIBFT_LIB} -o $(EXEC_FILE_NAME_BONUS)
	@echo "$(GREEN)✓ Created $(EXEC_FImakeLE_NAME_BONUS) File$(DEF_COLOR)\n"
shell_bonus: $(EXEC_FILE_NAME_BONUS)

bonus: subsystems shell_bonus

debug: $(LIBFT_LIB)
	@$(MAKE) CFLAGS="$(CFLAGS) $(DEBUG_FLAGS) $(SANITIZE_FLAGS)" shell
	@echo "$(GREEN)DEBUG: 🛠️  launching minishell with debug messages$(DEF_COLOR)"
	@./$(EXEC_FILE_NAME)

valgrind: fclean $(LIBFT_LIB)
	@$(MAKE) CFLAGS="$(CFLAGS) $(VALGRIND_FLAGS)" shell
	@echo "$(GREEN)DEBUG: 🔍 Launching Valgrind with full leak check...$(DEF_COLOR)\n"
	@$(VALGRIND) ./$(EXEC_FILE_NAME)
	@echo "$(YELLOW)⚠️ Note: memory still reachable is due to readline and is not considered leak$(DEF_COLOR)"
	@echo "$(CYAN)💧 Leak Summary (from valgrind.log):$(DEF_COLOR)"
	@grep -A5 "LEAK SUMMARY:" valgrind.log
	@grep "definitely lost:" valgrind.log | grep -v "0 bytes" && \
		(echo "$(RED)❌ MEM LEAK DETECTED!$(DEF_COLOR)"; exit 1) || \
		echo "$(GREEN)✅ No memory leaks detected$(DEF_COLOR)"
		
clean:
	@${RM} ${OBJS} ${OBJS_BONUS} $(OBJS:.o=.d) $(OBJS_BONUS:.o=.d) valgrind.log
	@echo "\n$(GREEN)✓ All objects from ${PROY_NAME} cleaned successfully$(DEF_COLOR)"
	@echo "$(GREEN)Note: run $(YELLOW)make cleanlibs$(GREEN) if also want to clean all libraries $(DEF_COLOR)\n"

cleanlibs: 
	@${MAKE} -C $(LIBFT_DIR) clean

fclean: clean
	@${RM} ${EXEC_FILE_NAME} ${EXEC_FILE_NAME_BONUS} valgrind.log
	@echo "\n$(GREEN)✓ All executable from ${PROY_NAME} cleaned successfully $(DEF_COLOR)"
	@echo "$(GREEN)Note: run $(YELLOW)make fcleanlibs$(GREEN) if also want to fclean all libraries $(DEF_COLOR)\n"

fcleanlibs: 
	@${MAKE} -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all bonus clean cleanlibs fclean fcleanlibs re