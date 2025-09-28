import time                    # biblioteca para poder trabalhar com delays
from periphery import I2C      # biblioteca para comunicação com os periférios, nesse caso usado para comunicação i2c
from datetime import datetime  # módulo para trabalhar com data e hora

# configurações do sensor de luminosidade
BH1750_ADDRESS = 0x23
BH1750_POWER_ON = 0x01
BH1750_CONT_HIGH_RES_MODE = 0x10
I2C_BUS = "/dev/i2c-2"

# configurações do datalogger
INTERVALO_LEITURA = 10                                                      # Intervalo entre as leituras em segundos
CAMINHO_ARQUIVO = "/media/caninos/2584-1320/leituras_luminosidade.txt"      # Caminho completo para o arquivo de log no cartão SD

# função responsável por iniciar o sensor de luminosidade
def bh1750_init():
    i2c.transfer(BH1750_ADDRESS, [I2C.Message([BH1750_POWER_ON])])
    time.sleep(0.01)
    i2c.transfer(BH1750_ADDRESS, [I2C.Message([BH1750_CONT_HIGH_RES_MODE])])
    time.sleep(0.18)

# função que realiza a leitura do sensor e retorna o falor em lux
def bh1750_read():
    read_msg = I2C.Message([0, 0], read=True)
    i2c.transfer(BH1750_ADDRESS, [read_msg])
    data = read_msg.data
    
    raw_value = (data[0] << 8) | data[1]
    return raw_value / 1.2

# função que retorna o timestamp atual
def get_timestamp():
    return datetime.now().strftime("%Y-%m-%d %H:%M:%S")

print("Iniciando Datalogger de Luminosidade...")
print(f"As leituras serão salvas em: {CAMINHO_ARQUIVO}")
print(f"Intervalo entre leituras: {INTERVALO_LEITURA} segundos")
i2c = None

try:
    # abre o barramento I2C
    i2c = I2C(I2C_BUS)
    
    # iniciando o sensor bh1750
    print("Configurando o sensor BH1750...")
    bh1750_init()
    
    # abre o arquivo no modo 'append' ('a'), que adiciona novas linhas ao final
    # do arquivo sem apagar o conteúdo existente.
    with open(CAMINHO_ARQUIVO, "a") as arquivo_log:
        print("Iniciando leituras...")
        while True:
            # lê o valor atual que o sensor está captando
            lux = bh1750_read()
            
            # obtendo timestamp
            timestamp = get_timestamp()

            # preparando a linha a ser salva
            linha_para_salvar = f"{timestamp}; Luminosidade: {lux:.2f} lux\n"
            
            # --- Armazenamento e Feedback ---
            # armazenando a linha no arquivo especificado
            arquivo_log.write(linha_para_salvar)        # escreve a linha no arquivo
            arquivo_log.flush()                         # força a gravação imediata no disco (evita perda de dados)
            
            # exibe no terminal a informação que foi salva
            print(f"Salvo: {linha_para_salvar.strip()}")
            
            # aguarda o dalay definido
            time.sleep(INTERVALO_LEITURA)

except KeyboardInterrupt:
    print("\nPrograma encerrado pelo usuário.")
except Exception as e:
    print(f"Ocorreu um erro: {e}")
finally:
    if i2c:
        i2c.close()
    print("Recursos liberados.")