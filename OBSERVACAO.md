# OBSERVAÇÃO TÉCNICA — Estado atual do projeto

Versão documentada: `2026.11.22.0`
Data do snapshot técnico: `22 de novembro de 2026`
Repositório: `Compression_Manager_cpp`

---

## 1. Escopo e Propósito deste Documento

Este documento consolida o estado técnico, arquitetural e de distribuição do projeto **Compression_Manager_cpp** após a modernização para **C++23** nativo com **Qt 6.12.0 Widgets**, evolução a partir da base original em Python/PySide6 e do port inicial em C++17.

O documento detalha:
- A segregação rigorosa entre a **Edição Full** (`ENABLE_RAR_SUPPORT=ON`) e a **Edição Standart** (`ENABLE_RAR_SUPPORT=OFF`).
- Os fluxos unificados de compilação em **C++23 / Qt 6.12.0** via **MSVC puro x64 (`14.52.37025` / Visual Studio 18 2026 MSBuild / Windows SDK `10.0.28000.0`)** e **MinGW-w64 x64 (`GCC 15.1.0` / Ninja)** com CMake `3.28+`.
- A geração de distribuições externas ultralimpas em `D:\MISCELANEAS\VSCode\Compression_Manager` sem `vc_redist.x64.exe`, sem `rarreg.key`, sem banners promocionais da Store e sem módulos Qt não utilizados.
- Os instaladores balanceados do **Inno Setup 7** (com MSVC menor que MinGW devido ao CRT *app-local*).
- O modelo de **Duplo Licenciamento** (`GNU AGPLv3` + `EULA Oficial / CLC`), o ecossistema `.github/`, a documentação técnica em `docs/` e a árvore real atualizada do projeto.

---

## 2. Segregação Estrita de Produtos: Full vs. Standart

A compilação condicional controlada pela opção `ENABLE_RAR_SUPPORT` (`CMakeLists.txt:18-24`) produz duas edições isoladas:

### 2.1. Produto Full (`ENABLE_RAR_SUPPORT=1`)
- **Escopo:** Empacotamento em **RAR** (via `WinRAR.exe`), **ZIP**, **7Z**, **TAR** e **WIM** (via `7zG.exe` / `7z.exe`), além de extração multiformato (`.rar`, `.zip`, `.7z`, `.tar`, `.wim`, `.bz2`, `.tar.xz`, `.tgz`, `.tar.gz`, `.zipx`, `.lzh`, `.iso`).
- **Deploy Enxuto:** Copia `plugins/7Z/{7zG.exe, 7z.exe, 7z.dll, License.txt}` e `plugins/WinRAR/{WinRAR.exe, License.txt}` (`copy_lean_compression_engines.cmake`), além de `winrar3.png` (`copy_lean_app_assets.cmake`). Bloqueia explicitamente `rarreg.key`.
- **Distribuições & Instaladores:** `Compression_Manager_Full_{mingw,msvc}` e `Compression_Manager_Full_{mingw,msvc}_v2026.11.22.0.exe`.

### 2.2. Produto Standart (`ENABLE_RAR_SUPPORT=0`)
- **Escopo:** Versão focada exclusivamente em formatos abertos (**ZIP**, **7Z**, **TAR**, **WIM**) utilizando apenas o motor **7-Zip**.
- **Isolamento em Tempo de Compilação e Deploy:** `buscar_winrar_executavel()` retorna vazio (`#if !ENABLE_RAR_SUPPORT`), menus/checkboxes/manual/sobre omitem RAR e WinRAR, e tanto o deploy CMake quanto `gerar_distribuicoes_limpas.ps1` e os scripts `.iss` impedem com `#error` qualquer presença de `WinRAR.exe` ou `winrar3.png`.
- **Distribuições & Instaladores:** `Compression_Manager_Standart_{mingw,msvc}` e `Compression_Manager_Standart_{mingw,msvc}_v2026.11.22.0.exe`.

---

## 3. Diretórios de Build e Distribuição Externa Ultralimpa

