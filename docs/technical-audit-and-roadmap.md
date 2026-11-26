# Auditoria Técnica, Decisões Arquiteturais, Matriz V&V e Roadmap / Technical Audit, ADRs & Roadmap — Compression Manager

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details open>
<summary>Clique para expandir / recolher o documento técnico em português</summary><br>

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  
**Autor:** Fernando Nillsson Cidade  

Este documento consolida a auditoria técnica, decisões arquiteturais (ADRs), matriz de integração, verificação e validação (V&V), conformidade legal/regulatória, limitações reais verificadas no código-fonte e roadmap do **Compression Manager**.

---

## A. Inventário Funcional e Classificação Estrita por Recurso

Cada módulo é classificado segundo o estado efetivamente verificado no código-fonte e nas execuções de build/deploy atuais (`Implementado`, `Integrado`, `Verificado`, `Documentado`, `Pendente`):

| Recurso / Módulo | Escopo Técnico Real | Classificação Atual | Evidência Principal no Repositório | Observação / Limitação |
|---|---|---|---|---|
| **Núcleo UI & Inicialização C++23 / Qt 6.12.0** | `iniciar_aplicacao()`, `qt.conf`, estilo nativo (`windows11` -> `windowsvista` -> `Fusion`), `Segoe UI 9pt` | Implementado, Integrado, Documentado | `source/src_01_InicializadorMain.cpp`, `CMakeLists.txt` | Substituiu integralmente protótipo Python/PySide6 |
| **Segregação Full vs. Standart (`ENABLE_RAR_SUPPORT`)** | Full: RAR+ZIP+7Z+TAR+WIM (`WinRAR` + `7-Zip`); Standart: ZIP+7Z+TAR+WIM (`7-Zip` puro) | Implementado, Integrado, Verificado, Documentado | `CMakeLists.txt`, `ginter_01_init_menu.cpp`, `ui_01_layoutsCompressao.cpp`, `mtcomp_02_motoresCompressao.cpp`, `cmake/copy_lean_*.cmake` | Segregação verificada em todas as 4 distribuições e scripts `.iss` |
| **Layout Duplo Paralelo & Preservação Maximizada** | Alternância `Layout 1`/`Layout 2` mantendo `isMaximized()` + adaptação vertical/horizontal | Implementado, Integrado, Documentado | `ginter_21_toggle_segundo_layout.cpp`, `ginter_27_adapt_layout_orientation.cpp`, `ui_01_layoutsCompressao.cpp` | Entrega central da versão `2026.11.22.0` |
| **Motores Assíncronos (`QThread` + `QProcess`)** | `Compressao{RAR,ZIP,7Z,TAR,WIM}`, `TesteIntegridade`, `Extracao`, fila `QQueue` por formato | Implementado, Integrado, Documentado | `source/MotoresCompressao/mtcomp_02_motoresCompressao.cpp`, `source/GerenciamentoUI/ui_02_gerenteGUILayouts.cpp` | Migração de `startCommand` -> lista de argumentos `QProcess::start` e propagação de `exitCode` mapeadas no backlog |
| **Drag & Drop com Ponte UIPI Elevada** | Aceita arquivos/pastas em entrada e pastas em saída; filtro `WM_DROPFILES` + HiDPI `devicePixelRatioF()` | Implementado, Integrado, Documentado | `source/GerenciamentoUI/ui_03_dragDrop.cpp` | Bypass escopado apenas quando `processo_esta_elevado()` é verdadeiro |
| **Internacionalização em Tempo de Execução** | Catálogos `.ts`/`.qm` (`pt_BR`, `en_US`), `CompileTranslations{GUI,Terminal}`, troca ao vivo | Implementado, Integrado, Verificado, Documentado | `source/language/tr_01_gerenciadorTraducao.cpp`, `mocks/CompileTranslationsTerminal.cpp` | Testado `CompileTranslationsTerminal.exe --help` |
| **Persistência Local & Logs** | `%LOCALAPPDATA%\CompressionManager\{config.json, language.json, logs\file_compression_*.log}` | Implementado, Integrado, Documentado | `CaminhoPersistenteUtils.cpp`, `mtcomp_01_metodosCompressao.cpp`, `LogManager.cpp` | Gravação atômica `QSaveFile` e retenção limitada de logs pendentes |
| **Distribuições Ultralimpas & Instaladores MSVC/MinGW** | MSVC 14.52 CRT app-local (sem `vc_redist.x64.exe`), MinGW 15.1.0 explícito, poda Qt/WinRAR/banners | Implementado, Verificado, Documentado | `cmake/*.cmake`, `cmake/gerar_distribuicoes_limpas.ps1`, `library/Compression_Manager*.iss` | Todas as 4 variantes + Mocks validadas |
| **Auditoria de Versão (`find_VersionEditor`)** | Inspeção/atualização sincronizada de versões e datas em `source/assets/`, badge/docs raiz e `.iss` | Implementado, Integrado, Verificado | `mocks/find_VersionEditor.cpp` | `34/34 [OK]` em MSVC e MinGW |
| **Suíte de Testes Automatizados (`tests/`)** | 10 executáveis `Qt6::Test` + `CTest` em `build/build_suite_{msvc,mingw}` (Full & Standart) | Implementado, Integrado, Verificado, Documentado | `tests/test_01..10`, `tests/CMakeLists.txt` | `10/10` aprovados em MSVC e MinGW (stub legado `teste.cpp` mantido com `0 bytes`) |

