####################################################################################################################################################
# Author        : Labhesh Patil
# Date          : Wed Feb 11 2026
# File          : Makefile
# Title         : Simple build configuration for Minishell
# Description   : Simple Makefile for compiling the minishell project with essential targets.
####################################################################################################################################################

CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = minishell

SRCS = main.c \
       common.c \
       execute_external_commands.c \
       execute_internal_commands.c \
       job_management.c \
       pipe_handling.c \
       signal_handling.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c main.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

cleanall:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run