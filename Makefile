CC      = gcc
FLEX    = flex
BISON   = bison

LEXER_DIR  = lexer
PARSER_DIR = parser
BUILD_DIR  = build
TEST_DIR   = test

# Arquivos gerados
BISON_SRC  = $(PARSER_DIR)/portugol.tab.c
BISON_HDR  = $(PARSER_DIR)/portugol.tab.h
FLEX_SRC   = $(LEXER_DIR)/lex.yy.c

TARGET = $(BUILD_DIR)/portugol

# -------------------------------------------------------
# Alvo padrão
.PHONY: all clean test help

all: $(TARGET)

# -------------------------------------------------------

$(BISON_SRC) $(BISON_HDR): $(PARSER_DIR)/portugol.y
	$(BISON) -d -v -o $(BISON_SRC) $(PARSER_DIR)/portugol.y

# -------------------------------------------------------
# 2) Gerar lexer com Flex (depende do .h do Bison)
$(FLEX_SRC): $(LEXER_DIR)/portugol.l $(BISON_HDR)
	$(FLEX) -o $(FLEX_SRC) $(LEXER_DIR)/portugol.l

# -------------------------------------------------------
# 3) Compilar tudo com gcc
#    noyywrap esta no .l, entao nao precisamos de -lfl
# -------------------------------------------------------
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(FLEX_SRC) $(BISON_SRC) | $(BUILD_DIR)
	$(CC) -Wall -o $@ $(FLEX_SRC) $(BISON_SRC)

# -------------------------------------------------------
# 4) Executar todos os testes
#    tests normais (sem "err" no nome): devem ser ACEITOS
#    testes de erro (com "err" no nome): devem ser REJEITADOS
# -------------------------------------------------------
test: $(TARGET)
	@echo ""; echo "========================================"
	@echo " RODANDO TESTES"
	@echo "========================================"
	@pass=0; fail=0; \
	echo ""; echo "-- Testes validos (devem PASS) --"; \
	for f in $(TEST_DIR)/teste_[0-9]*.pt; do \
		printf "  %-40s " "$$f"; \
		if ./$(TARGET) $$f > /dev/null 2>&1; then \
			echo "[PASS]"; pass=$$((pass+1)); \
		else \
			echo "[FAIL] <- ERRO: deveria aceitar"; fail=$$((fail+1)); \
		fi; \
	done; \
	echo ""; echo "-- Testes de erro (devem FAIL) --"; \
	for f in $(TEST_DIR)/teste_err*.pt; do \
		printf "  %-40s " "$$f"; \
		if ./$(TARGET) $$f > /dev/null 2>&1; then \
			echo "[FAIL] <- ERRO: deveria rejeitar"; fail=$$((fail+1)); \
		else \
			echo "[PASS] (erro detectado corretamente)"; pass=$$((pass+1)); \
		fi; \
	done; \
	echo ""; echo "========================================"; \
	echo "  Resultado: $$pass PASS  |  $$fail FAIL"; \
	echo "========================================"

# -------------------------------------------------------
# 5) Rodar um arquivo especifico com saida visivel
run: $(TARGET)
	./$(TARGET) $(FILE)

# -------------------------------------------------------
# 6) Limpeza
# -------------------------------------------------------
clean:
	rm -f $(FLEX_SRC) $(BISON_SRC) $(BISON_HDR)
	rm -f $(PARSER_DIR)/portugol.output
	rm -rf $(BUILD_DIR)

# -------------------------------------------------------
# 7) Ajuda
# -------------------------------------------------------
help:
	@echo ""
	@echo "Alvos disponiveis:"
	@echo "  make          -> compila o compilador"
	@echo "  make test     -> roda todos os testes em test/"
	@echo "  make run FILE=test/exemplo.pt  -> roda um arquivo"
	@echo "  make clean    -> remove arquivos gerados"
	@echo ""
