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
.PHONY: all clean test test-tac test-sem test-all help

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

$(TARGET): $(FLEX_SRC) $(BISON_SRC) src/ast.c src/simbolos.c src/semantica.c src/codigo_intermediario.c | $(BUILD_DIR)
	$(CC) -Wall -Isrc -o $@ $(FLEX_SRC) $(BISON_SRC) src/simbolos.c src/ast.c src/semantica.c src/codigo_intermediario.c

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
	for f in $(TEST_DIR)/valid/*.pt; do \
		printf "  %-40s " "$$f"; \
		if ./$(TARGET) $$f > /dev/null 2>&1; then \
			echo "[PASS]"; pass=$$((pass+1)); \
		else \
			echo "[FAIL] <- ERRO: deveria aceitar"; fail=$$((fail+1)); \
		fi; \
	done; \
	echo ""; echo "-- Testes de erro (devem FAIL) --"; \
	for f in $(TEST_DIR)/error/*.pt; do \
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
# Testes do codigo intermediario: compara a saida TAC com o arquivo .tac esperado
test-tac: $(TARGET)
	@pass=0; fail=0; echo ""; echo "-- Testes de codigo intermediario (TAC) --"; \
	for f in $(TEST_DIR)/tac/*.pt; do \
		printf "  %-40s " "$$f"; \
		if ./$(TARGET) $$f 2>/dev/null | sed -n '/Código de três/,/^$$/p' | sed '1d;$$d' | diff -q - $${f%.pt}.tac > /dev/null; then \
			echo "[PASS]"; pass=$$((pass+1)); \
		else echo "[FAIL] <- TAC diferente do esperado"; fail=$$((fail+1)); fi; \
	done; echo "  Resultado TAC: $$pass PASS | $$fail FAIL"; [ $$fail -eq 0 ]

# Testes semanticos: deve falhar (exit != 0) e emitir a mensagem do arquivo .msg
test-sem: $(TARGET)
	@pass=0; fail=0; echo ""; echo "-- Testes semanticos (erro + mensagem) --"; \
	for f in $(TEST_DIR)/semantica/*.pt; do \
		printf "  %-40s " "$$f"; \
		out=$$(./$(TARGET) $$f 2>&1 >/dev/null); rc=$$?; \
		if [ $$rc -ne 0 ] && echo "$$out" | grep -qF "$$(cat $${f%.pt}.msg)"; then \
			echo "[PASS]"; pass=$$((pass+1)); \
		else echo "[FAIL] <- mensagem esperada nao encontrada"; fail=$$((fail+1)); fi; \
	done; echo "  Resultado SEM: $$pass PASS | $$fail FAIL"; [ $$fail -eq 0 ]

test-all: test test-sem test-tac

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
