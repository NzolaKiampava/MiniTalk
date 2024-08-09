/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:00:26 by nkiampav          #+#    #+#             */
/*   Updated: 2024/08/05 10:00:31 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk_bonus.h"

/*
  Funcao define os sinais para serem capturados pelo manipulador 
  personalizado.
  Caso essa acao falhe, imprime uma mensagem de erro no stdout e encerra
  o programa
*/
void	configure_sigaction_signals(struct sigaction *sa)
{
	if (sigaction(SIGUSR1, sa, NULL) < 0)
	{
		ft_putstr_fd("\e[31m## error - could not setup SIGUSR1 ##\n\e[0m",
			STDOUT_FILENO);
		exit(EXIT_FAILURE);
	}
	if (sigaction(SIGUSR2, sa, NULL) < 0)
	{
		ft_putstr_fd("\e[31m## error - could not setup SIGUSR2 ##\n\e[0m",
			STDOUT_FILENO);
		exit(EXIT_FAILURE);
	}
}

/*
  Funcao envia um inteiro contendo o comprimento da mensagem.
  Para cada bit enviado pelo cliente, espera-se um sinal de retorno
  antes de continuar, usando flag = 1 na funcao send_bit().
  Considera-se que 1 byte = 8 bits.
*/
void	send_int(pid_t pid, int num)
{
	int		shift;
	char	bit;

	shift = (sizeof(int) * 8) - 1;
	while (shift >= 0)
	{
		bit = (num >> shift) & 1;
		send_bit(pid, bit, 1);
		shift--;
	}
}

/*
  Funcao envia 1 char, que normalmente e um octeto (8 bits)
  Para cada bit enviado pelo cliente, espera-se um sinal de retorno antes
  de continuar, usando flag = 1 na funcao send_bit()
  Assumed 1 byte = 8 bits
*/
void	send_char(pid_t pid, char c)
{
	int		shift;
	char	bit;

	shift = (sizeof(char) * 8) - 1;
	while (shift >= 0)
	{
		bit = (c >> shift) & 1;
		send_bit(pid, bit, 1);
		shift--;
	}
}

/*
  Funcao envia um bit (0 / 1) para p processo com o PID especifico.
  O retorno da funcao ocorre apos o sinal de ACK ser recebido caso a flag
  de espera esteja definida como um valor diferente de zero,
  caso contrario, a funcao retornara imediatamente.
*/
void	send_bit(pid_t pid, char bit, char flag_to_pause)
{
	if (bit == 0)
	{
		if (kill(pid, SIGUSR1) < 0)
		{
			ft_putstr_fd("\e[31m## error - sending SIGUSR1 ##\n\e[0m",
				STDOUT_FILENO);
			exit(EXIT_FAILURE);
		}
	}
	else if (bit == 1)
	{
		if (kill(pid, SIGUSR2) < 0)
		{
			ft_putstr_fd("\e[31m## error - sending SIGUSR2 ##\n\e[0m",
				STDOUT_FILENO);
			exit(EXIT_FAILURE);
		}
	}
	if (flag_to_pause != 0)
		pause();
}
