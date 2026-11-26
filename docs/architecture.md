# Arquitetura do Sistema / System Architecture — Compression Manager

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details open>
<summary>Clique para expandir / recolher a arquitetura em português</summary><br>

# Compression Manager — Arquitetura de Sistema e Especificações Técnicas

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  
**Padrão:** C++23 | Qt 6.12.0 (`Core`, `Gui`, `Widgets`) | CMake `3.28+` | MSVC puro (VS 18 2026 MSBuild) & Ninja (MinGW)  
**Plataforma Alvo:** Windows 11 / Windows 10 (`17763+`) x64  
**Autor:** Fernando Nillsson Cidade  

---

## 1. Filosofia Arquitetural e Visão Geral em Camadas

O **Compression Manager** foi estruturado em camadas modulares C++23 sobre o **Qt 6.12.0 Widgets**, separando inicialização de plataforma e estilo visual (`src_01_InicializadorMain`), orquestração de janela principal e layouts responsivos (`InterfaceCore`), gerenciadores operacionais e diálogos (`GerenciamentoUI`), abstrações assíncronas dos motores externos (`MotoresCompressao`), internacionalização dinâmica (`language`) e utilitários de sistema (`utils`):

```text
+-----------------------------------------------------------------------------+
|                    CAMADA DE INICIALIZAÇÃO E PLATAFORMA                     |
|         main.cpp -> src_01_InicializadorMain (iniciar_aplicacao)            |
|  qt.conf autônomo | LibraryPaths locais | Estilo windows11/windowsvista     |
|  LogManager (rotação por execução) | DragDrop UIPIPrivilege Diagnostics     |
+-----------------------------------------------------------------------------+
                                      |
                                      v
+-----------------------------------------------------------------------------+
|                  INTERFACE CORE & LAYOUTS (InterfaceCore)                   |
|   InterfaceGrafica (QMainWindow, MetodoCompressao, Dual-Layout Orchestrator)|
|   ginter_01..27: Menu Bar | QScrollArea métodos | Layout 1 & Segundo Layout |
|   adapt_layout_orientation (QHBoxLayout paisagem / QVBoxLayout retrato)     |
+-----------------------------------------------------------------------------+
         |                                                         |
         v                                                         v
+-----------------------------------------+     +-----------------------------+
|     GERENCIAMENTO UI (GerenciamentoUI)  |     |  IDIOMAS (source/language)  |
|  GerenciadorInterface (2 instâncias):   |     |  GerenciadorTraducao        |
|   - Filas QQueue por formato            |     |   - Carga dinâmica .qm      |
|   - Seleção de arquivos/destinos        |     |   - Persiste language.json  |
|  LayoutsCompressao (largura unificada)  |     |  CompileTranslations (CLI/  |
|  DragDropListWidget (+ WinNativeDrop    |     |   GUI em runtime/mocks)     |
|   EventFilter UIPI WM_DROPFILES HiDPI)  |     +-----------------------------+
|  ManualDialog | SobreDialog | OpcoesSobre|
+-----------------------------------------+
         |
         v
+-----------------------------------------------------------------------------+
|            MOTORES DE COMPRESSÃO & PERSISTÊNCIA (MotoresCompressao)         |
|  MetodoCompressao -> config.json (%LOCALAPPDATA%\CompressionManager\)       |
|  Threads QThread -> QProcess:                                               |
|   - CompressaoRAR   (#if ENABLE_RAR_SUPPORT=1 -> WinRAR.exe)                |
|   - CompressaoZIP / 7Z / TAR / WIM -> 7zG.exe / 7z.exe                      |
|   - TesteIntegridade (`7zG`/`7z t`) | Extracao (`x -y`, 2 fases bz2/tar.xz) |
+-----------------------------------------------------------------------------+
```

---

## 2. Segregação em Tempo de Compilação: Edição Full vs. Edição Standart