### 3.1. Diretórios de Build Internos (`build/`)
```text
C:\Users\ferna\APLICATIVOS\CPP\Compression_Manager_cpp\build\
├── build_Compression_Manager_Full_mingw/
├── build_Compression_Manager_Full_msvc/
├── build_Compression_Manager_Standart_mingw/
├── build_Compression_Manager_Standart_msvc/
├── build_Mocks_mingw/
├── build_Mocks_msvc/
├── build_suite_mingw/
└── build_suite_msvc/
```

### 3.2. Distribuições Externas Enxutas (`D:\MISCELANEAS\VSCode\Compression_Manager`) & Instaladores Inno Setup 7
*(Valores reais verificados após remoção de `vc_redist.x64.exe`, `rarreg.key`, banners da Microsoft Store, `.pdf` e módulos Qt supérfluos, já com `source/assets/` sincronizados)*:

| Pasta de Distribuição | Produto | Toolchain (C++23 / Qt 6.12.0) | Arquivos | Tamanho Descompactado | Instalador Inno Setup 7 |
|---|---|---|---:|---:|---:|
| `Compression_Manager_Full_msvc` | Full (com RAR) | MSVC `14.52.37025` / SDK `10.0.28000.0` (CRT app-local) | `58` | `35,20 MB` | `13,05 MB` |
| `Compression_Manager_Full_mingw` | Full (com RAR) | MinGW-w64 GCC `15.1.0` | `56` | `41,96 MB` | `15,13 MB` |
| `Compression_Manager_Standart_msvc` | Standart (sem RAR) | MSVC `14.52.37025` / SDK `10.0.28000.0` (CRT app-local) | `55` | `31,96 MB` | `11,57 MB` |
| `Compression_Manager_Standart_mingw` | Standart (sem RAR) | MinGW-w64 GCC `15.1.0` | `53` | `38,72 MB` | `13,63 MB` |
| `Mocks_msvc` | Ferramentas Mocks | MSVC `14.52.37025` / SDK `10.0.28000.0` (CRT app-local) | `17` | `30,15 MB` | — |
| `Mocks_mingw` | Ferramentas Mocks | MinGW-w64 GCC `15.1.0` | `15` | `36,74 MB` | — |

---

## 4. Eliminação de `vc_redist.x64.exe` e Otimização MSVC vs. MinGW

1. **Implantação CRT App-Local sem `vc_redist.x64.exe`:** A inclusão de `vc_redist.x64.exe` (~18 MB pré-comprimidos) tornava o instalador MSVC artificialmente maior que o MinGW. Os scripts `cmake/copy_msvc_runtime.cmake` e `cmake/remove_compiler_runtime_installer.cmake` copiam localmente apenas as 5 DLLs da `Microsoft.VC145.CRT` (`msvcp140.dll`, `msvcp140_1.dll`, `msvcp140_2.dll`, `vcruntime140.dll`, `vcruntime140_1.dll`) e bloqueiam `vc_redist*.exe` via `#error` nos três `.iss`, deixando os instaladores MSVC menores do que os equivalentes MinGW.
2. **Poda Estrita de Qt 6.12.0:** Apenas `Qt6Core.dll`, `Qt6Gui.dll`, `Qt6Widgets.dll` e os plugins `platforms/qwindows.dll`, `styles/qmodernwindowsstyle.dll` e `imageformats/qico.dll` são mantidos (`prune_optional_root_runtime_dlls.cmake`, `deploy_optional_qt_binary.cmake`), descartando `Qt6Network`, `Qt6Pdf`, `Qt6Svg`, `Qt6PrintSupport`, `d3dcompiler_*`, `opengl32sw.dll` e plugins não usados.
3. **Sobrescrita Explícita das DLLs GCC `15.1.0` no MinGW e Stripping (`-s`):** O `CMakeLists.txt` e o `CMakePresets.json` priorizam `C:\Qt\Tools\mingw1510_64\bin` e sobrescrevem `libgcc_s_seh-1.dll`, `libstdc++-6.dll` e `libwinpthread-1.dll` após o `windeployqt` para impedir regressão às DLLs mais antigas distribuídas dentro de `C:\Qt\6.12.0\mingw_64\bin`.

