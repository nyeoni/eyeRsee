NAME = ft_irc

CC				= c++

ifdef DEBUG
	CFLAGS		= -g3 -fsanitize=address
else ifdef LEAKS
	CFLAGS		= -g
else
	CFLAGS 		= -Wall -Wextra -Werror
endif

RM				= rm -rf

INC_DIR = ./include/
INC = -I$(INC_DIR)

SRC_MAIN_DIR	= main/
SRC_MAIN 		= $(addprefix $(SRC_MAIN_DIR), main.cpp)

SRC_CONTROLLER_DIR	= controller/
SRC_CONTROLLER		= $(addprefix $(SRC_CONTROLLER_DIR), \
										StringController.cpp \
										ChannelController.cpp \
										Executor.cpp \
										ClientController.cpp)

SRC_ENTITY_DIR		= entity/
SRC_ENTITY			= $(addprefix $(SRC_ENTITY_DIR), \
										Channel.cpp \
										Client.cpp)

SRC_CORE_DIR		= core/
SRC_CORE			= $(addprefix $(SRC_CORE_DIR), \
										EventHandler.cpp \
										Server.cpp \
										Udata.cpp \
										Socket.cpp)

SRC					= $(SRC_MAIN) \
						$(SRC_CONTROLLER) \
						$(SRC_ENTITY) \
						$(SRC_CORE)

SRCS_DIR	= ./src/
SRCS 		= $(addprefix $(SRCS_DIR), $(SRC))

# OBJS_DIR	= $(addprefix $(OBJ_DIR), $(SRC_MAIN_DIR) \
# 										$(SRC_CONTROLLER_DIR) \
# 										$(SRC_ENTITY_DIR) \
# 										$(SRC_CORE_DIR))
OBJS_DIR		= ./obj/
OBJS 		= $(addprefix $(OBJS_DIR), $(SRC:.cpp=.o))

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)/$(SRC_MAIN_DIR)
	@mkdir -p $(OBJS_DIR)/$(SRC_CONTROLLER_DIR)
	@mkdir -p $(OBJS_DIR)/$(SRC_CORE_DIR)
	@mkdir -p $(OBJS_DIR)/$(SRC_ENTITY_DIR)
	@$(CC) $(CFLAGS) $(INC) -o $@ -c $<
	@echo $(CUT)$(BOLD)$(MINT) Compiling with $(CFLAGS)...$(RESET)
	@echo $(CUT)$(MAUVE) [$(notdir $<)] to [$(notdir $@)] $(RESET)
	@printf $(UP)$(UP)


$(NAME) : $(OBJS)
	@printf $(DOWN)$(CUT)
	@echo  $(BOLD)$(GREEN)"🔮 Creating ft_irc"$(RESET)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@printf $(CUT)$(CUT)
	@echo $(BOLD)$(MINT) 🎮 $(NAME) 🎮 $(RESET)$(BOLD)is ready 🎉 $(RESET)

.PHONY : all
all: $(NAME)


.PHONY : clean
clean:
	@$(RM) $(OBJS_DIR) $(NAME).dSYM
	@echo $(BOLD)$(MAUVE) 🗒 object files $(RESET)$(BOLD)have been cleaned....🗑️$(RESET)

.PHONY : fclean
fclean: clean
	@$(RM) $(NAME)
	@echo $(BOLD)$(MINT) 🎮 $(NAME) 🎮 $(RESET)$(BOLD)has been cleaned....🗑️$(RESET)

.PHONY : re
re: fclean all

.PHONY	: debug
debug	:
	@make fclean
	@make DEBUG=1 all

.PHONY	: leaks
leaks	:
	@make LEAKS=1 all

######################### Color #########################
GREEN="\033[32m"
MINT="\033[38;5;51m"
L_PURPLE="\033[38;5;55m"
MAUVE="\033[38;5;147m"
PINK="\033[38;5;175m"
RESET="\033[0m"
BOLD="\033[1m"
UP = "\033[A"
DOWN = "\033[B"
RIGHT = "\033[C"
LEFT = "\033[D"
CUT = "\033[K"
SAVE = "\033[s"
RESTORE = "\033[u"