O controle entre a **Edição Full** e a **Edição Standart** ocorre no pré-processador C++ e no pipeline CMake pela opção `ENABLE_RAR_SUPPORT` (`CMakeLists.txt:18-24`), definindo globalmente `ENABLE_RAR_SUPPORT=1` ou `ENABLE_RAR_SUPPORT=0`:

- **Edição Full (`ENABLE_RAR_SUPPORT=ON` / `1`):**
  - Habilita o bloco de menu, checkbox e painel `.RAR` (`ginter_01_init_menu.cpp`, `ui_01_layoutsCompressao.cpp`, `mtcomp_01_metodosCompressao.cpp`), instanciando `CompressaoRAR` e pesquisando `WinRAR.exe` via `buscar_winrar_executavel()`;
  - Copia exclusivamente `WinRAR.exe` e `License.txt` para `<build>/plugins/WinRAR/` e inclui `winrar3.png` em `<build>/assets/icones/`.
- **Edição Standart (`ENABLE_RAR_SUPPORT=OFF` / `0`):**
  - `buscar_winrar_executavel()` retorna `QString()` imediatamente em tempo de compilação (`#if !ENABLE_RAR_SUPPORT`);
  - Oculta ações de menu RAR, painel `.RAR`, ícone `winrar3.png` e adapta os textos de `ManualDialog` (`ui_07_Manual.cpp`), `SobreDialog` (`ginter_20_exibir_sobre.cpp`) e `ReleaseNotes` (`ui_08`/`ui_09`) para exibir exclusivamente **ZIP**, **7Z**, **TAR** e **WIM**;
  - Tanto `copy_lean_compression_engines.cmake` quanto `copy_lean_app_assets.cmake` e `gerar_distribuicoes_limpas.ps1` asseguram zero artefatos do WinRAR no pacote final.

---

## 3. Anatomia da Interface, Layout Duplo e Alinhamento Responsivo

### 3.1. Dois Conjuntos Independentes de Estado (`GerenciadorInterface`)
Na `InterfaceGrafica` (`intercore_01_InterfaceGrafica.hpp`), cada coluna de trabalho possui sua própria instância desacoplada de `GerenciadorInterface` e `LayoutsCompressao`:
- **Coluna 1 (Padrão):** `gerenciador_interface` e `layouts_compressao`, controlando sua própria lista de entrada (`folder_listbox`), caixas de destino por formato (`output_listbox_{rar,zip,7z,tar,wim,extracao}`), filas de tarefas e instâncias de botões/rótulos.
- **Coluna 2 (Segundo Layout):** `gerenciador_interface_2` e `layouts_compressao_2`, construída dinamicamente em `ginter_toggle_segundo_layout()` (`ginter_21_toggle_segundo_layout.cpp`) dentro de `columns_widget` permitindo configurar e acompanhar fluxos paralelos (por exemplo, ZIP de um lado e 7Z/RAR do outro).

### 3.2. Preservação de Janela Maximizada e Adaptação de Orientação
1. **`ginter_toggle_segundo_layout()`:** Quando o usuário ativa o **Segundo Layout**, caso a janela não esteja maximizada (`!self->isMaximized()`) e tenha largura inferior a `1200 px`, redimensiona para `(1200, max(height, 750))`. Se já estiver maximizada, **preserva integralmente `isMaximized()`** sem restaurar a janela nem perder listas configuradas. Ao desativar, limpa com segurança `current_layouts_2`, `m_left_widgets` e o ponteiro `segundo_layout_widget`.
2. **`ginter_adapt_layout_orientation()`:** Avalia `win_h > 1.5 * win_w` com Segundo Layout ativo: em proporção retrato alterna o `columns_widget` para `QVBoxLayout` (empilhado); em paisagem/maximizado mantém `QHBoxLayout` (lado a lado), evitando recriações redundantes quando o tipo atual de layout já corresponde à orientação calculada.
3. **Equalização Geométrica (`LayoutsCompressao`):** Calcula `m_max_left_width` sobre os rótulos (`Especificar Saída`, `Armazenar`, `Extrair Arquivos`, `Limpar Saída`) e botões adjacentes em cada painel de formato ativo, equalizando a coluna esquerda entre todos os formatos visíveis para evitar cortes de texto após trocas dinâmicas de idioma.

