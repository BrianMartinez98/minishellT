# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/04 18:59:16 by jarregui          #+#    #+#              #
#    Updated: 2025/08/07 01:51:13 by jarregui         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# INCLUDE COLOR VARIABLES DECLARATION:
include libs/colors.mk

# VARIABLES DECLARATION:
CC = gcc
CFLAGS = -Wall -Wextra -Werror
DEPENDENCY_FLAGS = -MMD -MP
DEBUG_FLAGS = -g -DDEBUG=1
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
		srcs/execute.c \
		srcs/main.c \
		srcs/readline.c \
		srcs/redirect.c \
		srcs/signals.c \
		srcs/token.c \
		srcs/utils.c
SRCS_BONUS 	= srcs/shell_bonus.c
OBJS = $(SRCS:.c=.o)
OBJS_BONUS = ${SRCS_BONUS:.c=.o}
LIBS = ${LIBFT_LIB}

# RULES DECLARATION:
all: subsystems shell

# CREACION ARCHIVOS OBJETO
%.o : %.c
	@echo "${ORANGE}${PROY_NAME} compiling file: $(BROWN)[$<]...${DEF_COLOR}"
	@$(CC) $(CFLAGS) $(DEPENDENCY_FLAGS) -c -o $@ $< 

subsystems:
# Do Make only if there are changes in the libraries:
	@if [ -f $(LIBFT_LIB) ]; then \
	if find $(LIBFT_DIR) -name "*.c" -newer $(LIBFT_LIB) | grep -q .; then \
	${MAKE} -C $(LIBFT_DIR) all; \
	fi; \
	else \
	${MAKE} -C $(LIBFT_DIR) all; \
	fi

$(EXEC_FILE_NAME): $(OBJS) $(LIBS)
	@$(CC) ${CFLAGS} $(DEPENDENCY_FLAGS) $(OBJS) ${LIBS} $(SYS_LIBS) -o $(EXEC_FILE_NAME)
	@echo "$(GREEN)✓ Created $(EXEC_FILE_NAME) File$(DEF_COLOR)\n"
shell: $(EXEC_FILE_NAME)

$(EXEC_FILE_NAME_BONUS): $(OBJS_BONUS) $(LIBS)
	@$(CC) ${CFLAGS} $(DEPENDENCY_FLAGS) srcs/server_bonus.o ${LIBS} -o $(EXEC_FILE_NAME_BONUS)
	@echo "$(GREEN)✓ Created $(EXEC_FILE_NAME_BONUS) File$(DEF_COLOR)\n"
shell_bonus: $(EXEC_FILE_NAME_BONUS)

bonus: subsystems shell_bonus

debug: fclean
	@$(MAKE) CFLAGS="$(DEBUG_FLAGS)" shell
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

-include $(OBJS:.o=.d) $(OBJS_BONUS:.o=.d)

.PHONY: all bonus clean cleanlibs fclean fcleanlibs re