---

## 5. Ferramentas Mocks, Suíte de Testes e Controle de Versão

- **`mocks/find_VersionEditor.cpp`:** Audita (`--check`) e sincroniza versões e datas técnicas em todos os cabeçalhos de `source/assets/` (`ABOUT`, `CLC`, `COPYRIGHT`, `EULA`, `NOTICES`, `PRIVACY_POLICY`), `ginter_20_exibir_sobre.cpp`, `README.md` (incluindo o badge `Latest Release`), `MANUAL.md`, `OBSERVACAO.md`, `CODE_OF_CONDUCT.md`, `CONTRIBUTING.md`, `SECURITY.md`, `docs/*.md`, `library/AppxManifest.xml`, `library/Compression_Manager_cpp.txt` e `library/*.iss`.
- **`mocks/CompileTranslationsGUI.cpp` e `CompileTranslationsTerminal.cpp`:** Validam e compilam catálogos `.ts` → `.qm` em `source/language/translations/`.
- **`mocks/limpar_build_qt6_regeneravel.ps1`:** Limpeza segura dos artefatos intermediários regeneráveis em `build/`.
- **Suíte Completa de Testes em `tests/` (`Qt6::Test` + `CTest`):** 10 executáveis (`test_01_framework_plugins_env` a `test_10_repo_integrity_hygiene`) integrados a `build/build_suite_msvc` (MSVC puro) e `build/build_suite_mingw` (MinGW), validando plugins, traduções, licenciamento, persistência, UIPI Drag & Drop, empacotamento E2E (SHA-256), edições Full/Standart e higiene do repositório (`10/10` aprovados em ambas as toolchains, sem gerar distribuição externa). O arquivo legado `teste.cpp` na raiz permanece vazio (`0 bytes`) apenas para compatibilidade retroativa.

---

## 6. Estabilidade do Layout Duplo, MSVC Puro e Duplo Licenciamento (Versão `2026.11.22.0`)

1. **Layout Duplo e Janela Maximizada (`ginter_21_toggle_segundo_layout.cpp`, `ginter_27_adapt_layout_orientation.cpp`):** Alternância estável preservando `isMaximized()`, duas instâncias independentes de `GerenciadorInterface`, adaptação dinâmica retrato/paisagem (`win_h > 1.5 * win_w`) e equalização geométrica da coluna de botões (`LayoutsCompressao`).
2. **MSVC Puro & Organização do VS Code (`.vscode/tasks.json`):** Compilações MSVC executadas exclusivamente via `Visual Studio 18 2026` (MSBuild, sem Ninja); painel de tarefas simplificado para as 8 tarefas principais ordenadas (`CM-FULL`, `CM-STANDART`, `MOCKS` e `SUITE` em MinGW e MSVC) com todas as subtarefas ocultadas (`"hide": true`).
3. **Duplo Licenciamento e Conformidade:** Adoção formal de **GNU AGPLv3** (`LICENSE`) para código-fonte aberto e **EULA Oficial + CLC** para distribuições assinadas do autor, com documentação completa bilíngue em `docs/` e `source/assets/`.

---

## 7. Estrutura Atualizada do Repositório

