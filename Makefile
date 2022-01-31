
SRC = ./src/Request.cpp ./src/main.cpp ./src/ConnectedNode.cpp  ./src/Response.cpp
HEADERS = ./src/ConnectedNode.hpp ./src/Request.hpp ./src/Response.hpp
OBJ = ${SRC:.cpp=.o}
#CFLAGCS = -Wall -Wextra -Werror -Wshadow -Wno-shadow -std='c++98'
CFLAGCS = -std='c++98'
CC = clang++
NAME = Webserv

%.o:		%.cpp
		$(CC) $(CFLAGCS) -c -o $@ $<

all: $(NAME)

${NAME}: ${OBJ}
	$(CC) $(CFLAGCS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

debug:
	$(CC) -g $(CFLAGCS) $(SRC) -o $(NAME)

re: fclean all

.PHONY : all clean fclean re