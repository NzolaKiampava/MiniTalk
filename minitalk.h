/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:19:27 by nkiampav          #+#    #+#             */
/*   Updated: 2024/07/24 11:19:32 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ======================== DEFINITIONS

#ifndef MINITALK_H
# define MINITALK_H

# define WAIT_US  100 // usleep()

// ======================== INCLUDES

# include "libft/libft.h" // ft_atoi(), ft_strlen(), ft_putstr(), ft_putnbr()
# include <unistd.h> // getpid(), write()
# include <signal.h> // sigemptyset(), sigaction()
# include <stdlib.h> // exit()

// ======================== PROTOTYPES

// minitalk.c
void	configure_sigaction_signals(struct sigaction *sa);
void	send_int(pid_t pid, int num);
void	send_char(pid_t pid, char c);
void	send_bit(pid_t pid, char bit, char flag_to_pause);

// ======================== TYPEDEF and STRUCTURES

/*
  Structure to handle the protocol between server-client
  
  bits : bits received
  data : data received, can be an integer (length of message) or char
  flag : 1 if length of message was received, else 0
  message : message received
*/
typedef struct s_protocol
{
	int		bits;
	int		data;
	int		flag;
	char	*message;
}			t_protocol;

#endif
