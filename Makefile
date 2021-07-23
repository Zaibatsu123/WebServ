##                                   ##
## Created by:                       ##
##              Equal rights team    ##
##         on:                       ##
##              7/07/21              ##
##                                   ##

NAME			= webserv
FLAGS			= -Wall -Wextra -Werror
CC				= clang++
VPATH			= src
OBJPATH			= obj/
INC				= inc
HEADER			=
SRC				:=	main.cpp	\
					createResponse.cpp \
					Response.cpp \
					webserver.cpp \
					Config.cpp 
PARS			:= 	utils.cpp \
					Request.cpp 

OBJ				:= $(addprefix $(OBJPATH), $(SRC:.cpp=.o), $(PARS:.cpp=.o))

$(OBJPATH)%.o: %.cpp $(HEADER)
	mkdir -p obj
	$(CC) $(FLAGS) -c $< -o $@ -I$(INC)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $^ -o $@ -I$(INC)

all: $(NAME)

clean:
	rm -rf obj

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re