---

## B. Decisões Arquiteturais (ADRs)

- **ADR-001 — Segregação por Macro `ENABLE_RAR_SUPPORT` e Scripts CMake Dedicados:** Em vez de manter dois repositórios distintos para as edições **Full** e **Standart**, um único código-fonte controla menus, painéis, manual, descrições de "Sobre", ícones e binários externos via `ENABLE_RAR_SUPPORT` (`CMakeLists.txt`) somado a `copy_lean_compression_engines.cmake` e `copy_lean_app_assets.cmake`, evitando divergências de manutenção e assegurando ausência zero-residual de WinRAR na edição Standart.
- **ADR-002 — Eliminação de `vc_redist.x64.exe` via Implantação App-Local da `Microsoft.VC145.CRT`:** Embutir `vc_redist.x64.exe` (~18 MB pré-comprimidos) tornava o instalador Inno Setup MSVC artificialmente ~2× maior que o MinGW, mesmo quando os binários MSVC descompactados são menores (~31,9–35,2 MB vs. ~38,7–41,9 MB). Optou-se pela implantação *app-local* das 5 DLLs essenciais (`msvcp140*.dll`, `vcruntime140*.dll`) via `cmake/copy_msvc_runtime.cmake` e remoção defensiva em `remove_compiler_runtime_installer.cmake` + guarda `#error` nos `.iss`.
- **ADR-003 — Sobrescrita Explícita das DLLs GCC `15.1.0` no Deploy MinGW:** O diretório `bin` do pacote Qt `6.12.0\mingw_64` traz DLLs C++ runtime antigas (`13.1.0`) incompatíveis com recursos modernos do GCC `15.1.0`. O `CMakeLists.txt` e o `CMakePresets.json` forçam `C:\Qt\Tools\mingw1510_64\bin` em prioridade de `PATH` e sobrescrevem incondicionalmente `libgcc_s_seh-1.dll`, `libstdc++-6.dll` e `libwinpthread-1.dll` após `windeployqt.exe`.
- **ADR-004 — Ponte UIPI por `WM_DROPFILES` em Execuções Elevadas:** Como o Windows bloqueia arrastar-e-soltar OLE entre Explorer (`Medium`) e processos elevados (`High`), `DragDropListWidget` revoga o alvo OLE local somente quando elevado e processa `WM_DROPFILES` via `QAbstractNativeEventFilter` convertendo coordenadas por `devicePixelRatioF()`.
- **ADR-005 — Política Estrita Anti-Vazamento e Higiene de Pacotes:** Proibição rígida de `rarreg.key`, exclusão de módulos Qt não ligados (`Qt6Network`, `Qt6Pdf`, `Qt6Svg`, `Qt6PrintSupport`), banners promocionais, documentos `.pdf` e resíduos MSBuild/Visual Studio nas pastas de distribuição final, garantindo pacotes enxutos e juridicamente limpos.
- **ADR-006 — Duplo Licenciamento Transparente (`GNU AGPLv3` + `EULA Oficial / CLC`):** Código aberto público sob `GNU AGPLv3` garantindo auditabilidade e forks livres (incluindo liberdade integral sobre textos de `source/assets/`), combinado a `EULA Oficial + CLC` para proteger pacotes binários oficiais assinados na Microsoft Store e instaladores do autor contra revenda indevida ou apropriação de marca.