```text
Compression_Manager_cpp/
├── CMakeLists.txt
├── CMakePresets.json
├── CODE_OF_CONDUCT.md
├── CONTRIBUTING.md
├── LICENSE
├── MANUAL.md
├── OBSERVACAO.md
├── README.md
├── SECURITY.md
├── main.cpp
├── teste.cpp                             # Legado (0 bytes; suíte oficial ativa em tests/)
├── .github/
│   ├── ISSUE_TEMPLATE/
│   │   ├── bug_report.md
│   │   ├── config.yml
│   │   ├── feature_request.md
│   │   └── question.md
│   ├── PULL_REQUEST_TEMPLATE.md
│   └── workflows/
│       └── update-readme-version.yml
├── .vscode/
│   ├── c_cpp_properties.json
│   ├── cmake-kits.json
│   ├── settings.json
│   └── tasks.json
├── cmake/
│   ├── copy_lean_app_assets.cmake
│   ├── copy_lean_compression_engines.cmake
│   ├── copy_lean_mock_icons.cmake
│   ├── copy_msvc_runtime.cmake
│   ├── copy_optional_directory.cmake
│   ├── copy_qm_translations.cmake
│   ├── deploy_optional_qt_binary.cmake
│   ├── gerar_distribuicoes_limpas.ps1
│   ├── mingw_env.bat
│   ├── msvc_env.bat
│   ├── prune_optional_root_runtime_dlls.cmake
│   ├── remove_compiler_runtime_installer.cmake
│   ├── run_mingw.bat
│   └── run_msvc.bat
├── docs/
│   ├── architecture.md
│   ├── compliance.md
│   ├── licensing.md
│   ├── privacy-policy.md
│   ├── release-v2026.11.22.0.md
│   ├── security-model.md
│   ├── suggested-issues.md
│   ├── technical-audit-and-roadmap.md
│   ├── user-guide.md
│   └── images/
│       ├── banner.png
│       └── logo.png
├── images/
│   └── interfaces/
│       ├── pt_BR/                        # Telas Full & Standart (Layout Padrão e Duplo)
│       └── en_US/                        # Full & Standart screenshots (Standard & Dual Layout)
├── library/
│   ├── AppxManifest.xml
│   ├── Compression_Manager.iss
│   ├── Compression_Manager_MinGW.iss
│   ├── Compression_Manager_MSVC.iss
│   └── Compression_Manager_cpp.txt
├── mocks/
│   ├── CMakeLists.txt
│   ├── CompileTranslationsGUI.{cpp,hpp}
│   ├── CompileTranslationsTerminal.{cpp,hpp}
│   ├── INSTRUCOES_find_VersionEditor.txt
│   ├── find_VersionEditor.{cpp,hpp}
│   ├── icones/
│   ├── launch_detached_gui.cmake
│   └── limpar_build_qt6_regeneravel.ps1
├── repo/
│   └── prompt_Compression_Manager_cpp.txt
├── tests/
│   ├── CMakeLists.txt
│   ├── test_common.hpp
│   └── test_01 .. test_10                # Suíte Qt6::Test + CTest (MSVC & MinGW)
└── source/
    ├── src_01_InicializadorMain.{cpp,hpp}
    ├── GerenciamentoUI/                  # ui_01 .. ui_09
    ├── InterfaceCore/                    # intercore_01 & GerenciadorInterfaceGrafica/ginter_01 .. ginter_27
    ├── MotoresCompressao/                # mtcomp_01 .. mtcomp_02
    ├── language/                         # tr_01 .. tr_02 & translations/*.{ts,qm}
    ├── utils/                            # ApplicationPathUtils, CaminhoPersistenteUtils, IconUtils, LogManager
    └── assets/
        ├── ABOUT/                        # ABOUT_{en_US,pt_BR}.txt, History_APP_{en_US,pt_BR}.txt
        ├── CLC/                          # CLC_{en_US,pt_BR} - Compression Manager.txt
        ├── COPYRIGHT/                    # AVISO DE COPYRIGHT E MARCA REGISTRA_pt_BR.txt, COPYRIGHT AND TRADEMARK NOTICE_en_US.txt
        ├── EULA/                         # EULA_{en_US,pt_BR} - Compression Manager.txt
        ├── LICENSES/                     # License.txt, GNU AGPLv3, GNU GPLv3, GNU LGPLv2.1/v3, Apache 2.0, BSD-2/3, ISC, MIT, MS-Store-ADA-v8.9-FINAL-EN.pdf
        ├── NOTICES/                      # NOTICE_{en_US,pt_BR}.txt
        ├── PRIVACY_POLICY/               # Privacy_Policy_{en_US,pt_BR}.txt
        └── icones/
```
