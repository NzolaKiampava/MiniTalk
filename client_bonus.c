/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkiampav <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:32:29 by nkiampav          #+#    #+#             */
/*   Updated: 2024/08/05 09:32:33 by nkiampav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk_bonus.h"

/*
  Funcao manipula sinais SIGUSR1/SIGUSR2 recebido apartir do servidor
  
  A ser observado que o server so envia um sinal apos receber 
  um sinal do cliente, conforme explicado abaixo:
  - Client envia um bit para o server e,(normalm.) aguarde uma resposta - 
  usando pause()
  - O seridor entao envia um sinal para confirmar que cada bit foi 
  recebida (ACK)
  ou
  - O servidor envia um sinal para confirmar q o terminador NULL foi recebido
  (fim da mensafem de string), e em seguida a funcao é encerrada
*/
void	client_handler(int sig)
{
	if (sig == SIGUSR1)
		ft_putstr_fd("\e[34m > ACK Sinal recebido pelo servidor\n\e[0m",
			STDOUT_FILENO);
	else if (sig == SIGUSR2)
	{
		ft_putstr_fd("\e[92m > Fim da mensagem recebida pelo servidor\n\e[0m",
			STDOUT_FILENO);
		exit(EXIT_SUCCESS);
	}
}

/*
  Funcao envia o comprimento da string para o server, e em seguida, envia
  a propria string, incluindo o terminador nulo

  Nota: no caso especifico de uma string nula, sera enviada zero como
  comprimento e, em seguida, uma string com o terminador nulo
*/
void	client_send_message(int server_pid, char *str)
{
	int	i;

	i = 0;
	ft_putstr_fd("\e[36mTamanho da Mensagem = [", STDOUT_FILENO);
	ft_putnbr_fd(ft_strlen(str), STDOUT_FILENO);
	ft_putstr_fd("]\n\e[0m", STDOUT_FILENO);
	send_int(server_pid, ft_strlen(str));
	ft_putstr_fd("\e[36mEnviando mensagem\n\e[0m", STDOUT_FILENO);
	while (str[i] != '\0')
		send_char(server_pid, str[i++]);
	ft_putstr_fd("\e[36mEnviando terminador nulo\n\e[0m", STDOUT_FILENO);
	send_char(server_pid, '\0');
}

/*
  Neste prog. (client) envia uma string message para outro processo(server)

  Os argumentos passados pela linha de comando sao verificados para garantir
  se sao validos:
  - string and PID server arguments must be passed, and not more or less
  - PID server must be valid, kill() and ft_atoi() are used to check this
  (not only it must be a valid number as it must be an existing process)
  
  A estrutura sigaction é usada para capturar os sinais definidos pelo user
  e cuidar da comunicacao com o server. Basicamente, para cada sinal enviado
  pelo client, espera-se que um sinal de resposta seja reebido do servidor
*/
int	main(int argc, char **argv)
{	
	struct sigaction	s_client;

	if (argc != 3)
	{
		ft_putstr_fd("\e[31m## error - Sintaxe Errado ##\n\e[0m", STDOUT_FILENO);
		ft_putstr_fd(
			"\e[92m./client <PID> <mensagem>\n\e[0m",
			STDOUT_FILENO);
		return (EXIT_FAILURE);
	}
	else if (kill(ft_atoi(argv[1]), 0) < 0)
	{
		ft_putstr_fd("\e[31m## error - PID Invalido ##\n\e[0m", STDOUT_FILENO);
		return (EXIT_FAILURE);
	}
	sigemptyset(&s_client.sa_mask);
	s_client.sa_flags = SA_RESTART;
	s_client.sa_handler = client_handler;
	configure_sigaction_signals(&s_client);
	ft_putstr_fd("\e[92mclient [PID = ", STDOUT_FILENO);
	ft_putnbr_fd(getpid(), STDOUT_FILENO);
	ft_putstr_fd("]\n\e[0m", STDOUT_FILENO);
	client_send_message(ft_atoi(argv[1]), argv[2]);
	return (EXIT_SUCCESS);
}
