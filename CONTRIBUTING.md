# Guia de Contribuição / Contributing Guide — Compression Manager

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details open>
<summary>Clique para expandir / recolher as instruções em português</summary><br>

# Contribuindo com o Compression Manager

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  

Agradecemos muito pelo seu interesse em contribuir com o **Compression Manager**! Sua colaboração fortalece uma ferramenta gratuita, aberta, local e transparente para gerenciamento e arquivamento multiformato no Windows.

> [!NOTE]
> **Sobre o Desenvolvimento, Suporte e Contribuições Externas:**  
> Este projeto é concebido e mantido pelo autor como iniciativa pessoal e voluntária de desenvolvimento em C++23 e Qt 6.  
> Sugestões, *Issues* e *Pull Requests (PRs)* são sempre bem-vindos! No entanto, a revisão de código e a incorporação de alterações ocorrem no ritmo disponível do autor e sem garantia de prazo. Conforme assegurado pela licença **GNU AGPLv3**, você tem plena liberdade para criar um *Fork* independente deste repositório e adaptar inclusive os arquivos institucionais em `source/assets/` nos termos da documentação do projeto ([`README.md`](README.md#ptbr) e [EULA](source/assets/EULA/EULA_pt_BR%20-%20Compression%20Manager.txt)).

---

## 1. Código de Conduta

Ao participar deste projeto, você concorda em respeitar nosso [Código de Conduta](CODE_OF_CONDUCT.md#ptbr).

---

## 2. Formas de Contribuir

Você pode colaborar em diversas frentes (veja também [`docs/suggested-issues.md`](docs/suggested-issues.md#ptbr)):
- **Relatar Bugs:** Identifique problemas de interface, transições de layout, tratamento de caminhos especiais, detecção de motores ou empacotamento MSVC/MinGW.
- **Expandir Testes Automatizados (`tests/`):** Amplie a suíte de engenharia existente (`test_01`–`test_10` em `Qt6::Test` + `CTest`) cobrindo ambos os produtos (**Full** e **Standart**) e cenários adicionais de estresse.
- **Endurecimento de Subprocessos e I/O:** Migração de `QProcess::startCommand()` para lista explícita de argumentos (`QProcess::start(exe, args)`), escrita atômica com `QSaveFile` e rotação de logs no `LogManager`.
- **Aprimorar Documentação e Traduções:** Manuais bilíngues e catálogos `.ts` em `source/language/translations/`.

---

## 3. Configuração do Ambiente de Desenvolvimento

### 3.1. Pré-requisitos
- **Sistema Operacional:** Windows 10 (`17763+`) / Windows 11 x64
- **Compilador C++23:** **MSVC x64** (`14.52.37025`, VS 18 Build Tools, Windows SDK `10.0.28000.0`) e/ou **MinGW-w64 x64** (GCC `15.1.0` em `C:\Qt\Tools\mingw1510_64`)
- **Qt 6.12.0:** Módulos `Core`, `Gui`, `Widgets`, `Test`, `LinguistTools` (`msvc2022_64` e `mingw_64`)
- **CMake, MSBuild & Ninja:** CMake `3.28+`, Visual Studio 18 2026 / MSBuild (MSVC puro) e Ninja `1.13+` (MinGW)
- **Motores Externos para Deploy Local:** **7-Zip** instalado em `C:\Program Files\7-Zip` e (para Edição Full) **WinRAR** instalado em `C:\Program Files\WinRAR`
- **Empacotamento Opcional:** **Inno Setup 7** (`ISCC.exe`)

### 3.2. Compilando via Presets do CMake (`CMakePresets.json`) ou Tarefas do VS Code

No VS Code, você pode usar diretamente as 8 tarefas principais ordenadas em `.vscode/tasks.json` (`CM-FULL`, `CM-STANDART`, `MOCKS` e `SUITE`, em MinGW e MSVC):

```powershell
# Clonar o repositório
git clone https://github.com/fernandoncidade/Compression_Manager_cpp.git
cd Compression_Manager_cpp

# Edição Full (ENABLE_RAR_SUPPORT=ON)
cmake --preset full-msvc-release && cmake --build --preset full-msvc-release --target Compression_ManagerDeploy --parallel
cmake --preset full-mingw-release && cmake --build --preset full-mingw-release --target Compression_ManagerDeploy --parallel

# Edição Standart (ENABLE_RAR_SUPPORT=OFF)
cmake --preset standart-msvc-release && cmake --build --preset standart-msvc-release --target Compression_ManagerDeploy --parallel
cmake --preset standart-mingw-release && cmake --build --preset standart-mingw-release --target Compression_ManagerDeploy --parallel

# Ferramentas Auxiliares (find_VersionEditor e CompileTranslations)
cmake --preset mocks-msvc-release && cmake --build --preset mocks-msvc-release --target MocksDeploy --parallel
cmake --preset mocks-mingw-release && cmake --build --preset mocks-mingw-release --target MocksDeploy --parallel

# Suíte de Testes Completa (Qt6::Test + CTest; sem distribuição externa)
cmake --preset suite-msvc-release && cmake --build --preset suite-msvc-release --target SuiteDeploy --parallel && ctest --preset suite-msvc-release --output-on-failure
cmake --preset suite-mingw-release && cmake --build --preset suite-mingw-release --target SuiteDeploy --parallel && ctest --preset suite-mingw-release --output-on-failure
```

---

## 4. Diretrizes Arquiteturais e Regras de Higiene

1. **Padrão de Linguagem:** C++23 moderno (`std::unique_ptr`, `QPointer`, RAII, zero *raw owning pointers*).
2. **Separação Modular:**
   - `source/src_01_InicializadorMain.*`: Bootstrap, estilo nativo Qt 6 e geração segura de `qt.conf`.
   - `source/InterfaceCore/`: Janela principal (`InterfaceGrafica`), menus, coordenação de layouts e adaptação de orientação (`ginter_01..27`).
   - `source/GerenciamentoUI/`: Widgets reutilizáveis (`DragDropListWidget` com ponte UIPI, `LayoutsCompressao`, `GerenciadorInterface`, `SobreDialog`, `ManualDialog`).
   - `source/MotoresCompressao/`: Configuração persistente (`MetodoCompressao`), resolução de executáveis e classes `QThread` de empacotamento/extração/teste (`mtcomp_01..02`).
   - `source/language/` e `source/utils/`: `GerenciadorTraducao`, caminhos persistentes, ícones e `LogManager`.
   - `tests/`: Suíte automatizada com 10 executáveis (`test_01`–`test_10`) cobrindo plugins, i18n, licenciamento, motores E2E e integridade.
   - `cmake/` e `library/`: Scripts de distribuição ultralimpa e instaladores Inno Setup 7.
3. **Proibições Absolutas:**
   - Nunca incluir `rarreg.key` ou arquivos de licença de usuário no repositório ou em scripts de deploy/instalador.
   - Nunca misturar DLLs MSVC e MinGW na mesma pasta de distribuição nem embutir `vc_redist.x64.exe`.
   - Manter a Edição Standart (`ENABLE_RAR_SUPPORT=0`) 100% isenta de referências a RAR/WinRAR.

---

## 5. Processo de Submissão de Pull Requests

1. Crie uma branch a partir de `main` (`git checkout -b feat/minha-melhoria`).
2. Verifique que suas alterações mantêm compatibilidade tanto com **MSVC** quanto com **MinGW** nas edições **Full** e **Standart**.
3. Execute `ctest --preset suite-msvc-release --output-on-failure`, `ctest --preset suite-mingw-release --output-on-failure`, `.\build\build_Mocks_msvc\find_VersionEditor.exe --check` e valide as distribuições com `.\cmake\gerar_distribuicoes_limpas.ps1`.
4. Abra o Pull Request usando o template padrão em `.github/PULL_REQUEST_TEMPLATE.md`.

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details open>
<summary>Click to expand / collapse instructions in English</summary><br>

# Contributing to Compression Manager

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  

Thank you for your interest in contributing to **Compression Manager**! Your collaboration helps improve a free, open, local-first multiformat archiving utility for Windows.

> [!NOTE]
> **Development Context, Support & External Contributions:**  
> This project is developed and maintained by the author as a personal, voluntary C++23 and Qt 6 software initiative.  
> Suggestions, *Issues*, and *Pull Requests (PRs)* are always welcome! However, code reviews and merges proceed strictly at the author's available pace with no guaranteed timeline. Under **GNU AGPLv3**, you have complete freedom to maintain an independent *Fork* of this repository and freely customize institutional documents in `source/assets/` per project terms ([`README.md`](README.md#enus) and [Official EULA](source/assets/EULA/EULA_en_US%20-%20Compression%20Manager.txt)).

---

## 1. Code of Conduct

By participating in this project, you agree to abide by our [Code of Conduct](CODE_OF_CONDUCT.md#enus).

---

## 2. Ways to Contribute

You can contribute across several tracked areas (see [`docs/suggested-issues.md`](docs/suggested-issues.md#enus)):
- **Bug Reports:** Identify UI glitches, Dual Layout / maximized transition edge cases, special-path handling bugs, engine detection failures, or MSVC/MinGW packaging defects.
- **Expand Automated Tests (`tests/`):** Expand the existing 10-executable engineering suite (`test_01`–`test_10` built on `Qt6::Test` + `CTest`) covering both **Full** and **Standart** editions and additional stress scenarios.
- **Subprocess & Persistence Hardening:** Migrate `QProcess::startCommand()` calls to explicit argument arrays (`QProcess::start(exe, args)`), adopt atomic `QSaveFile` config writes, and implement bounded log rotation in `LogManager`.
- **Documentation & Localization:** Improve bilingual guides and `.ts` translation catalogs in `source/language/translations/`.

---

## 3. Development Environment Setup

### 3.1. Prerequisites
- **Operating System:** Windows 10 (`17763+`) / Windows 11 x64
- **C++23 Compiler:** **MSVC x64** (`14.52.37025`, VS 18 Build Tools, Windows SDK `10.0.28000.0`) and/or **MinGW-w64 x64** (GCC `15.1.0` at `C:\Qt\Tools\mingw1510_64`)
- **Qt 6.12.0:** Modules `Core`, `Gui`, `Widgets`, `Test`, `LinguistTools` (`msvc2022_64` and `mingw_64`)
- **CMake, MSBuild & Ninja:** CMake `3.28+`, Visual Studio 18 2026 / MSBuild (pure MSVC), and Ninja `1.13+` (MinGW)
- **External Archiving Engines:** **7-Zip** at `C:\Program Files\7-Zip` and (for Full Edition) **WinRAR** at `C:\Program Files\WinRAR`
- **Optional Installer Tooling:** **Inno Setup 7** (`ISCC.exe`)

### 3.2. Building with CMake Presets (`CMakePresets.json`) or VS Code Tasks

Inside VS Code, you can directly run the 8 ordered main tasks in `.vscode/tasks.json` (`CM-FULL`, `CM-STANDART`, `MOCKS`, and `SUITE` across MinGW & MSVC):

```powershell
# Clone repository
git clone https://github.com/fernandoncidade/Compression_Manager_cpp.git
cd Compression_Manager_cpp

# Full Edition (ENABLE_RAR_SUPPORT=ON)
cmake --preset full-msvc-release && cmake --build --preset full-msvc-release --target Compression_ManagerDeploy --parallel
cmake --preset full-mingw-release && cmake --build --preset full-mingw-release --target Compression_ManagerDeploy --parallel

# Standart Edition (ENABLE_RAR_SUPPORT=OFF)
cmake --preset standart-msvc-release && cmake --build --preset standart-msvc-release --target Compression_ManagerDeploy --parallel
cmake --preset standart-mingw-release && cmake --build --preset standart-mingw-release --target Compression_ManagerDeploy --parallel

# Auxiliary Tools (find_VersionEditor & CompileTranslations)
cmake --preset mocks-msvc-release && cmake --build --preset mocks-msvc-release --target MocksDeploy --parallel
cmake --preset mocks-mingw-release && cmake --build --preset mocks-mingw-release --target MocksDeploy --parallel

# Full Engineering Test Suite (Qt6::Test + CTest; zero external distribution)
cmake --preset suite-msvc-release && cmake --build --preset suite-msvc-release --target SuiteDeploy --parallel && ctest --preset suite-msvc-release --output-on-failure
cmake --preset suite-mingw-release && cmake --build --preset suite-mingw-release --target SuiteDeploy --parallel && ctest --preset suite-mingw-release --output-on-failure
```

---

## 4. Architectural & Hygiene Rules

1. **Language Standard:** Modern C++23 (`std::unique_ptr`, `QPointer`, RAII, no manual owning raw pointers).
2. **Modular Separation:**
   - `source/src_01_InicializadorMain.*`: Platform initialization, native Qt 6 style selection, and self-contained `qt.conf` setup.
   - `source/InterfaceCore/`: Main window (`InterfaceGrafica`), menus, Dual Layout choreography, and orientation adaptation (`ginter_01..27`).
   - `source/GerenciamentoUI/`: UI managers and widgets (`DragDropListWidget` with UIPI bridge, `LayoutsCompressao`, `GerenciadorInterface`, `SobreDialog`, `ManualDialog`).
   - `source/MotoresCompressao/`: Format preference persistence (`MetodoCompressao`), engine lookup, and `QThread` workers (`mtcomp_01..02`).
   - `source/language/` and `source/utils/`: `GerenciadorTraducao`, persistent path helpers, icons, and `LogManager`.
   - `tests/`: Automated 10-executable engineering suite (`test_01`–`test_10`) covering plugins, i18n, licensing, E2E archiving/extraction, and hygiene.
   - `cmake/` and `library/`: Ultra-lean deployment scripts and Inno Setup 7 installers.
3. **Strict Prohibitions:**
   - Never commit or deploy `rarreg.key` or personal registration keys.
   - Never mix MSVC and MinGW runtime DLLs in the same distribution or bundle `vc_redist.x64.exe`.
   - Keep Standart Edition (`ENABLE_RAR_SUPPORT=0`) 100% free of RAR/WinRAR references.

---

## 5. Pull Request Process

1. Create a feature branch from `main` (`git checkout -b feat/my-improvement`).
2. Verify builds across both **MSVC** and **MinGW** for **Full** and **Standart** editions.
3. Run `ctest --preset suite-msvc-release --output-on-failure`, `ctest --preset suite-mingw-release --output-on-failure`, `.\build\build_Mocks_msvc\find_VersionEditor.exe --check`, and inspect clean distributions with `.\cmake\gerar_distribuicoes_limpas.ps1`.
4. Submit your PR following `.github/PULL_REQUEST_TEMPLATE.md`.

</details>
