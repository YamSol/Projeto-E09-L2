
# Definições do Arduino
BOARD=arduino:avr:uno
PORT=/dev/ttyUSB0 # Alterar para a porta correta
UPLOAD_PROTOCOL=arduino

# Diretórios
SRC_DIR=src/main
BUILD_DIR=./build
ARDUINO_CLI=arduino-cli

# Flags de compilação
FLAGS=-v -I$(SRC_DIR)

# Arquivo do sketch
SKETCH=$(SRC_DIR)/main.ino

# Compilar
build:
	$(ARDUINO_CLI) compile --fqbn $(BOARD) --build-path $(BUILD_DIR) $(SKETCH)

# Fazer upload para o dispositivo
upload:
	$(ARDUINO_CLI) upload -p $(PORT) --fqbn $(BOARD) --upload-speed 115200 --port $(PORT) --build-path $(BUILD_DIR) $(SKETCH)

# Limpeza dos arquivos de build
clean:
	rm -rf $(BUILD_DIR)
