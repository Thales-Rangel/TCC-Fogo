# Importação da biblioteca socket
import socket
import mariadb

## socket: Esta biblioteca fornece acesso à interface de rede de baixo nível.
## 		   Permitindo a criação e manipulação de sockets.
## mariadb: Esta biblioteca permite a conexão e manipulação de dados com o SQL do mariadb

# Definição de variáveis
localPort=8888 #Porta local que será usada pelo servidor UDP
bufferSize=1024 #Tamanho do buffer para a recepção de dados (1024 bytes)

conexao = mariadb.connect( #Objeto de conexão ao banco de dados
    user="root",
    password="tcc-fogo",
    host="localhost",
    port=3306,
    database="tcc_fogo"
)

### Buffer: É usado para armazenar dados recebidos pelo socket antes de serem processados


# Criação do socket e do cusor
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

## *socket.AF_INET: Indica que o socket usará o protocolo IPv4
## *socket.SOCK_DGRAM: Indica que o socket usará o protocolo UDP (User Datagram Protocol)

sql = conexao.cursor() ## Objeto que executará os comandos SQL 


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
    sql.execute(f"select end_ip from modolos")

    enderecos = sql.fetchall()

    for line in enderecos:
        print(line)

    while True:  # Loop infinito para manter o servidor em execução.
        data, addr = sock.recvfrom(bufferSize)  # Recebe dados de um cliente com o tamanho do buffer especificado.
                                                # O endereço do remetente é armazenado em addr
        print(f"received message:\n{data} from {addr} \n")  # Imprime a mensagem recebida e o endereço do remetente

        if (addr[0],) not in enderecos:
            try:
                if b'MAC' in data:
                    sql.execute(f'insert into modolos values (?, null, null, null, null, ?)', (addr[0], data[4::]))
                else:
                    sql.execute(f'insert into modolos values (?, null, null, null, null, null)', (addr[0],))
            
                conexao.commit()
                print(f"Novo Sensor de fogo cadastrado ao banco com sucesso!")

                sql.execute('select end_ip from modolos')
                enderecos = sql.fetchall()

                print('Endereços: ')
                for line in enderecos:
                     print(line)
            except Exception as e:
                 print(f"Não foi possível cadastrar o novo Sensor:")
                 print(e)
        
        fogo = int(data[16:17])
        gas = int(data[32:data.find(b'A')])
        status = str(data[data.find(b'A')::])

        print(f'Fogo: {fogo}; Gás: {gas}; Estado: {status}')

        #if b'MAC:' not in data:
        #    try:
        #        sql.execute(f"insert into modolos values (?, null, null, null, null, null)", (addr))
        #        conexao.commit()
        #        print(f"Deu bom colocar o end_ip")
        #    except Exception as e:
        #         print(f"Deu erro em inserir dados man, ó o erro aqui: {e}")
        
        #    fogo = int(data[16:17])
        #    gas = int(data[32:data.find(b'A')])
        #    estatus = str(data[data.find(b'A')::])
        
        #    print(f'Fogo: {fogo}; Gás: {gas}; Estado: {estatus}')
        
        #    sock.sendto(b'V= 700', addr)
        
        #    if b'Alerta!' in data:
        #        for i in enderecos:
        #            sock.sendto(b'F', i)

        #    try:
        #        sql.execute(f"insert into registros values (default, '22-Ago-2024 12:00h', ?, ?, '26°', ?)", (fogo, gas, estatus))
        #        conexao.commit()
        #        print("Dados guardados com sucesso")
        #    except Exception as e:
        #        print(f"Erro na incerção de dados: {e}")
        #else:
        #    sock.sendto(b'Vtmnc', addr)


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
