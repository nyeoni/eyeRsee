NAME = ft_irc

CC := c++
FLAGS := -Wall -Werror -Wextra #-g -fsanitize=address

INC := -I include

MAIN_SRC := main.cpp
_SRC := $(addprefix src/, \
		 Server.cpp \
		 Socket.cpp
		 )
# # 		 Channel.cpp
SRC := $(MAIN_SRC) $(_SRC)
OBJ := $(patsubst src%, obj%, $(SRC:.cpp=.o))

obj/%.o : src/%.cpp
	$(CC) $(FLAGS) $(INC) -o $@ -c $<

all: obj $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(FLAGS) -o $@ $^ $(LIB)

clean:
	rm -rf obj

fclean: clean
	@rm -rf $(NAME)

obj:
	@mkdir -p obj

re: fclean all

.PHONY: all clean bonus fclean re