---

## C. Métricas Medidas de Distribuição e Instaladores (Qt 6.12.0 + C++23)

*Métricas reais verificadas após a sincronização completa dos ativos documentais em `source/assets/`, migração para MSVC puro (`Visual Studio 18 2026` / MSBuild) e geração via `ISCC.exe` (Inno Setup 7) e `gerar_distribuicoes_limpas.ps1`:*

| Edição / Variante | Toolchain | Arquivos na Distribuição | Tamanho Descompactado | Instalador Inno Setup 7 | Restrições Validadas |
|---|---|---:|---:|---:|---|
| **Full (MSVC puro)** | MSVC `14.52.37025` / MSBuild / SDK `10.0.28000.0` | `58` | `35,20 MB` | `13,05 MB` | Sem `vc_redist.x64.exe`, sem `rarreg.key`, sem DLLs MinGW |
| **Full (MinGW)** | MinGW-w64 GCC `15.1.0` / Ninja | `56` | `41,96 MB` | `15,13 MB` | DLLs GCC `15.1.0` autênticas, sem `rarreg.key`, sem DLLs MSVC |
| **Standart (MSVC puro)** | MSVC `14.52.37025` / MSBuild / SDK `10.0.28000.0` | `55` | `31,96 MB` | `11,57 MB` | Zero WinRAR/RAR, sem `vc_redist.x64.exe` |
| **Standart (MinGW)** | MinGW-w64 GCC `15.1.0` / Ninja | `53` | `38,72 MB` | `13,63 MB` | Zero WinRAR/RAR, DLLs GCC `15.1.0` autênticas |
| **Mocks (MSVC puro / MinGW)** | MSVC `14.52` / GCC `15.1.0` | `17` / `15` | `30,15 MB` / `36,74 MB` | — | Utilitários auxiliares limpos |

---

## D. Matriz de Rastreabilidade (`Requisito -> Código -> Validação -> Documentação`)

