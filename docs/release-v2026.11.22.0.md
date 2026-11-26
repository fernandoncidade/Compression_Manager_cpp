# Release Notes — Compression Manager v2026.11.22.0

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details open>
<summary>Clique para expandir / recolher notas de lançamento em português</summary><br>

# Compression Manager — Versão 2026.11.22.0 (Estabilidade do Layout Duplo, C++23, Qt 6.12.0, Distribuições Ultralimpas & Duplo Licenciamento)

**Data de Lançamento:** 22 de novembro de 2026  
**Autor:** Fernando Nillsson Cidade (`linceu_lighthouse@outlook.com`)  
**Repositório:** [`fernandoncidade/Compression_Manager_cpp`](https://github.com/fernandoncidade/Compression_Manager_cpp)  
**Edições:** **Full** (`ENABLE_RAR_SUPPORT=ON`: RAR, ZIP, 7Z, TAR, WIM) e **Standart** (`ENABLE_RAR_SUPPORT=OFF`: ZIP, 7Z, TAR, WIM)  
**Licenciamento:** GNU AGPLv3 (Código Aberto) / EULA Oficial + CLC (Pacotes Binários Oficiais / Microsoft Store)  

---

## Principais Entregas e Aprimoramentos

### 1. Estabilidade Completa do Layout Duplo e Preservação de Janela Maximizada
- **Transição Imediata e Confiável (`ginter_21_toggle_segundo_layout.cpp`):** Ativar e desativar o **Segundo Layout (Layout Duplo)** via *Configurações > Layouts* agora transita de forma suave e determinística, preservando listas de entrada (`folder_listbox`), diretórios de saída por formato, checkboxes ativos e métodos selecionados.
- **Preservação Rigorosa da Janela Maximizada:** O aplicativo mantém integralmente o estado `isMaximized()` ao alternar entre o **Layout Padrão** e o **Layout Duplo**, eliminando restaurações indesejadas de janela ou saídas acidentais de tela cheia.
- **Adaptação Fluida de Orientação (`ginter_27_adapt_layout_orientation.cpp`):** Em telas panorâmicas ou maximizadas (`width >= height / 1.5`), os dois fluxos organizam-se lado a lado (`QHBoxLayout`); em janelas altas (`win_h > 1.5 * win_w`), alternam automaticamente para empilhamento vertical (`QVBoxLayout`).

### 2. Modernização Integral da Toolchain (C++23, Qt 6.12.0, MSVC 14.52 & MinGW 15.1.0)
- **Migração para C++23 & Qt 6.12.0 (`CMakeLists.txt`, `CMakePresets.json`):** Todo o projeto (aplicação principal, utilitários em `mocks/` e suíte em `tests/`) passou a compilar sob padrão estrito `C++23` (`CMAKE_CXX_EXTENSIONS OFF`) e **Qt 6.12.0** (`Core`, `Gui`, `Widgets`, `Test`, `LinguistTools`) via CMake `3.28+` com **MSVC puro** (`Visual Studio 18 2026` / MSBuild) e **MinGW** (Ninja).
- **Suporte Dual MSVC x64 & MinGW-w64 x64 Isolado:** Presets unificados para **MSVC `14.52.37025`** (Visual Studio 18 Build Tools / MSBuild / Windows SDK `10.0.28000.0`, gerando diretórios `build_*_msvc` e `*_msvc`) e **MinGW-w64 GCC `15.1.0`** (`C:\Qt\Tools\mingw1510_64`), com sobrescrita explícita das DLLs GCC `15.1.0` no deploy MinGW para evitar contaminação por DLLs legadas dentro da pasta de instalação do Qt.
- **Correção de Scripts Batch de Ambiente (`cmake/msvc_env.bat`, `cmake/mingw_env.bat`):** Eliminação de falhas de expansão de parênteses com `%ProgramFiles(x86)%` em ambiente CMD/VS Code.

### 3. Distribuições Externas Ultralimpas e Instaladores Inno Setup 7 Compactos
- **Eliminação de `vc_redist.x64.exe` via CRT App-Local (`cmake/copy_msvc_runtime.cmake`, `cmake/remove_compiler_runtime_installer.cmake`):** As distribuições MSVC implantam diretamente na pasta do executável apenas as 5 DLLs essenciais da `Microsoft.VC145.CRT` (`msvcp140.dll`, `msvcp140_1.dll`, `msvcp140_2.dll`, `vcruntime140.dll`, `vcruntime140_1.dll`), evitando embutir os ~18 MB do instalador redistribuível e tornando o instalador MSVC ainda menor que o MinGW (`13,05 MB` vs. `15,13 MB` na Full e `11,57 MB` vs. `13,63 MB` na Standart).
- **Poda Estrita de Módulos, Plugins, Banners e Chaves Não Redistribuíveis (`cmake/copy_lean_*.cmake`, `cmake/gerar_distribuicoes_limpas.ps1`):**
  - Exclusão de `Qt6Network.dll`, `Qt6Pdf.dll`, `Qt6Svg.dll`, `Qt6PrintSupport.dll` e plugins gráficos/rede/TLS não carregados pelo aplicativo, além de filtragem de resíduos MSBuild/Visual Studio (`.sln`, `.vcxproj`, `.dir`, `x64`);
  - Exclusão de banners promocionais Microsoft Store e PDFs das pastas de distribuição final;
  - Bloqueio rigoroso contra qualquer cópia de `rarreg.key`, `.chm` ou utilitários extras do WinRAR na Edição Full, e segregação total na Edição Standart;
  - Guardas `#if` e `#error` em tempo de compilação nos três scripts Inno Setup 7 em [`library/`](../library/Compression_Manager.iss) (`Compression_Manager.iss`, `Compression_Manager_MSVC.iss`, `Compression_Manager_MinGW.iss`).

### 4. Suíte de Testes de Engenharia, Ferramentas Mocks & Tarefas VS Code
- **Suíte Completa de Testes Automatizados (`tests/`):** Implementação de 10 executáveis `Qt6::Test` integrados ao **CTest** (`build/build_suite_msvc` e `build/build_suite_mingw`, sem distribuição externa), cobrindo ambiente/plugins Qt, persistência e logs, traduções `.qm`, ativos legais, métodos de compressão, Drag & Drop UIPI, empacotamento/extração E2E com checagem SHA-256, UI Full vs. Standart e higiene de pacotes (`10/10` aprovados em MSVC e MinGW).
- **Migração para `library/` & Auditoria Ampliada (`mocks/find_VersionEditor.cpp`):** Movimentação de `AppxManifest.xml`, `Compression_Manager_cpp.txt` e scripts `.iss` para `library/`, ampliação de monitoramento para 34 verificações sincronizadas (incluindo badge `Latest Release` no `README.md`) e simplificação de `.vscode/tasks.json` para as 8 tarefas principais ordenadas (`CM-FULL`, `CM-STANDART`, `MOCKS`, `SUITE` em MinGW e MSVC) com subtarefas ocultadas.

### 5. Duplo Licenciamento (GNU AGPLv3 + EULA Oficial / CLC) e Documentação Completa
- **Modelo Transparente em `LICENSE`, `docs/` e `source/assets/`:** Adoção explícita da **GNU Affero General Public License v3 (`LICENSE`)** para o código-fonte aberto no GitHub e do **EULA Oficial + CLC** para os pacotes binários oficiais assinados na Microsoft Store e instaladores do autor, mantendo integral liberdade de adaptação de `source/assets/` para forks e colaboradores.
- **Suíte Documental Bilíngue & Galerias de Interface:** Criação e atualização em Português (BR) e Inglês (US) de `README.md` (incluindo capturas em `images/interfaces/{pt_BR,en_US}/` para Full e Standart), `MANUAL.md`, `OBSERVACAO.md`, `CONTRIBUTING.md`, `CODE_OF_CONDUCT.md`, `SECURITY.md`, templates `.github/`, documentação técnica em `docs/` e recursos internos em `source/assets/`.

---

## Histórico Consolidado de Versões Anteriores

- **`2026.10.16.0` (16/10/2026):** Consolidação da arquitetura nativa em C++ + Qt 6 para as edições **Full** (RAR, ZIP, 7Z, TAR, WIM) e **Standart** (ZIP, 7Z, TAR, WIM), Layout Duplo adaptativo, filas assíncronas em segundo plano, Drag & Drop com ponte UIPI, troca dinâmica de idioma, Manual integrado e operação 100% offline.
- **`2026.8.9.0` (09/08/2026):** Introdução do suporte ao empacotamento RAR (na versão Full), primeira geração do Layout Duplo e sincronização automática de versionamento por calendário (`find_VersionEditor`).
- **`0.0.1.0` – `0.0.4.0` (Out–Dez/2025):** Série inicial (protótipo Python/PySide6) com multiformato 7-Zip, múltiplos diretórios de saída, alinhamento das caixas de saída e cálculo da largura mínima dos botões laterais.

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details open>
<summary>Click to expand / collapse release notes in English</summary><br>

# Compression Manager — Version 2026.11.22.0 (Dual Layout Stability, C++23, Qt 6.12.0, Ultra-Lean Deployments & Dual Licensing)

**Release Date:** November 22, 2026  
**Author:** Fernando Nillsson Cidade (`linceu_lighthouse@outlook.com`)  
**Repository:** [`fernandoncidade/Compression_Manager_cpp`](https://github.com/fernandoncidade/Compression_Manager_cpp)  
**Editions:** **Full** (`ENABLE_RAR_SUPPORT=ON`: RAR, ZIP, 7Z, TAR, WIM) and **Standart** (`ENABLE_RAR_SUPPORT=OFF`: ZIP, 7Z, TAR, WIM)  
**Licensing:** GNU AGPLv3 (Open Source) / Official EULA + CLC (Official Signed Store & Installer Packages)  

---

## Major Deliverables & Improvements

### 1. Complete Dual Layout Stability & Maximized Window Preservation
- **Instant, Reliable Layout Switching (`ginter_21_toggle_segundo_layout.cpp`):** Enabling and disabling the **Second Layout (Dual Layout)** under *Settings > Layouts* now transitions cleanly and deterministically while keeping queued input lists (`folder_listbox`), per-format output folders, active format checkboxes, and compression levels intact.
- **Strict Maximized State Preservation:** Switching between **Standard Layout** and **Dual Layout** preserves `isMaximized()` without unintended window restores or exiting full-screen workflows.
- **Fluid Responsive Orientation (`ginter_27_adapt_layout_orientation.cpp`):** Automatically arranges panels side-by-side (`QHBoxLayout`) on wide/maximized displays and stacks vertically (`QVBoxLayout`) on narrow portrait windows (`win_h > 1.5 * win_w`).

### 2. Full Toolchain Modernization (C++23, Qt 6.12.0, MSVC 14.52 & MinGW 15.1.0)
- **Migration to C++23 & Qt 6.12.0 (`CMakeLists.txt`, `CMakePresets.json`):** The desktop application, developer tooling in `mocks/`, and test suite in `tests/` now compile under strict `C++23` (`CMAKE_CXX_EXTENSIONS OFF`) and **Qt 6.12.0** (`Core`, `Gui`, `Widgets`, `Test`, `LinguistTools`) using CMake `3.28+` with **pure MSVC** (`Visual Studio 18 2026` / MSBuild) and **MinGW** (Ninja).
- **Isolated Dual MSVC x64 & MinGW-w64 x64 Presets:** Unified presets for **MSVC `14.52.37025`** (Visual Studio 18 Build Tools / MSBuild / Windows SDK `10.0.28000.0`, producing `build_*_msvc` and `*_msvc` folders) and **MinGW-w64 GCC `15.1.0`** (`C:\Qt\Tools\mingw1510_64`), with explicit GCC `15.1.0` runtime DLL copying during MinGW deploy to prevent legacy DLL leakage from Qt's bin directory.
- **Batch Environment Script Fixes (`cmake/msvc_env.bat`, `cmake/mingw_env.bat`):** Eliminated CMD parenthesis parsing failures caused by `%ProgramFiles(x86)%`.

### 3. Ultra-Lean External Distributions & Compact Inno Setup 7 Installers
- **Elimination of `vc_redist.x64.exe` via App-Local CRT (`cmake/copy_msvc_runtime.cmake`, `cmake/remove_compiler_runtime_installer.cmake`):** MSVC distributions deploy only the 5 essential `Microsoft.VC145.CRT` DLLs directly alongside the executable (`msvcp140.dll`, `msvcp140_1.dll`, `msvcp140_2.dll`, `vcruntime140.dll`, `vcruntime140_1.dll`), eliminating the ~18 MB redistributable installer and making MSVC setup packages smaller than MinGW setup packages (`13.05 MB` vs. `15.13 MB` Full; `11.57 MB` vs. `13.63 MB` Standart).
- **Strict Pruning of Unused Qt Modules, Store Banners, and Non-Redistributable Keys (`cmake/copy_lean_*.cmake`, `cmake/gerar_distribuicoes_limpas.ps1`):**
  - Excludes `Qt6Network.dll`, `Qt6Pdf.dll`, `Qt6Svg.dll`, `Qt6PrintSupport.dll`, unused image/TLS/network plugins, and Visual Studio / MSBuild build artifacts (`.sln`, `.vcxproj`, `.dir`, `x64`);
  - Excludes Store promotional banner artwork and PDF documents from standalone runtime folders;
  - Strictly prohibits `rarreg.key`, `.chm`, or extraneous WinRAR files in the Full Edition, and enforces complete WinRAR exclusion in Standart Edition;
  - Enforced by compile-time `#if` and `#error` gates across all three Inno Setup 7 scripts in [`library/`](../library/Compression_Manager.iss) (`Compression_Manager.iss`, `Compression_Manager_MSVC.iss`, `Compression_Manager_MinGW.iss`).

### 4. Engineering Test Suite, Developer Tooling & VS Code Workflow
- **Complete Automated Test Suite (`tests/`):** Added 10 `Qt6::Test` executables integrated into **CTest** (`build/build_suite_msvc` and `build/build_suite_mingw`, zero external distribution), covering Qt plugin loading, local persistence & logs, `.qm` translations, legal/licensing checks, compression methods, UIPI Drag & Drop, E2E archiving/extraction with SHA-256 verification, Full vs. Standart UI tests, and repo hygiene (`10/10` passing on pure MSVC and MinGW).
- **`library/` Layout & Expanded Audits (`mocks/find_VersionEditor.cpp`):** Relocated `AppxManifest.xml`, `Compression_Manager_cpp.txt`, and `.iss` scripts into `library/`, expanded version/date checks to 34 targets (including the `Latest Release` badge in `README.md`), and streamlined `.vscode/tasks.json` to the 8 ordered main tasks (`CM-FULL`, `CM-STANDART`, `MOCKS`, `SUITE` in MinGW & MSVC) with hidden subtasks.

### 5. Dual Licensing (GNU AGPLv3 + Official EULA / CLC) & Full Bilingual Documentation
- **Transparent Licensing Model (`LICENSE`, `docs/`, `source/assets/`):** Adopts **GNU Affero General Public License v3 (`LICENSE`)** for open-source code on GitHub alongside the **Official EULA + CLC** for author-signed Microsoft Store and standalone binary packages, while granting developers and forks full freedom to adapt institutional files under `source/assets/`.
- **Comprehensive Bilingual Documentation & UI Galleries:** Created and updated English (US) and Brazilian Portuguese (BR) documentation across `README.md` (with annotated Full & Standart UI screenshots in `images/interfaces/{pt_BR,en_US}/`), `MANUAL.md`, `OBSERVACAO.md`, `CONTRIBUTING.md`, `CODE_OF_CONDUCT.md`, `SECURITY.md`, `.github/` templates, `docs/`, and internal `source/assets/` resources.

---

## Historical Release Summary

- **`2026.10.16.0` (October 16, 2026):** Complete C++ + Qt 6 architecture across **Full** (RAR, ZIP, 7Z, TAR, WIM) and **Standart** (ZIP, 7Z, TAR, WIM) editions, adaptive Dual Layout, background queueing, Drag & Drop with elevated UIPI fallback, dynamic language switching, integrated Manual, and 100% offline execution.
- **`2026.8.9.0` (August 9, 2026):** Added RAR archive packaging (Full Edition), first-generation Dual Layout, and automated calendar versioning (`find_VersionEditor`).
- **`0.0.1.0` – `0.0.4.0` (Oct–Dec 2025):** Initial series (Python/PySide6 prototype) introducing multiformat 7-Zip packaging, multiple output folders, button alignment refinements, and consistent column widths.

</details>
