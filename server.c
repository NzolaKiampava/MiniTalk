/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:19:47 by nkiampav          #+#    #+#             */
/*   Updated: 2024/07/24 11:19:51 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/*
  A funcao verifica se a mensagem em string esta completa.
  Caso o terminador nulo da string seja recebida, imprime a string inteira
  e libera a memoria alocada na heap para string. Em seguida, informa ao 
  cliente que a mensagem foi recebida (o cliente entao saira), usando
  send_bit() sem esperar um sinal de confirmacao do cliente.
*/
void	server_is_message_finished(
	t_protocol *t_server, int *i, pid_t client_pid)
{
	if (t_server->bits == 8 && t_server->flag == 1)
	{
		t_server->message[*i] = t_server->data;
		(*i)++;
		if (t_server->data == '\0')
		{
			ft_putstr_fd("\e[92mMensagem Recebida = [", STDOUT_FILENO);
			ft_putstr_fd(t_server->message, STDOUT_FILENO);
			ft_putstr_fd("]\n\e[0m", STDOUT_FILENO);
			free(t_server->message);
			t_server->message = NULL;
			t_server->flag = 0;
			*i = 0;
			send_bit(client_pid, 1, 0);
		}
		t_server->bits = 0;
	}
}

/*
  Funcao verifica se todos os bits do comprimento da string foram processados,
  Se sim, o comprimento é impresso no stdout e é usado para alocar na memoria
  heap umas string com o tamanho exato recebido (mais o terminador nulo)
*/
void	server_is_str_length_finished(t_protocol *t_server)
{
	if (t_server->bits == sizeof(int) * 8 && t_server->flag == 0)
	{
		t_server->flag = 1;
		ft_putstr_fd("\e[36mTamanho da Mensagem = [", STDOUT_FILENO);
		ft_putnbr_fd(t_server->data, STDOUT_FILENO);
		ft_putstr_fd("]\n\e[0m", STDOUT_FILENO);
		t_server->message = ft_calloc(t_server->data + 1, sizeof(char));
		if (t_server->message == NULL)
		{
			ft_putstr_fd("\e[31m## error - ft_calloc() ##\n\e[0m", STDOUT_FILENO);
			exit(EXIT_FAILURE);
		}
		t_server->message[t_server->data] = '\0';
		t_server->bits = 0;
	}
}

/*
  Funcao captura os sinais SIGUSR1 e SIGUSR2 recebidos do cliente, que
  representam o status de cada bit (0 / 1).
  As variaveis declaradas como static sao inicializadas com zero e, apos
  cada 8 bits recebidos, sao novamente re-inicializadas para zero.
  Para cada bit recebido do cliente, o servidor envia um sinal de ACK
  (o cliente deve enviar o proximo bit somente apos receber esse sinal de ACK)
*/
void	server_handler(int num, siginfo_t *info, void *context)
{
	static t_protocol	t_server;
	static int			i;

	usleep(WAIT_US);
	(void)context;
	(void)info;
	if (t_server.bits == 0)
		t_server.data = 0;
	if (num == SIGUSR2 && t_server.flag == 0)
		t_server.data |= 1 << (((sizeof(int) * 8) - 1) - t_server.bits);
	else if (num == SIGUSR2 && t_server.flag == 1)
		t_server.data |= 1 << (((sizeof(char) * 8) - 1) - t_server.bits);
	t_server.bits++;
	server_is_str_length_finished(&t_server);
	server_is_message_finished(&t_server, &i, info->si_pid);
	send_bit(info->si_pid, 0, 0);
}

/*
  This program (server) prints to stdout the PID process and keeps
  listening for incoming message transmissions
  
  Each client should use the following sequence:
         (int)              (char)               (char)
    length of message -> regular chars -> null string terminator char

  The string message is only printed to stdout when the full string is received
  For each signal received from client, this program (server) sends
  a corresponding signal back
  
  Using sigaction structure to define which signals to catch by the handler
  The infinite loop will just keep the process running
  
  The pause() function causes the calling thread to pause until a
  signal is received
*/
int	main(void)
{
	struct sigaction	s_server;

	sigemptyset(&s_server.sa_mask);
	s_server.sa_sigaction = server_handler;
	s_server.sa_flags = SA_SIGINFO | SA_RESTART;
	configure_sigaction_signals(&s_server);
	ft_putstr_fd("\e[92mserver [PID = ", STDOUT_FILENO);
	ft_putnbr_fd(getpid(), STDOUT_FILENO);
	ft_putstr_fd("]\n\e[0m", STDOUT_FILENO);
	while (1)
	{
		pause();
	}
	return (EXIT_SUCCESS);
}
