# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/04 18:59:16 by jarregui          #+#    #+#              #
#    Updated: 2025/08/04 19:54:51 by jarregui         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# INCLUDE COLOR VARIABLES DECLARATION:
include libs/colors.mk

# VARIABLES DECLARATION:
CC = gcc
CFLAGS = -Wall -Wextra -Werror 
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
		srcs/redirect.c \
		srcs/signals.c \
		srcs/token.c \
		srcs/utils.c \
		srcs/redirection_utils.c
SRCS_BONUS 	= srcs/shell_bonus.c
OBJS = $(SRCS:.c=.o)
OBJS_BONUS = ${SRCS_BONUS:.c=.o}
LIBS = ${LIBFT_LIB}

# RULES DECLARATION:
all: subsystems shell

# CREACION ARCHIVOS OBJETO
%.o : %.c
	@echo "${ORANGE}${PROY_NAME} compiling file: $(BROWN)[$<]...${DEF_COLOR}"
	@$(CC) $(CFLAGS) -c -o $@ $< 

subsystems:
# Do Make only if there are changes in the libraries:
	@if [ -f $(LIBFT_LIB) ]; then \
	if find $(LIBFT_DIR) -name "*.c" -newer $(LIBFT_LIB) | grep -q .; then \
	${MAKE} -C $(LIBFT_DIR) all; \
	fi; \
	else \
	${MAKE} -C $(LIBFT_DIR) all; \
	fi

shell: $(OBJS) $(LIBS)
	@$(CC) ${CFLAGS} $(OBJS) ${LIBS} $(SYS_LIBS) -o $(EXEC_FILE_NAME)
	@echo "$(GREEN)✓ Created $(EXEC_FILE_NAME) File$(DEF_COLOR)\n"

shell_bonus: srcs/server_bonus.o $(LIBS)
	@$(CC) ${CFLAGS} srcs/server_bonus.o ${LIBS} -o $(EXEC_FILE_NAME_BONUS)
	@echo "$(GREEN)✓ Created $(EXEC_FILE_NAME_BONUS) File$(DEF_COLOR)\n"

bonus: subsystems shell_bonus

clean:
	@${RM} ${OBJS} ${OBJS_BONUS}
	@echo "\n$(GREEN)✓ All objects from ${PROY_NAME} cleaned successfully$(DEF_COLOR)"
	@echo "$(GREEN)Note: run $(YELLOW)make cleanlibs$(GREEN) if also want to clean all libraries $(DEF_COLOR)\n"

cleanlibs: 
	@${MAKE} -C $(LIBFT_DIR) clean

fclean: clean
	@${RM} ${EXEC_FILE_NAME} ${EXEC_FILE_CLIENT_NAME} ${EXEC_FILE_NAME_BONUS} ${EXEC_FILE_CLIENT_NAME_BONUS}
	@echo "\n$(GREEN)✓ All executable from ${PROY_NAME} cleaned successfully $(DEF_COLOR)"
	@echo "$(GREEN)Note: run $(YELLOW)make fcleanlibs$(GREEN) if also want to fclean all libraries $(DEF_COLOR)\n"

fcleanlibs: 
	@${MAKE} -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all bonus clean cleanlibs fclean fcleanlibs re