NAME := codexion

CC := cc
CFLAGS := -Wall -Wextra -Werror -MMD -MP -g -pthread

SRCDIR := src
OBJDIR := build

SRC := $(SRCDIR)/main.c \
			 $(SRCDIR)/args.c \
			 $(SRCDIR)/args_utils.c \
			 $(SRCDIR)/utils.c \
			 $(SRCDIR)/init.c \
			 $(SRCDIR)/heap.c \
			 $(SRCDIR)/dongle.c \
			 $(SRCDIR)/coder.c \
			 $(SRCDIR)/monitor.c

OBJ := $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)


DEPS := $(OBJ:.o=.d)

INCLUDES = -I$(SRCDIR) -I.

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)
