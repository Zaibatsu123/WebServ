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
RESPONSEPATH	= src/response/
INC				= inc
HEADER			= inc/output.hpp inc/global.hpp
SRC				:=	main.cpp	\
					\
					webserver.cpp \
					Config.cpp \
					utils.cpp \
					Request.cpp \
					Server.cpp

SRC_RESPONSE	:=	createResponse.cpp \
                    cgiHandler.cpp \
                    methods.cpp \
                    upload.cpp \
                    GoodResponse.cpp \
                    BadResponse.cpp \
                    RedirectResponse.cpp \
                    CgiResponse.cpp \
                    PutResponse.cpp \
                    AutoIndexResponse.cpp \
                    AResponse.cpp \
                    location_parse.cpp \


OBJ				:= $(addprefix $(OBJPATH), $(SRC:.cpp=.o))

OBJ_RESPONSE	:= $(addprefix $(OBJPATH), $(SRC_RESPONSE:.cpp=.o))

$(OBJPATH)%.o: $(RESPONSEPATH)%.cpp $(HEADER)
	mkdir -p obj
	$(CC) $(FLAGS) -c $< -o $@ -I$(INC)

$(OBJPATH)%.o: %.cpp $(HEADER)
	mkdir -p obj
	$(CC) $(FLAGS) -c $< -o $@ -I$(INC)

$(NAME): $(OBJ) $(OBJ_RESPONSE)
	$(CC) $(FLAGS) $^ -o $@ -I$(INC)

all: $(NAME)

clean:
	rm -rf obj

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