| Requisito | Implementação / Scripts | Evidência de Verificação | Documentação |
|---|---|---|---|
| **Segregação Full vs. Standart** | `CMakeLists.txt`, `ui_01_layoutsCompressao.cpp`, `copy_lean_compression_engines.cmake` | `test_08`, `test_09` e guardas `#error` nos 3 `library/*.iss` | [`compliance.md`](compliance.md#ptbr), [`architecture.md`](architecture.md#ptbr) |
| **Dual Layout & Maximização** | `ginter_21_toggle_segundo_layout.cpp`, `ginter_27_adapt_layout_orientation.cpp` | `test_08`, `test_09` e telas em `images/interfaces/` | [`user-guide.md`](user-guide.md#ptbr), [`release-v2026.11.22.0.md`](release-v2026.11.22.0.md#ptbr) |
| **Distribuição Ultralimpa MSVC/MinGW** | `cmake/copy_msvc_runtime.cmake`, `cmake/prune_optional_root_runtime_dlls.cmake`, `gerar_distribuicoes_limpas.ps1` | `test_01`, `test_10`, `Qt6Core == 6.12.0.0`, `ISCC.exe` | [`OBSERVACAO.md`](../OBSERVACAO.md) |
| **Duplo Licenciamento & Privacidade Local-First** | `LICENSE`, `source/assets/{EULA,CLC,COPYRIGHT,NOTICES,PRIVACY_POLICY}` | `test_04`, `test_10` e `library/AppxManifest.xml` | [`licensing.md`](licensing.md#ptbr), [`privacy-policy.md`](privacy-policy.md#ptbr) |
| **Sincronização CalVer** | `mocks/find_VersionEditor.cpp` | `find_VersionEditor.exe --check` (`34/34 [OK]`) e `test_10` | [`CONTRIBUTING.md`](../CONTRIBUTING.md#ptbr) |

---

## E. Limitações Conhecidas e Aviso Legal ("AS IS")

1. **Stub Legado `teste.cpp` & Execução em CI Remota:** A suíte completa `tests/` (`10/10` aprovados localmente via `ctest` em MSVC puro e MinGW) substituiu funcionalmente o alvo auxiliar legado `teste.cpp` (que permanece vazio com `0 bytes`), restando integrar os presets de suíte ao workflow remoto de CI no GitHub Actions.
2. **Construção de Comando e Propagação de Erros em `QProcess`:** `mtcomp_02_motoresCompressao.cpp` utiliza `QProcess::startCommand()` com string formatada e as mensagens finais de extração/teste de integridade não exibem detalhadamente o `exitCode()` individual do motor externo.
3. **Retenção de Logs e Gravação JSON:** `LogManager` cria um novo arquivo de log por sessão sem limite máximo automático de retenção; `config.json` e `language.json` usam escrita direta em vez de `QSaveFile`.
4. **Criptografia com Senha na GUI:** A interface gráfica atual concentra-se em métodos/níveis de compressão (`RAR 0–5`, `ZIP/7Z 0–9`, `TAR/WIM 0`), sem caixa de senha embutida na tela.
5. **Dependência Externa de Motores e Responsabilidade ("AS IS"):** O funcionamento de compressão, extração e verificação depende dos executáveis do **7-Zip** (e **WinRAR** na edição Full). Por ser software gratuito auxiliar fornecido **"NO ESTADO EM QUE SE ENCONTRA" ("AS IS")** sob o [EULA Oficial](../source/assets/EULA/EULA_pt_BR%20-%20Compression%20Manager.txt) e as Seções 15–16 da [GNU AGPLv3](../LICENSE), o usuário deve executar **Testar Integridade** após empacotamentos críticos e manter backups independentes de seus arquivos importantes.

---

## F. Roadmap Técnico Consolidado

| Prioridade | Frente Técnica | Entregas Planejadas | Critério de Aceitação |
|---|---|---|---|
| **P0 (Concluída)** | Toolchain C++23 / Qt 6.12.0 (MSVC puro & MinGW), Suíte `tests/` (`10/10`), Distribuições Limpas, Layout Duplo & Duplo Licenciamento | Presets MSVC puro (`VS 18 2026`) e MinGW, CRT app-local (sem `vc_redist`), suíte `tests/` (`test_01`–`test_10`), proteção `rarreg.key`, documentação em `docs/` e `source/assets/` | Todas as 4 distribuições + 4 instaladores compilam limpos (`MSVC < MinGW`) com `ctest` 10/10 em MSVC e MinGW |
| **P1** | Integração CI Remota & Endurecimento Thread/Process | Workflow CI para `suite-{msvc,mingw}-release`; migração para `QProcess::start(exe, args)`; captura de listas na UI thread + exibição real de `exitCode()` e aposentadoria de `teste.cpp` | Pipeline CI aprovado sem falsos positivos em falhas simuladas de arquivo corrompido |
| **P2** | Persistência Atômica & Retenção de Logs | `QSaveFile` em `config.json`/`language.json`; rotação automática de `logs\` (máx. 10–20 arquivos) | Tolerância a falhas de escrita sem perda de preferências |
| **P3** | Empacotamento MSIX Dual-Edition & Hardening Binário MSVC | Scripts reproduzíveis de geração MSIX (Full & Standart) + `/GS /sdl /guard:cf /CETCOMPAT` | Validação limpa pelo WACK (Windows App Certification Kit) |
| **P4** | Recursos Avançados de Arquivamento | Criptografia opcional AES-256 na GUI (sem persistir senhas em disco) + progresso percentual e cancelamento | Barra de progresso funcional em arquivos multi-GB |

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details open>
<summary>Click to expand / collapse technical audit in English</summary><br>

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  
**Author:** Fernando Nillsson Cidade  

This document consolidates the technical audit, Architecture Decision Records (ADRs), measured packaging metrics, verification and traceability matrices, honest source-grounded limitations, and roadmap for **Compression Manager**.

---

## A. Functional Inventory & Classification

| Module / Capability | Actual Codebase Scope | Current Status | Key Evidence in Repository | Notes / Limitations |
|---|---|---|---|---|
| **C++23 / Qt 6.12.0 Core & Bootstrap** | `iniciar_aplicacao()`, auto `qt.conf`, native styles (`windows11` -> `windowsvista` -> `Fusion`), `Segoe UI 9pt` | Implemented, Integrated, Documented | `source/src_01_InicializadorMain.cpp`, `CMakeLists.txt` | Complete native replacement of early Python/PySide6 prototype |
| **Full vs. Standart Segregation (`ENABLE_RAR_SUPPORT`)** | Full: RAR+ZIP+7Z+TAR+WIM (`WinRAR` + `7-Zip`); Standart: ZIP+7Z+TAR+WIM (`7-Zip` only) | Implemented, Integrated, Verified, Documented | `CMakeLists.txt`, `ginter_01_init_menu.cpp`, `ui_01_layoutsCompressao.cpp`, `mtcomp_02_motoresCompressao.cpp`, `cmake/copy_lean_*.cmake` | Enforced across all 4 external deployments and `.iss` scripts |
| **Parallel Dual Layout & Maximized Preservation** | `Layout 1` / `Layout 2` switching keeping `isMaximized()` + responsive portrait/landscape switch | Implemented, Integrated, Documented | `ginter_21_toggle_segundo_layout.cpp`, `ginter_27_adapt_layout_orientation.cpp`, `ui_01_layoutsCompressao.cpp` | Flagship stability deliverable of `v2026.11.22.0` |
| **Asynchronous Workers (`QThread` + `QProcess`)** | `Compressao{RAR,ZIP,7Z,TAR,WIM}`, `TesteIntegridade`, `Extracao`, per-format `QQueue` | Implemented, Integrated, Documented | `source/MotoresCompressao/mtcomp_02_motoresCompressao.cpp`, `source/GerenciamentoUI/ui_02_gerenteGUILayouts.cpp` | Argument-list `QProcess::start` refactor and exit-code UI surfacing tracked in backlog |
| **UIPI-Aware Drag & Drop Filter** | File/folder input and folder output drops; elevated `WM_DROPFILES` + HiDPI `devicePixelRatioF()` | Implemented, Integrated, Documented | `source/GerenciamentoUI/ui_03_dragDrop.cpp` | Scoped strictly to `processo_esta_elevado() == true` |
| **Runtime Internationalization** | `.ts`/`.qm` catalogs (`pt_BR`, `en_US`), `CompileTranslations{GUI,Terminal}`, live toggling | Implemented, Integrated, Verified, Documented | `source/language/tr_01_gerenciadorTraducao.cpp`, `mocks/CompileTranslationsTerminal.cpp` | `CompileTranslationsTerminal.exe --help` verified |
| **Local Persistence & Logs** | `%LOCALAPPDATA%\CompressionManager\{config.json, language.json, logs\file_compression_*.log}` | Implemented, Integrated, Documented | `CaminhoPersistenteUtils.cpp`, `mtcomp_01_metodosCompressao.cpp`, `LogManager.cpp` | Atomic `QSaveFile` writes and bounded log rotation pending |
| **Ultra-Lean Deployments & Dual Installers** | App-local MSVC 14.52 CRT (zero `vc_redist.x64.exe`), explicit MinGW 15.1.0, pruned Qt/banners | Implemented, Verified, Documented | `cmake/*.cmake`, `cmake/gerar_distribuicoes_limpas.ps1`, `library/Compression_Manager*.iss` | All 4 app variants + Mocks validated |
| **Version Synchronization (`find_VersionEditor`)** | Audited header regexes and badge sync across `source/assets/`, root docs, `README.md`, and `.iss` files | Implemented, Integrated, Verified | `mocks/find_VersionEditor.cpp` | `34/34 [OK]` across MSVC & MinGW |
| **Automated Test Suite (`tests/`)** | 10 `Qt6::Test` executables + `CTest` in `build/build_suite_{msvc,mingw}` (Full & Standart) | Implemented, Integrated, Verified, Documented | `tests/test_01..10`, `tests/CMakeLists.txt` | `10/10` passing on MSVC & MinGW (legacy stub `teste.cpp` remains `0 bytes`) |

---

## B. Architecture Decision Records (ADRs)

- **ADR-001 — Unified Source with Compile-Time `ENABLE_RAR_SUPPORT` & Selective Deploy Scripts:** Instead of forking repositories for Full vs. Standart editions, one C++23 codebase switches menus, panels, in-app manuals, icons, and bundled engines cleanly via `ENABLE_RAR_SUPPORT`, `copy_lean_compression_engines.cmake`, and `copy_lean_app_assets.cmake`.
- **ADR-002 — Eliminating `vc_redist.x64.exe` via App-Local `Microsoft.VC145.CRT`:** Bundling the ~18 MB precompressed Microsoft installer bloated MSVC Inno Setup installers past MinGW despite MSVC app binaries being smaller (~31.9–35.2 MB vs. ~38.7–41.9 MB). App-local deployment of the 5 CRT DLLs via `copy_msvc_runtime.cmake` plus `remove_compiler_runtime_installer.cmake` keeps MSVC installers smaller and completely self-contained.
- **ADR-003 — Unconditional GCC `15.1.0` Runtime DLL Overwrite:** Qt `6.12.0\mingw_64\bin` ships legacy MinGW 13.x runtime DLLs. CMake and `CMakePresets.json` prepend `C:\Qt\Tools\mingw1510_64\bin` and explicitly overwrite `libgcc_s_seh-1.dll`, `libstdc++-6.dll`, and `libwinpthread-1.dll` after `windeployqt.exe`.
- **ADR-004 — Scoped `WM_DROPFILES` UIPI Bridge:** When running with Administrator elevation, Windows UIPI blocks Explorer OLE drag-and-drop. `DragDropListWidget` revokes OLE drop registration locally when elevated and translates `WM_DROPFILES` drop points via `devicePixelRatioF()`.
- **ADR-005 — Strict Artifact & Key Hygiene:** Triple-blocked `rarreg.key` exclusion, unused Qt module pruning (`Qt6Network`, `Qt6Pdf`, `Qt6Svg`, `Qt6PrintSupport`), and removal of Microsoft Store marketing banners/PDFs and MSBuild clutter (`.sln`, `.vcxproj`, `.dir`, `x64`) from distribution folders.
- **ADR-006 — Transparent Dual Licensing (`GNU AGPLv3` + `Official EULA / CLC`):** Strong-copyleft `GNU AGPLv3` for open-source collaboration alongside explicit developer freedom over `source/assets/` templates, paired with an Official EULA + CLC governing the author's digitally signed Microsoft Store and desktop installer releases.

---

## C. Measured Distribution & Installer Metrics (Qt 6.12.0 + C++23)

*Measured metrics after complete `source/assets/` documentation synchronization, pure MSVC (`Visual Studio 18 2026` / MSBuild) migration, and packaging via `ISCC.exe` (Inno Setup 7) and `gerar_distribuicoes_limpas.ps1`:*

| Edition / Variant | Toolchain | Deployed Files | Unpacked Size | Inno Setup 7 Installer | Enforced Guardrails |
|---|---|---:|---:|---:|---|
| **Full (Pure MSVC)** | MSVC `14.52.37025` / MSBuild / SDK `10.0.28000.0` | `58` | `35.20 MB` | `13.05 MB` | Zero `vc_redist.x64.exe`, zero `rarreg.key`, zero MinGW DLLs |
| **Full (MinGW)** | MinGW-w64 GCC `15.1.0` / Ninja | `56` | `41.96 MB` | `15.13 MB` | Authentic GCC `15.1.0` DLLs, zero `rarreg.key`, zero MSVC DLLs |
| **Standart (Pure MSVC)** | MSVC `14.52.37025` / MSBuild / SDK `10.0.28000.0` | `55` | `31.96 MB` | `11.57 MB` | Zero WinRAR/RAR, zero `vc_redist.x64.exe` |
| **Standart (MinGW)** | MinGW-w64 GCC `15.1.0` / Ninja | `53` | `38.72 MB` | `13.63 MB` | Zero WinRAR/RAR, authentic GCC `15.1.0` DLLs |
| **Mocks (Pure MSVC / MinGW)** | MSVC `14.52` / GCC `15.1.0` | `17` / `15` | `30.15 MB` / `36.74 MB` | — | Clean auxiliary utilities |

---

## D. Traceability Matrix

| Requirement | Implementation / Scripts | Verification Evidence | Documentation |
|---|---|---|---|
| **Full vs. Standart Segregation** | `CMakeLists.txt`, `ui_01_layoutsCompressao.cpp`, `copy_lean_compression_engines.cmake` | `test_08`, `test_09`, and `.iss` `#error` gates | [`compliance.md`](compliance.md#enus), [`architecture.md`](architecture.md#enus) |
| **Dual Layout & Maximized Stability** | `ginter_21_toggle_segundo_layout.cpp`, `ginter_27_adapt_layout_orientation.cpp` | `test_08`, `test_09`, and screenshots in `images/interfaces/` | [`user-guide.md`](user-guide.md#enus), [`release-v2026.11.22.0.md`](release-v2026.11.22.0.md#enus) |
| **Lean MSVC & MinGW Deployments** | `cmake/copy_msvc_runtime.cmake`, `cmake/prune_optional_root_runtime_dlls.cmake`, `gerar_distribuicoes_limpas.ps1` | `test_01`, `test_10`, `Qt6Core == 6.12.0.0`, `ISCC.exe` | [`OBSERVACAO.md`](../OBSERVACAO.md) |
| **Dual Licensing & Local-First Privacy** | `LICENSE`, `source/assets/{EULA,CLC,COPYRIGHT,NOTICES,PRIVACY_POLICY}` | `test_04`, `test_10`, and `library/AppxManifest.xml` inspection | [`licensing.md`](licensing.md#enus), [`privacy-policy.md`](privacy-policy.md#enus) |
| **CalVer Synchronization** | `mocks/find_VersionEditor.cpp` | `find_VersionEditor.exe --check` (`34/34 [OK]`) & `test_10` | [`CONTRIBUTING.md`](../CONTRIBUTING.md#enus) |

---

## E. Known Limitations & Legal Disclaimer ("AS IS")

1. **Legacy `teste.cpp` Stub & Remote CI Wiring:** The complete `tests/` suite (`10/10` passing locally via `ctest` on pure MSVC & MinGW) supersedes the legacy empty `teste.cpp` target (`0 bytes`); automated remote CI execution in GitHub Actions remains on the P1 backlog.
2. **Subprocess Invocation & Error Surfacing:** `mtcomp_02_motoresCompressao.cpp` calls `QProcess::startCommand()` via string interpolation, and extraction/integrity-test completion dialogs do not yet surface individual non-zero engine exit codes.
3. **Log Retention & Non-Atomic Preference Writes:** `LogManager` creates a log file per session without automatic max-file pruning; `config.json` and `language.json` write directly via `QFile` rather than `QSaveFile`.
4. **Password Encryption in UI:** The GUI exposes format compression levels (`RAR 0–5`, `ZIP/7Z 0–9`, `TAR/WIM 0`) without interactive archive password encryption controls yet.
5. **External Engine Dependency & "AS IS" Disclaimer:** Archiving, extraction, and checksum verification rely on **7-Zip** (and **WinRAR** in Full Edition). Supplied free of charge **"AS IS"** under the [Official EULA](../source/assets/EULA/EULA_en_US%20-%20Compression%20Manager.txt) and [GNU AGPLv3](../LICENSE) Sections 15–16; users should run **Test Integrity** on critical archives and maintain independent backups.

---

## F. Consolidated Technical Roadmap

| Priority | Initiative | Planned Deliverables | Acceptance Criteria |
|---|---|---|---|
| **P0 (Completed)** | C++23 / Qt 6.12.0 Migration (Pure MSVC & MinGW), `tests/` Suite (`10/10`), Lean Deployments, Dual Layout & Dual Licensing | Pure MSVC (`VS 18 2026`) & MinGW presets, app-local CRT (no `vc_redist`), `tests/` suite (`test_01`–`test_10`), `rarreg.key` guardrails, full docs in `docs/` & `source/assets/` | All 4 deployments + 4 installers build cleanly (`MSVC < MinGW`) with `ctest` 10/10 on MSVC & MinGW |
| **P1** | Remote CI Integration & Thread/Process Hardening | CI PR workflow for `suite-{msvc,mingw}-release`; refactor `QProcess::start(exe, args)`; snapshot lists on GUI thread + surface `exitCode()` and retire `teste.cpp` | CI workflow passes without false positives on simulated corrupted archives |
| **P2** | Atomic Persistence & Log Pruning | `QSaveFile` for `config.json`/`language.json`; cap `logs\` at 10–20 files | Crash-safe preference writes and bounded log footprint |
| **P3** | Dual-Edition MSIX Automation & MSVC Hardening | Parameterized MSIX builds (Full & Standart) + `/GS /sdl /guard:cf /CETCOMPAT` | Clean Windows App Certification Kit (WACK) validation |
| **P4** | Advanced Archiving Features | Optional GUI AES-256 encryption (zero disk password persistence) + real-time progress bar & cancel | Smooth progress tracking on multi-GB jobs |

</details>
