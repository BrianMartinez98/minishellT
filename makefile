# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jarregui <jarregui@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/04 18:59:16 by jarregui          #+#    #+#              #
#    Updated: 2025/09/11 11:55:05 by jarregui         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# INCLUDE COLOR VARIABLES DECLARATION:
include libs/colors.mk

# VARIABLES DECLARATION:
CC = gcc
CFLAGS = -Wall -Wextra -Werror
DEBUG_FLAGS = -g -DDEBUG=1
VALGRIND_FLAGS = -g
# VALGRIND = valgrind --leak-check=full --track-fds=yes --track-origins=yes --show-leak-kinds=all
VALGRIND = valgrind --leak-check=summary
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
		srcs/execute.c \
		srcs/main.c \
		srcs/readline.c \
		srcs/redirection_utils.c \
		srcs/shell.c \
		srcs/signals.c \
		srcs/token.c \
		srcs/utils.c \
		srcs/pipes.c
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

debug: fclean $(LIBFT_LIB)
	@$(MAKE) CFLAGS="$(CFLAGS) $(DEBUG_FLAGS)" shell
	@echo "$(GREEN)DEBUG: 🛠️  launching minishell with debug messages$(DEF_COLOR)"
	@./$(EXEC_FILE_NAME)

valgrind: fclean $(LIBFT_LIB)
	@$(MAKE) CFLAGS="$(CFLAGS) $(VALGRIND_FLAGS)" shell
	@echo "$(GREEN)DEBUG: 🔍 Launching Valgrind with debug build...$(DEF_COLOR)\n"
	@echo "$(CYAN)💧 Valgrind launched$(DEF_COLOR)"
	@$(VALGRIND) ./$(EXEC_FILE_NAME) 2>&1 | tee valgrind.log
	@echo "$(YELLOW)⚠️ Note: memory still reachable is due to readline and is not considered leak$(DEF_COLOR)"
	@echo "$(CYAN)💧 Leak Summary (if any):$(DEF_COLOR)"
	@grep -q "definitely lost: 0 bytes in 0 blocks" valgrind.log && \
		echo "$(GREEN)✅ No memory leaks detected$(DEF_COLOR)" || \
		(echo "$(RED)❌ MEM LEAK DETECTED! Check summary at valgrind.log$(DEF_COLOR)"; grep -A5 "LEAK SUMMARY:" valgrind.log)
	
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