# Sugestões de Issues e Roadmap / Suggested Issues & Roadmap — Compression Manager

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details open>
<summary>Clique para expandir / recolher sugestões em português</summary><br>

# Compression Manager — Backlog Técnico e Sugestões para Contribuidores

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  

Este documento reúne melhorias técnicas, endurecimentos de segurança e extensões funcionais mapeadas diretamente a partir da análise real do código-fonte de **Compression Manager (`v2026.11.22.0`)**, orientando colaboradores e forks interessados em evoluir o projeto.

---

### Issue 1: Integração CI Contínua da Suíte `tests/` e Testes de Estresse/Fuzzing
- **Área:** `testes` / `ci`
- **Prioridade:** Alta
- **Contexto Atual no Código:** A suíte automatizada principal em `tests/` (`test_01` a `test_10`, `Qt6::Test` + `CTest`) já está implementada e aprovada (`10/10`) localmente em MSVC puro e MinGW; permanece pendente a execução automática em esteira CI remota (GitHub Actions) além da remoção ou aproveitamento do alvo legado vazio `teste.cpp` (`CMakeLists.txt:178`).
- **Descrição:** Configurar pipeline CI no `.github/workflows/` para disparar `suite-msvc-release` e `suite-mingw-release` a cada Pull Request, adicionar testes de estresse/fuzzing para caminhos longos Unicode e pacotes corrompidos de grande porte, e aposentar o stub legado `teste.cpp`.
- **Benefício:** Validação automatizada contínua na nuvem antes de empacotamentos MSVC/MinGW e releases.

---

### Issue 2: Endurecimento da Invocação de Subprocessos (`QProcess::start` com Lista de Argumentos) e Códigos de Retorno
- **Área:** `MotoresCompressao` / `seguranca`
- **Prioridade:** Alta
- **Contexto Atual no Código:** Em `source/MotoresCompressao/mtcomp_02_motoresCompressao.cpp`, as classes `CompressaoRAR`, `CompressaoZIP`, `Compressao7Z`, `CompressaoTAR`, `CompressaoWIM`, `TesteIntegridade` e `Extracao` invocam `process.startCommand(cmd)` com interpolação de strings entre aspas, e os resultados de conclusão (`on_teste_integridade_finished`, `on_extract_finished`, `on_compress_finished`) em `ui_02_gerenteGUILayouts.cpp` exibem sempre mensagem de sucesso sem propagar `exitCode()` negativo para a interface.
- **Descrição:** Substituir `QProcess::startCommand(cmd)` por `QProcess::start(executable, QStringList{...})` com parâmetros separados, extrair listas de origem/destino na thread da GUI antes de iniciar a `QThread` (evitando leitura direta de `QListWidget` em thread trabalhadora) e propagar código de saída / stderr dos motores para um relatório claro de sucesso/falha ao usuário.
- **Benefício:** Elimina ambiguidades com caminhos contendo caracteres especiais/aspas, torna o acesso a widgets 100% *thread-safe* e garante diagnóstico preciso de falhas de compressão/integridade.

---