---

## 4. Motores Externos, Filas Assíncronas e Drag & Drop UIPI

### 4.1. Resolução de Binários e Execução via `QThread` + `QProcess`
- **Descoberta de Motores (`mtcomp_02_motoresCompressao.cpp`):** `buscar_sevenzip_executavel()` e `buscar_winrar_executavel()` verificam primeiro os caminhos padrão instalados no sistema (`C:\Program Files\7-Zip\`, `C:\Program Files (x86)\7-Zip\`, `WinRAR\`) e, como fallback portátil, os diretórios embarcados `plugins/7Z/` (`7zG.exe` / `7z.exe`), `plugins/7-Zip/` e `plugins/WinRAR/` ao lado do executável ou em `C:\Program Files\Compression Manager\plugins\`.
- **Subclasses de Trabalho em Thread (`CompressaoBase`):** `CompressaoRAR`, `CompressaoZIP`, `Compressao7Z`, `CompressaoTAR`, `CompressaoWIM`, `TesteIntegridade` e `Extracao` herdam de `QThread` e executam comandos externos via `QProcess`:
  - RAR (Edição Full): `"<exe>" <a|u> -r [-s] -ep1 -m<0..5> -afrar "<dest.rar>" "<origem>"`
  - ZIP: `"<exe>" <a|u> -r -tzip -mx=<0|1|3|5|7|9> "<dest.zip>" "<origem>"`
  - 7Z: `"<exe>" <a|u> -r -t7z -mx=<0|1|3|5|7|9> "<dest.7z>" "<origem>"`
  - TAR / WIM: `"<exe>" <a|u> -r -t<tar|wim> "<dest>" "<origem>"`
  - Teste de Integridade: `"<7z_exe>" t "<arquivo>"` (para `.rar`, `.zip`, `.7z`, `.bz2`, `.tar`, `.wim`, `.tar.xz`, `.zipx`, `.tgz`, `.tar.gz`, `.lzh`, `.iso`)
  - Extração: `"<7z_exe>" x -y "<arquivo>" -o"<destino>"`, com etapa intermediária automática em `QTemporaryDir` para formatos compostos de dupla passagem (`.bz2`, `.tar.xz`, `.tgz`, `.tar.gz`).
- **Controle de Concorrência por Formato (`GerenciadorInterface::start_compression_thread`):** Monitora `compress_threads` ativos por `format`; novos pedidos para um formato já em execução são enfileirados em `QQueue<CompressaoBase*> compress_queue` (notificando o usuário) e disparados sequencialmente em `on_compress_finished()`.

### 4.2. Drag & Drop com Bypass Controlado de UIPI (`ui_03_dragDrop.cpp`)
No Windows, se o aplicativo for iniciado com privilégios elevados (`High`), o isolamento **UIPI (User Interface Privilege Isolation)** bloqueia arrastar-e-soltar OLE oriundo do Windows Explorer (nível `Medium`). O módulo `DragDropListWidget` resolve isso de forma determinística:
1. Diagnostica elevação e nível de integridade via `OpenProcessToken` (`TokenElevation` / `TokenIntegrityLevel`).
2. Quando elevado (`processo_esta_elevado()`), instala `WinNativeDropEventFilter` (`QAbstractNativeEventFilter` para `WM_DROPFILES`), revoga o `IDropTarget` OLE via `RevokeDragDrop(hwnd)`, libera mensagens `WM_DROPFILES`, `WM_COPYDATA` e `WM_COPYGLOBALDATA` via `ChangeWindowMessageFilterEx(..., MSGFLT_ALLOW)` e ativa `DragAcceptFiles(hwnd, TRUE)`.
3. Converte coordenadas físicas Win32 (`DragQueryPoint` -> `ScreenToClient`) para coordenadas lógicas Qt dividindo pelo `devicePixelRatioF()` para identificar exatamente qual `DragDropListWidget` (entrada vs. pasta de saída) recebeu os arquivos.

---

## 5. Persistência Local, Internacionalização e Cadeia de Build/Deploy Ultralimpo

- **Persistência em `%LOCALAPPDATA%\CompressionManager\` (`CaminhoPersistenteUtils.cpp`):**
  - `config.json`: níveis de compressão escolhidos por formato (`compress_type_{rar,zip,7z,tar,wim}`);
  - `language.json`: idioma ativo (`"idioma": "pt_BR"` ou `"en_US"`);
  - `logs\file_compression_yyyyMMdd_hhmmss.log`: arquivo de log por sessão gerado por `LogManager` com handler Qt (`qInstallMessageHandler`).
- **Internacionalização Dinâmica (`GerenciadorTraducao`):** Carrega catálogos binários `manager_compression_{pt_BR,en_US}.qm` de `translations/` e dispara `retranslateUi()` + reconstrução de painéis/menus e diálogos abertos em tempo real.
- **Pipeline Enxuto CMake & Distribuição (`cmake/*.cmake`, `cmake/gerar_distribuicoes_limpas.ps1`):**
  - Compilação em **C++23** estrito com **MSVC puro** (`Visual Studio 18 2026` / MSBuild `14.52.37025`, `/MD`, implantação CRT *app-local* das 5 DLLs `Microsoft.VC145.CRT` sem `vc_redist.x64.exe`) e **MinGW-w64 15.1.0** (`Ninja`, `-s`, sobrescrita explícita das 3 DLLs GCC `15.1.0`);
  - Poda rigorosa de módulos Qt não ligados (`Qt6Network`, `Qt6Pdf`, `Qt6Svg`, `Qt6PrintSupport`) e plugins supérfluos (`prune_optional_root_runtime_dlls.cmake`, `deploy_optional_qt_binary.cmake`), garantindo apenas `Qt6Core.dll`, `Qt6Gui.dll`, `Qt6Widgets.dll` + `qwindows.dll`, `qmodernwindowsstyle.dll` e `qico.dll`.
- **Suíte Isolada de Testes de Engenharia (`tests/`, `build/build_suite_{msvc,mingw}`):**
  - 10 executáveis `Qt6::Test` integrados ao **CTest** (`test_01`–`test_10`) que validam carregamento de plugins Qt, persistência, internacionalização, ativos legais, UIPI Drag & Drop, empacotamento/extração E2E com verificação SHA-256, segregação Full/Standart e higiene do repositório/instaladores, com deploy local isolado e sem geração de distribuição externa.

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details open>
<summary>Click to expand / collapse architecture in English</summary><br>

# Compression Manager — System Architecture & Technical Specifications

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  
**Standard:** C++23 | Qt 6.12.0 (`Core`, `Gui`, `Widgets`) | CMake `3.28+` | Pure MSVC (VS 18 2026 MSBuild) & Ninja (MinGW)  
**Target Platform:** Windows 11 / Windows 10 (`17763+`) x64  
**Author:** Fernando Nillsson Cidade  

---

## 1. Architectural Philosophy & Layered Design

**Compression Manager** organizes its C++23 codebase over **Qt 6.12.0 Widgets** into clearly bounded subsystems separating platform initialization (`src_01_InicializadorMain`), main window layout management (`InterfaceCore`), UI widgets/dialogs/drag-and-drop (`GerenciamentoUI`), asynchronous compression engine wrappers (`MotoresCompressao`), runtime translation (`language`), and system utilities (`utils`):

```text
+-----------------------------------------------------------------------------+
|                    PLATFORM INITIALIZATION & BOOTSTRAP                      |
|         main.cpp -> src_01_InicializadorMain (iniciar_aplicacao)            |
|  Self-contained qt.conf | Local LibraryPaths | windows11/windowsvista style |
|  LogManager (per-session file) | DragDrop UIPI Privilege Diagnostics        |
+-----------------------------------------------------------------------------+
                                      |
                                      v
+-----------------------------------------------------------------------------+
|                  INTERFACE CORE & LAYOUTS (InterfaceCore)                   |
|   InterfaceGrafica (QMainWindow, MetodoCompressao, Dual-Layout Orchestrator)|
|   ginter_01..27: Menu Bar | QScrollArea panels | Layout 1 & Second Layout   |
|   adapt_layout_orientation (QHBoxLayout landscape / QVBoxLayout portrait)   |
+-----------------------------------------------------------------------------+
         |                                                         |
         v                                                         v
+-----------------------------------------+     +-----------------------------+
|     UI MANAGEMENT (GerenciamentoUI)     |     |  LOCALIZATION (language)    |
|  GerenciadorInterface (2 instances):    |     |  GerenciadorTraducao        |
|   - Per-format QQueue serialization     |     |   - Dynamic .qm switching   |
|   - Native file/folder pickers          |     |   - Persists language.json  |
|  LayoutsCompressao (unified column size)|     |  CompileTranslations (CLI/  |
|  DragDropListWidget (+ WinNativeDrop    |     |   GUI utilities in mocks/)  |
|   EventFilter UIPI WM_DROPFILES HiDPI)  |     +-----------------------------+
|  ManualDialog | SobreDialog | OpcoesSobre|
+-----------------------------------------+
         |
         v
+-----------------------------------------------------------------------------+
|            COMPRESSION ENGINES & PERSISTENCE (MotoresCompressao)            |
|  MetodoCompressao -> config.json (%LOCALAPPDATA%\CompressionManager\)       |
|  QThread workers -> QProcess:                                               |
|   - CompressaoRAR   (#if ENABLE_RAR_SUPPORT=1 -> WinRAR.exe)                |
|   - CompressaoZIP / 7Z / TAR / WIM -> 7zG.exe / 7z.exe                      |
|   - TesteIntegridade (`7zG`/`7z t`) | Extracao (`x -y`, 2-pass bz2/tar.xz)  |
+-----------------------------------------------------------------------------+
```

---

## 2. Compile-Time Edition Segregation: Full vs. Standart

Separation between the **Full Edition** and **Standart Edition** is enforced at both CMake configuration time and C++ compile time via `ENABLE_RAR_SUPPORT` (`CMakeLists.txt:18-24`), defining `ENABLE_RAR_SUPPORT=1` or `ENABLE_RAR_SUPPORT=0`:

- **Full Edition (`ENABLE_RAR_SUPPORT=ON` / `1`):**
  - Compiles in the `.RAR` menu actions, format checkbox, and panel (`ginter_01_init_menu.cpp`, `ui_01_layoutsCompressao.cpp`, `mtcomp_01_metodosCompressao.cpp`), instantiating `CompressaoRAR` and locating `WinRAR.exe` via `buscar_winrar_executavel()`;
  - Deploys strictly `WinRAR.exe` and `License.txt` into `<build>/plugins/WinRAR/` and includes `winrar3.png` under `<build>/assets/icones/`.
- **Standart Edition (`ENABLE_RAR_SUPPORT=OFF` / `0`):**
  - `buscar_winrar_executavel()` returns `QString()` immediately (`#if !ENABLE_RAR_SUPPORT`);
  - Omits RAR menus, RAR panel, `winrar3.png` icon, and dynamically switches `ManualDialog` (`ui_07_Manual.cpp`), `SobreDialog` (`ginter_20_exibir_sobre.cpp`), and release notes (`ui_08`/`ui_09`) to cover open formats only (**ZIP**, **7Z**, **TAR**, and **WIM**);
  - `copy_lean_compression_engines.cmake`, `copy_lean_app_assets.cmake`, and `gerar_distribuicoes_limpas.ps1` guarantee zero WinRAR files or references remain in Standart bundles.

---

## 3. UI Architecture, Dual Layout & Responsive Orientation

### 3.1. Two Independent Workspace State Instances (`GerenciadorInterface`)
Inside `InterfaceGrafica` (`intercore_01_InterfaceGrafica.hpp`), each workspace column holds an independent instance of `GerenciadorInterface` and `LayoutsCompressao`:
- **Column 1 (Standard):** `gerenciador_interface` and `layouts_compressao`, maintaining dedicated input lists (`folder_listbox`), per-format output lists (`output_listbox_{rar,zip,7z,tar,wim,extracao}`), task queues, and method panels.
- **Column 2 (Second Layout):** `gerenciador_interface_2` and `layouts_compressao_2`, mounted dynamically by `ginter_toggle_segundo_layout()` (`ginter_21_toggle_segundo_layout.cpp`) inside `columns_widget` so users can configure and run side-by-side workflows (e.g., archiving to ZIP and 7Z/RAR concurrently).

### 3.2. Maximized Window Preservation & Dynamic Reorientation
1. **`ginter_toggle_segundo_layout()`:** When enabling the **Second Layout**, non-maximized windows narrower than `1200 px` expand smoothly to `(1200, max(height, 750))`; already maximized windows **strictly preserve `isMaximized()`** without exiting full screen or resetting queued paths. Disabling tears down `current_layouts_2`, `m_left_widgets`, and `segundo_layout_widget` cleanly.
2. **`ginter_adapt_layout_orientation()`:** Evaluates `win_h > 1.5 * win_w` when Dual Layout is active, switching `columns_widget` between `QVBoxLayout` (portrait stacked) and `QHBoxLayout` (landscape/maximized side-by-side) without redundant layout rebuilds.
3. **Uniform Column Sizing (`LayoutsCompressao`):** Tracks `m_max_left_width` across active method rows (`Specify Output`, `Store`, `Extract Files`, `Clear Output`) to keep action buttons vertically aligned across all enabled formats even after runtime language switches.

---

## 4. External Compression Engines, Async Queuing & UIPI Drag & Drop

### 4.1. Binary Discovery & `QThread` + `QProcess` Execution
- **Engine Lookup (`mtcomp_02_motoresCompressao.cpp`):** `buscar_sevenzip_executavel()` and `buscar_winrar_executavel()` inspect standard system installation paths first (`C:\Program Files\7-Zip\`, `WinRAR\`) and fall back to bundled `plugins/7Z/` (`7zG.exe` / `7z.exe`), `plugins/7-Zip/`, and `plugins/WinRAR/` directories alongside the binary or under `C:\Program Files\Compression Manager\plugins\`.
- **Background Worker Subclasses (`CompressaoBase`):** `CompressaoRAR`, `CompressaoZIP`, `Compressao7Z`, `CompressaoTAR`, `CompressaoWIM`, `TesteIntegridade`, and `Extracao` extend `QThread` and execute local engine commands via `QProcess`:
  - RAR (Full Edition): `"<exe>" <a|u> -r [-s] -ep1 -m<0..5> -afrar "<dest.rar>" "<source>"`
  - ZIP: `"<exe>" <a|u> -r -tzip -mx=<0|1|3|5|7|9> "<dest.zip>" "<source>"`
  - 7Z: `"<exe>" <a|u> -r -t7z -mx=<0|1|3|5|7|9> "<dest.7z>" "<source>"`
  - TAR / WIM: `"<exe>" <a|u> -r -t<tar|wim> "<dest>" "<source>"`
  - Integrity Verification: `"<7z_exe>" t "<archive>"` (supporting `.rar`, `.zip`, `.7z`, `.bz2`, `.tar`, `.wim`, `.tar.xz`, `.zipx`, `.tgz`, `.tar.gz`, `.lzh`, `.iso`)
  - Extraction: `"<7z_exe>" x -y "<archive>" -o"<dest>"`, with automatic two-stage temporary decompression (`QTemporaryDir`) for double-wrapped `.bz2`, `.tar.xz`, `.tgz`, and `.tar.gz` archives.
- **Per-Format Serialization Queue (`GerenciadorInterface::start_compression_thread`):** Checks active workers in `compress_threads` by `format`; overlapping requests for the same format enqueue cleanly in `QQueue<CompressaoBase*> compress_queue` and dequeue sequentially on `on_compress_finished()`.

### 4.2. Controlled UIPI Drag & Drop Bridge (`ui_03_dragDrop.cpp`)
When launched with elevated (`High`) privileges on Windows, UIPI blocks standard OLE drag-and-drop from Windows Explorer (`Medium` integrity). `DragDropListWidget` resolves this cleanly:
1. Audits token elevation and integrity level via `OpenProcessToken` (`TokenElevation` / `TokenIntegrityLevel`).
2. When elevated (`processo_esta_elevado()`), registers `WinNativeDropEventFilter` (`QAbstractNativeEventFilter` listening for `WM_DROPFILES`), revokes OLE drop targets (`RevokeDragDrop`), allows `WM_DROPFILES` / `WM_COPYDATA` / `WM_COPYGLOBALDATA` via `ChangeWindowMessageFilterEx(..., MSGFLT_ALLOW)`, and enables `DragAcceptFiles(hwnd, TRUE)`.
3. Converts physical Win32 drop coordinates (`DragQueryPoint` -> `ScreenToClient`) into logical Qt coordinates scaled by `devicePixelRatioF()` to target the exact input or output `DragDropListWidget`.

---

## 5. Local Persistence, Localization & Ultra-Lean Build/Deploy Pipeline

- **Persistence under `%LOCALAPPDATA%\CompressionManager\` (`CaminhoPersistenteUtils.cpp`):**
  - `config.json`: persisted compression level per format (`compress_type_{rar,zip,7z,tar,wim}`);
  - `language.json`: persisted active UI locale (`"idioma": "pt_BR"` or `"en_US"`);
  - `logs\file_compression_yyyyMMdd_hhmmss.log`: session execution log managed by `LogManager` via `qInstallMessageHandler`.
- **Dynamic Runtime Localization (`GerenciadorTraducao`):** Loads compiled `.qm` catalogs (`manager_compression_{pt_BR,en_US}.qm`) from `translations/` and updates widgets, dynamically aligned method rows, and open dialogs without restart.
- **Lean CMake Build & Distribution Pipeline (`cmake/*.cmake`, `cmake/gerar_distribuicoes_limpas.ps1`):**
  - Compiles strictly under **C++23** with **pure MSVC** (`Visual Studio 18 2026` / MSBuild `14.52.37025`, `/MD`, deploying 5 app-local `Microsoft.VC145.CRT` DLLs without `vc_redist.x64.exe`) and **MinGW-w64 15.1.0** (`Ninja`, `-s`, explicitly deploying GCC `15.1.0` runtime DLLs);
  - Prunes unused Qt modules (`Qt6Network`, `Qt6Pdf`, `Qt6Svg`, `Qt6PrintSupport`) and superfluous plugins (`prune_optional_root_runtime_dlls.cmake`, `deploy_optional_qt_binary.cmake`), retaining solely `Qt6Core.dll`, `Qt6Gui.dll`, `Qt6Widgets.dll` plus `qwindows.dll`, `qmodernwindowsstyle.dll`, and `qico.dll`.
- **Isolated Engineering Test Suite (`tests/`, `build/build_suite_{msvc,mingw}`):**
  - 10 `Qt6::Test` executables integrated into **CTest** (`test_01`–`test_10`) validating Qt plugin loading, local persistence, i18n translations, legal assets, UIPI Drag & Drop, E2E archiving/extraction with SHA-256 verification, Full/Standart segregation, and repo/installer hygiene inside isolated build folders with zero external distribution.

</details>
