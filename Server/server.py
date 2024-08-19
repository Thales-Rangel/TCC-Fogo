# Importação da biblioteca socket
import socket

## Esta biblioteca fornece acesso à interface de rede de baixo nível.
## Permitindo a criação e manipulação de sockets.


# Definição de variáveis
localPort=8888 #Porta local que será usada pelo servidor UDP
bufferSize=1024 #Tamanho do buffer para a recepção de dados (1024 bytes)

### Buffer: É usado para armazenar dados recebidos pelo socket antes de serem processados


# Criação do socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

## *socket.AF_INET: Indica que o socket usará o protocolo IPv4
## *socket.SOCK_DGRAM: Indica que o socket usará o protocolo UDP (User Datagram Protocol)


enderecos = []


# Função init:
def init():
	sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1) #Permite que múltiplos sockets usem a mesma porta.
	sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1) #Permite o uso de mensagens de broadcast.
	
	### Broadcast: Mensagens enviadas de um dispositivo para todos de uma rede local;
	
	
	sock.bind(('', localPort)) #Associa o socket a porta local específicada (localPort).
                               #O endereço vazio '' indica que o socket aceitará conexões de qualquer IP.
	print(f"UDP server : {get_ip_address()}:{localPort}") #Imprime o endereço IP do servidor e a porta local.
	
	
# Função Main:
def main():
    while True:  # Loop infinito para manter o servidor em execução.
        data, addr = sock.recvfrom(bufferSize)  # Recebe dados de um cliente com o tamanho do buffer especificado.
                                                # O endereço do remetente é armazenado em addr
        print(f"received message:\n{data} from {addr} \n")  # Imprime a mensagem recebida e o endereço do remetente
        
        if data not in enderecos:
            enderecos.append(addr)
        
        if b' 0 ' in data or b'500' in data:
            for i in enderecos:
                sock.sendto(b'F', i)



# Função get_ip_address:
def get_ip_address():
	"""get host ip address"""
	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) #Cria um socket.
	s.connect(("8.8.8.8",80)) #Se conecta ao endereço IP 8.8.8.8 na porta 80 (DNS do Google).
	
	ip_address = s.getsockname()[0] #Obtém o endereço local do host.
	
	s.close() #Fecha o socket.
	
	return ip_address #Retorna o endereço IP.
	



# Bloco principal:
if __name__ == '__main__': #Verifica se o script está sendo usado diretamente (não importando como módulo).
	init() #Inicialização do servidor.
	main() #Loop de recepção e envio de mensagens.