### Issue 3: Escrita Atômica (`QSaveFile`) para Configurações e Rotação Limitada de Logs (`LogManager`)
- **Área:** `utils` / `persistencia`
- **Prioridade:** Média
- **Contexto Atual no Código:** `LogManager::init()` (`source/utils/LogManager.cpp`) cria um arquivo `file_compression_yyyyMMdd_hhmmss.log` por execução sem política automática de exclusão de logs antigos, e `MetodoCompressao::save_compression_method()` / `GerenciadorTraducao::salvar_configuracao_idioma()` gravam diretamente com `QFile::WriteOnly`.
- **Descrição:** Adotar `QSaveFile` para gravação atômica de `config.json` e `language.json` (evitando truncamento em desligamento abrupto) e adicionar política de retenção automática (ex.: manter os últimos 10–20 arquivos de log em `%LOCALAPPDATA%\CompressionManager\logs\`).
- **Benefício:** Proteção contra corrupção acidental de preferências e controle automático de espaço em disco.

---

### Issue 4: Empacotamento MSIX Automatizado para Ambas as Edições e Assinatura/Hardening MSVC
- **Área:** `cmake` / `library` / `seguranca`
- **Prioridade:** Média
- **Descrição:** Criar script/target dedicado para gerar pacotes `.msix` separados ou parametrizados alinhados a `library/AppxManifest.xml` (sincronizando `Version="2026.11.22.0"` automaticamente via `find_VersionEditor` e adicionando suporte à edição **Standart** na Microsoft Store sem WinRAR), além de habilitar flags opcionais de hardening MSVC (`/GS /sdl /guard:cf /CETCOMPAT`) no `CMakeLists.txt`.
- **Benefício:** Reprodutibilidade integral do pipeline Microsoft Store e proteção binária adicional contra corrupção de memória.

---

### Issue 5: Suporte a Criptografia por Senha (AES-256), Progresso em Tempo Real e Cancelamento Cooperativo
- **Área:** `GerenciamentoUI` / `MotoresCompressao`
- **Prioridade:** Média
- **Descrição:** Adicionar opção opcional na interface para proteção por senha em arquivos `.7z` (AES-256 com `-mhe=on`), `.zip` (AES-256 via `-mem=AES256`) e `.rar` (Full, `-p`/`-hp`) — mantendo zero armazenamento de senhas em disco —, somado ao parsing da saída stdout do `7z.exe -bsp1` para barra de progresso percentual e botão de cancelamento limpo em tarefas longas.
- **Benefício:** Atendimento a fluxos de backup confidenciais mantendo arquitetura 100% offline e feedback visual preciso para arquivos grandes.

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details open>
<summary>Click to expand / collapse suggested issues in English</summary><br>

# Compression Manager — Technical Backlog & Contributor Suggestions

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  

This document tracks technical improvements, security hardenings, and feature extensions grounded directly in **Compression Manager (`v2026.11.22.0`)** source code inspection, guiding contributors and independent forks.

---

### Issue 1: Remote CI Execution for the `tests/` Suite & Stress/Fuzz Coverage
- **Area:** `tests` / `ci`
- **Priority:** High
- **Current Codebase Context:** The complete automated suite under `tests/` (`test_01`–`test_10`, `Qt6::Test` + `CTest`) is implemented and passing (`10/10`) locally on pure MSVC and MinGW; remaining work includes automated remote CI triggers in `.github/workflows/` and retiring or repurposing the legacy empty `teste.cpp` stub (`CMakeLists.txt:178`).
- **Description:** Wire `suite-msvc-release` and `suite-mingw-release` into GitHub Actions PR workflows, expand stress/fuzz tests for long Unicode paths and oversized corrupted archives, and remove the unused legacy `teste` stub.
- **Benefit:** Continuous automated cloud regression verification prior to MSVC/MinGW packaging and releases.

---

### Issue 2: Subprocess Hardening (`QProcess::start` Argument List) & Worker Error Propagation
- **Area:** `MotoresCompressao` / `security`
- **Priority:** High
- **Current Codebase Context:** In `source/MotoresCompressao/mtcomp_02_motoresCompressao.cpp`, worker classes (`CompressaoRAR`, `CompressaoZIP`, `Compressao7Z`, `CompressaoTAR`, `CompressaoWIM`, `TesteIntegridade`, `Extracao`) invoke `process.startCommand(cmd)` with string interpolation, and completion slots in `ui_02_gerenteGUILayouts.cpp` always display success messages regardless of non-zero subprocess exit codes.
- **Description:** Migrate `QProcess::startCommand(cmd)` to explicit `QProcess::start(executable, QStringList{...})` calls, capture input/output path lists on the UI thread prior to `QThread::start()` (eliminating cross-thread `QListWidget` reads), and surface engine `exitCode()` / stderr diagnostics to users cleanly.
- **Benefit:** Eliminates edge-case quoting ambiguities on special paths, ensures strict GUI thread safety, and accurately reports archive/integrity failures.

---

### Issue 3: Atomic Preference Writes (`QSaveFile`) & Bounded Log Retention (`LogManager`)
- **Area:** `utils` / `persistence`
- **Priority:** Medium
- **Current Codebase Context:** `LogManager::init()` (`source/utils/LogManager.cpp`) creates a timestamped `file_compression_yyyyMMdd_hhmmss.log` per run without pruning older logs, and `config.json`/`language.json` write directly via `QFile::WriteOnly`.
- **Description:** Switch `config.json` and `language.json` writes to atomic `QSaveFile` commits and add configurable log retention pruning (e.g., keeping the 10–20 most recent session logs inside `%LOCALAPPDATA%\CompressionManager\logs\`).
- **Benefit:** Prevents partial/truncated preference JSON files during sudden shutdown and keeps local log storage bounded.

---

### Issue 4: Automated Dual-Edition MSIX Packaging & MSVC Binary Hardening Flags
- **Area:** `cmake` / `library` / `security`
- **Priority:** Medium
- **Description:** Add automated packaging helpers aligned with `library/AppxManifest.xml` (synchronizing `Version="2026.11.22.0"` via `find_VersionEditor` and producing clean MSIX layouts for Standart and Full), plus optional MSVC compiler/linker hardening flags (`/GS /sdl /guard:cf /CETCOMPAT`) in `CMakeLists.txt`.
- **Benefit:** Full Store submission reproducibility and hardened release binaries.

---

### Issue 5: Optional Password Encryption (AES-256), Live Progress Bar & Cooperative Cancellation
- **Area:** `GerenciamentoUI` / `MotoresCompressao`
- **Priority:** Medium
- **Description:** Provide optional UI password inputs (never persisted on disk) for `.7z` AES-256 (`-mhe=on`), `.zip` AES-256 (`-mem=AES256`), and `.rar` (`-p`/`-hp` in Full Edition), paired with stdout progress parsing (`7z.exe -bsp1`) and clean job cancellation for multi-gigabyte archives.
- **Benefit:** Supports confidential offline backup workflows with responsive progress visibility.

</details>
