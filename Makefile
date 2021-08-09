#-fsanitize=address
NAME				=	minishell.a
OUTPUT				=	minishell
PID					=	.pid

CC					=	@gcc
SH					=	@bash
RM					=	@/bin/rm -rf
FLAGS				=	-Wextra -Wall -Werror -g3 -fsanitize=address

HEADER_DIR			=	includes/
OBJ_DIR				=	obj/
BUILTINS_DIR		=	builtins/
LISTS_DIR			=	lists/
SHELL_LISTS_DIR		=	shell/
REDIRECTS_LISTS_DIR	=	redirects/
HISTORY_LISTS_DIR	=	history/
ARGS_LISTS_DIR		=	args/
ARG_LISTS_DIR		=	arg/
ENV_LISTS_DIR		=	env/
UTILS_DIR			=	utils/
SRC_DIR				=	srcs/
PARSING_DIR			=	parsing/
VALIDATION_DIR		=	validation/
LIBFT_DIR			=	libft/
ifeq ($(shell whoami), runner)
	COVID_NORM		=	ruby ~/.norminette/norminette.rb
else
	COVID_NORM		=	ruby -e STDOUT.sync=true -e 'load($$0=ARGV.shift)' ~/.norminette/norminette.rb
endif
RESULT				=	$(shell cat output.txt)
AUTHOR				=	$(shell cat author)
LAST_COMMIT_DATE	=	$(shell git log -1 --date=format:"%m/%d/%Y" --format="%ad   [%cr]")
LAST_COMMIT_HASH	=	$(shell git log -1 --date=format:"%m/%d/%y %T" --format="%H")
LAST_COMMIT_MESSAGE	=	$(shell git log -1 --date=format:"%m/%d/%y %T" --format=\'%s\')
OS					=	$(shell lsb_release -si)
USER				=	$(shell whoami)
ARCH				=	$(shell uname -m | sed 's/x86_//;s/i[3-6]86/32/')
VER					=	$(shell lsb_release -sr)

# Mandatory part

SRCS				=	init.c					parser.c					sub_parser.c				initshell.c			\
						fix_cmd.c

BUILTINS			=	builtins/echo.c			builtins/pwd.c				builtins/cd.c				builtins/unset.c	\
						builtins/exit.c			builtins/export/export.c	builtins/export/utils.c		builtins/env/env.c	\
						builtins/env/utils.c	builtins/export/utils2.c

UTILS				=	utils/builtins.c		utils/file.c				utils/signals.c				utils/execution.c		\
						utils/termcaps.c 		utils/parsing.c				utils/parsing/cmd.c			utils/parsing/tilde.c	\
						utils/parsing/dollar.c	utils/validation/quotes.c	utils/validation/redirect.c	utils/control.c	\
						utils/termtext.c		utils/termhist.c			utils/redirection.c			utils/args.c \
						utils/validation/pipe.c	utils/error.c				utils/split.c				utils/loureed.c	\
						utils/clear_strings.c

SHELL_LISTS			=	lists/shell/ft_slstadd_back.c		lists/shell/ft_slstadd_front.c	lists/shell/ft_slstclear.c	\
						lists/shell/ft_slstdelone.c			lists/shell/ft_slstiter.c		lists/shell/ft_slstlast.c	\
						lists/shell/ft_slstnew.c			lists/shell/ft_slstsize.c

REDIRECTS_LISTS		=	lists/redirects/ft_rlstadd_back.c	lists/redirects/ft_rlstadd_front.c	lists/redirects/ft_rlstclear.c	\
						lists/redirects/ft_rlstdelone.c		lists/redirects/ft_rlstiter.c		lists/redirects/ft_rlstlast.c	\
						lists/redirects/ft_rlstnew.c		lists/redirects/ft_rlstsize.c

HISTORY_LISTS		=	lists/history/ft_hlstadd_back.c		lists/history/ft_hlstadd_front.c	lists/history/ft_hlstclear.c	\
						lists/history/ft_hlstdelone.c		lists/history/ft_hlstfirst.c		lists/history/ft_hlstlast.c		\
						lists/history/ft_hlstnew.c			lists/history/ft_hlstsize.c

ARGS_LISTS			=	lists/args/ft_alstadd_back.c		lists/args/ft_alstadd_front.c		lists/args/ft_alstclear.c	\
						lists/args/ft_alstdelone.c			lists/args/ft_alstfirst.c			lists/args/ft_alstlast.c	\
						lists/args/ft_alstnew.c				lists/args/ft_alstsize.c			lists/args/ft_alstmap.c		\
						lists/args/ft_alstfilter.c

ENV_LISTS			=	lists/env/ft_envlstadd_back.c		lists/env/ft_envlstadd_front.c		lists/env/ft_envlstclear.c	\
						lists/env/ft_envlstdelone.c			lists/env/ft_envlstfirst.c			lists/env/ft_envlstlast.c	\
						lists/env/ft_envlstnew.c			lists/env/ft_envlstsize.c			lists/env/ft_envlstmap.c		\
						lists/env/ft_envlstfilter.c

SARG_LISTS			=	lists/arg/ft_aslstadd_back.c		lists/arg/ft_aslstadd_front.c		lists/arg/ft_aslstclear.c	\
						lists/arg/ft_aslstdelone.c			lists/arg/ft_aslstfirst.c			lists/arg/ft_aslstlast.c	\
						lists/arg/ft_aslstnew.c				lists/arg/ft_aslstsize.c

LISTS				=	$(SHELL_LISTS) $(REDIRECTS_LISTS) $(HISTORY_LISTS) $(ARGS_LISTS) $(SARG_LISTS) $(ENV_LISTS)

SOURCES				=	$(SRCS) $(BUILTINS) $(UTILS) $(LISTS)

# Bonus part

LEAKS_FLAGS			=	--tool=memcheck --leak-check=full --leak-resolution=high --show-leak-kinds=all --track-origins=yes


LEAKS_EXE			=	./tools/memory_leak.sh ${OUTPUT} ${LEAKS_FLAGS}
NORME				=	$(addsuffix *.h,$(HEADER_DIR)) \
						$(addprefix $(SRC_DIR),$(SOURCES)) \
						$(addsuffix **/*.h,$(LIBFT_DIR)) \
						$(addsuffix **/*.c,$(LIBFT_DIR)) \
						$(addsuffix **/**/*.c,$(LIBFT_DIR))


# Mandatory Objects
OFILE				=	$(SOURCES:%.c=%.o)
OBJS				=	$(addprefix $(OBJ_DIR), $(OFILE))

# Functions
disp_indent			=	for I in `seq 1 $(MAKELEVEL)`; do \
						test "$(MAKELEVEL)" '!=' '0' && printf "\t"; \
						done

exec_color			=	$(call disp_indent); \
						echo $(1)$(2) $(3) $(4) $(5) $(6) $(7) $(8) $(9) $(10)"\033[31m"; \
						$(2) $(3) $(4) $(5) $(6) $(7) $(8) $(9) $(10)

disp_title			=	$(call disp_indent); \
						echo "$(2)[  $(1) \#$(MAKELEVEL)  ]\033[0m"


################
##   COLORS   ##
################

Y					=	$(shell printf "\033[33m")
R					=	$(shell printf "\033[31m")
G					=	$(shell printf "\033[32m")
CYAN				=	$(shell printf "\033[36m")
B					=	$(shell printf "\033[34m")
X					=	$(shell printf "\033[0m")
UP					=	$(shell printf "\033[A")
CUT					=	$(shell printf "\033[K")
W					=	$(shell printf "\033[37m")
UND					=	$(shell printf "\033[4m")
BLINK				=	$(shell printf "\033[5m")
BOLD				=	$(shell printf "\033[1m")
UP					=	$(shell printf "\033[5A")
NORM_COLOR_T		=	$(shell printf "\033[0;1;141m")
NORM_COLOR			=	$(shell printf "\033[0;38;5;153m")
NORM_COLOR_WAR		=	$(shell printf "\033[38;5;214m")
NORM_COLOR_ERR		=	$(shell printf "\033[0;41m")
BG_X				=	$(shell printf "\033[0;39m")
TAB					=	$(shell printf "\t")

# Make all files.
all:
			@make $(NAME)

# Objects directory
$(OBJ_DIR):
			@make -C $(LIBFT_DIR)
			@echo ${CUT}[${Y}$(OUTPUT)]${X} ${B}Creating: ${R}$(OBJ_DIR)${X}
			@mkdir -p $(OBJ_DIR)/$(BUILTINS_DIR)/export
			@mkdir -p $(OBJ_DIR)/$(BUILTINS_DIR)/env
			@mkdir -p $(OBJ_DIR)/$(UTILS_DIR)/$(PARSING_DIR)
			@mkdir -p $(OBJ_DIR)/$(UTILS_DIR)/$(VALIDATION_DIR)
			@mkdir -p $(OBJ_DIR)/$(LISTS_DIR)/$(SHELL_LISTS_DIR)
			@mkdir -p $(OBJ_DIR)/$(LISTS_DIR)/$(REDIRECTS_LISTS_DIR)
			@mkdir -p $(OBJ_DIR)/$(LISTS_DIR)/$(HISTORY_LISTS_DIR)
			@mkdir -p $(OBJ_DIR)/$(LISTS_DIR)/$(ARGS_LISTS_DIR)
			@mkdir -p $(OBJ_DIR)/$(LISTS_DIR)/$(ARGS_LISTS_DIR)
			@mkdir -p $(OBJ_DIR)/$(LISTS_DIR)/$(ARG_LISTS_DIR)
			@mkdir -p $(OBJ_DIR)/$(LISTS_DIR)/$(ENV_LISTS_DIR)

# Normal objects
$(NAME): $(OBJ_DIR) $(OBJS)
			@echo ${B}[-----------------------------------------]${X}
			@echo ${B}[------------ ${BOLD}OK${X}${B} - ${BOLD}$(NAME)${X}${B} -----------]${X}
			@echo ${B}[----- 🦑 Iä, iä, Cthulhu fhtagn! 🦑 -----]${X}
			@echo ${B}[-----------------------------------------]${X}

$(OBJS):
			@printf '${CUT}[${Y}$(OUTPUT)]${X} ${B}Compiling: ${X}'
			@make $(OFILE)
			@echo ${B}Crafting: ${R}$(OUTPUT)${X}
			$(CC) main.c $(OBJS) $(LIBFT_DIR)libft.a -ltermcap -I $(HEADER_DIR) $(FLAGS) -o $(OUTPUT)
			@echo

$(OFILE):
			@printf ${R}'$(@:%.o=%.c) '${X}
			$(CC) -o $(OBJ_DIR)$@ -I$(HEADER_DIR) -c $(SRC_DIR)$(@:%.o=%.c) $(FLAGS)

##@ Cleaning
clean:		## Clean all objects.
			$(RM) $(OBJ_DIR)


fclean:		## Remove all objects and executables.
			$(RM) $(OUTPUT)
			$(RM) $(OBJ_DIR)
			$(RM) $(NAME)
			@echo ${CUT}[${Y}$(OUTPUT)]${X} [Garbage collector:${Y} ${OUTPUT} $(NAME) *.o${X}][${G}OK!${X}]
			@make fclean -C $(LIBFT_DIR)

##@ Checks
normi:		## Check norminette.
			@$(call disp_title,Checking norminette,$(NORM_COLOR_T))
			@if [ ${OS} = "Ubuntu" ]; then\
				norminette $(NORME) | sed "s/Norme/$(NORM_COLOR_T)➤  $(NORM_COLOR)Norme/g;s/Warning/\t $(NORM_COLOR_WAR)Warning/g;s/Error/\t🚨 $(NORM_COLOR_ERR)Error/gm;s/$$/$(X)/g"; \
			else\
				norminette $(NORME) | sed "s/Norme/$(NORM_COLOR_T)➤  $(NORM_COLOR)Norme/g;s/Warning/\t $(NORM_COLOR_WAR)Warning/g;s/Error:/$(TAB)🚨 $(NORM_COLOR_ERR)Error:/g;s/$$/$(X)/g"; \
			fi

##@ Compilation

re:			## Call fclean => all
			@make clean
			@make all

##@ Testing

test:		## Make minishell test
			make re && cd 42_minishell_tester && ./grademe.sh -c

leak:		## Run memory leak for valid cub file.
			@if [ $(shell ./tools/memory_leak.sh $(OUTPUT) -c $$FOO $(LEAKS_FLAGS) && cat valgrind_out | grep "definitely lost:" | cut -d : -f 2 | cut -d b  -f 1 | tr -d " " | tr -d ",") ]; then\
				echo ${BOLD}${UND}${R}🚨 Memory leaks detected${X};\
				if [ $(USER) = "runner" ]; then\
					cat valgrind_out;\
				else\
					echo ${G};cat valgrind_out | grep -A2 "HEAP SUMMARY:" | cut -d = -f 5 | cut -c 2-;\
					echo ${CYAN};cat valgrind_out | grep -A5 "LEAK SUMMARY:" | cut -d = -f 5 | cut -c 2-;\
				fi;\
				exit 1;\
			else\
				echo ☕ ${BOLD}${UND}${G}No memory leaks detected${X} ☕;\
				echo ${CYAN};cat valgrind_out | grep -A4 "HEAP SUMMARY:" | cut -d = -f 5 | cut -c 2-;\
			fi

##@ Helpers
kill:		## Kill all minishells
			@pkill -f $(OUTPUT)

escape:		## Kill all minishells
			./tools/escaper.sh
##@ Help
help:		## View all available commands.
			$(shell $(TARGETS_EXE))
			@echo ${CYAN} Proyecto: ${UND}${BOLD}$(OUTPUT)${CYAN}${X}${CYAN}${UND}\\t\\t\\t\\t\\tAuthor: $(AUTHOR)${X}
			@echo ${CYAN} Last commit:${X}
			@echo ${CYAN} \\tDate: $(LAST_COMMIT_DATE)
			@echo ${CYAN} \\tHash: $(LAST_COMMIT_HASH)${X}
			@echo ${CYAN} \\tMessage: $(LAST_COMMIT_MESSAGE)${X}
			@echo ${CYAN}--------------------------------------------------------------------------
			@echo ${CYAN}Available commands: "\\t\\t\\t\\t(${G}Usage:${X} make ${CYAN}<target>${X})"
			@awk 'BEGIN {FS = ":.*##"; printf ""} /^[a-zA-Z_-]+:.*?##/ { printf "  ${CYAN}%-25s${X} %s\n", $$1, $$2 } /^##@/ { printf "\n${UND}${BOLD}%s${X}\n", substr($$0, 5) } ' $(MAKEFILE_LIST)
			@echo ${CYAN}--------------------------------------------------------------------------

.PHONY:		all clean flclean re test
