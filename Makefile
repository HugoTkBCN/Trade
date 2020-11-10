##
## EPITECH PROJECT, 2020
## Makefile
## File description:
## Trade
##

NAME		=	trade

SRCS    	=	./main.cpp \
            	./Trade.cpp
 
OBJS    	=	$(SRCS:.cpp=.o)

CPP			=	g++
RM      	=	rm -f
CPPFLAGS	=   -W -Wall -Wextra -Werror
CPPFLAGS	+=	-I./include/
LIBFLAGS	=   -lpthread

.cpp.o  :
	@$(CPP) $(CPPFLAGS) -c $< -o $@
	@printf "\033[39;1m%s \033[32;1m[Compiled]\033[39;1m\n" $<

all     :	$(OBJS)
	@$(CPP) $(OBJS) $(LIBFLAGS) -o $(NAME)
	@printf "\n \033[33;1m[Success]\033[39;1m Compilation done\n\n"

clean   :
	@$(RM) *~ $(OBJS)
	@printf "\033[39;1m%s \033[31;1m[Deleted] \033[39;1m\n" $(OBJS)

fclean  :       clean
	@$(RM) $(NAME)
	@printf "\n\033[39;1m%s \033[31;1m[Deleted] \033[39;1m\n\n" $(NAME)

re      :       fclean all

.PHONY: all clean fclean re