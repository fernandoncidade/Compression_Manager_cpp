# Conformidade Normativa, Licenciamento e Verificação / Compliance & Verification — Compression Manager

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details open>
<summary>Clique para expandir / recolher o documento de conformidade em português</summary><br>

# Compression Manager — Conformidade Técnica, Regulatória e de Distribuição

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  
**Autor:** Fernando Nillsson Cidade  

---

## 1. Conformidade de Formatos de Compressão e Segregação de Edições

O **Compression Manager** segrega estritamente os formatos proprietários dos formatos abertos por meio da diretiva de compilação `ENABLE_RAR_SUPPORT` (`CMakeLists.txt`), assegurando conformidade previsível tanto em ambientes pessoais quanto institucionais:

| Formato / Operação | Edição Full (`ENABLE_RAR_SUPPORT=1`) | Edição Standart (`ENABLE_RAR_SUPPORT=0`) | Motor Responsável | Níveis Configuráveis |
|---|---|---|---|---|
| **Empacotamento `.rar`** | Suportado | **Estritamente Ausente** *(sem código, ícone ou binário)* | `WinRAR.exe` (externo) | `0` (Armazenar) a `5` (Ótimo) |
| **Empacotamento `.zip`** | Suportado | Suportado | `7zG.exe` / `7z.exe` | `0`, `1`, `3`, `5`, `7`, `9` |
| **Empacotamento `.7z`** | Suportado | Suportado | `7zG.exe` / `7z.exe` | `0`, `1`, `3`, `5`, `7`, `9` |
| **Empacotamento `.tar`** | Suportado | Suportado | `7zG.exe` / `7z.exe` | `0` (Armazenar) |
| **Empacotamento `.wim`** | Suportado | Suportado | `7zG.exe` / `7z.exe` | `0` (Armazenar) |
| **Extração Multiformato** | `.rar`, `.zip`, `.7z`, `.tar`, `.wim`, `.iso`, `.gz`, `.bz2`, `.xz` etc. | `.zip`, `.7z`, `.tar`, `.wim`, `.iso`, `.gz`, `.bz2`, `.xz` etc. | `7zG.exe` / `7z.exe` | Preserva árvore original |
| **Teste de Integridade (`t`)** | Suportado (somatórios bloco a bloco sem gravar em disco) | Suportado (somatórios bloco a bloco sem gravar em disco) | `7zG.exe` / `7z.exe` | Validação pass/fail |

---

## 2. Matriz de Conformidade de Licenças e Binários de Terceiros

Para viabilizar redistribuição binária segura, audível e enxuta tanto por **MSVC x64 (`14.52.37025` / SDK `10.0.28000.0`)** quanto por **MinGW-w64 x64 (`GCC 15.1.0`)**, os scripts CMake (`cmake/*.cmake`) e instaladores (`library/*.iss`) aplicam regras automatizadas de higiene:

| Componente / Artefato | Regra de Conformidade | Mecanismo Automatizado de Verificação |
|---|---|---|
| **Código Autoral Compression Manager** | Duplo Licenciamento: **GNU AGPLv3** no código-fonte público ([`LICENSE`](../LICENSE)) + **EULA Oficial / CLC** nos pacotes assinados do autor. | Textos completos presentes na raiz e em [`source/assets/`](../source/assets/LICENSES/License.txt), replicados em `<dist>/assets/` via `copy_lean_app_assets.cmake`. |
| **Qt 6.12.0 (`Core`, `Gui`, `Widgets`)** | Conformidade **GNU LGPLv3** garantida via **linkagem dinâmica** (`.dll`s compartilhadas + plugins `qwindows.dll`, `qmodernwindowsstyle.dll`, `qico.dll`), permitindo substituição de DLLs ABI-compatíveis pelo usuário. | `windeployqt.exe` + `prune_optional_root_runtime_dlls.cmake`; versão exata `6.12.0.0` validada por `#if GetVersionNumbersString` em `library/*.iss` e `gerar_distribuicoes_limpas.ps1`. |
| **7-Zip (`plugins/7Z/`)** | Conformidade com os avisos de Igor Pavlov (GNU LGPL v2.1+, BSD-2-Clause, BSD-3-Clause e restrição unRAR): distribuição acompanhada do arquivo oficial `License.txt` em `plugins/7Z/` e atribuições completas em `NOTICES/` e `LICENSES/License.txt`. | Cópia enxuta exclusiva (`7zG.exe`, `7z.exe`, `7z.dll`, `License.txt`) via [`cmake/copy_lean_compression_engines.cmake`](../cmake/copy_lean_compression_engines.cmake), com guarda `#if !FileExists` nos `.iss`. |
| **WinRAR (`plugins/WinRAR/` — Full Only)** | Segregação estrita e proibição absoluta de chave proprietária: `WinRAR.exe` + `License.txt` copiados **somente** quando `ENABLE_RAR_SUPPORT=ON`; bloqueio rigoroso de `rarreg.key`, `.chm`, shell extensions e SFX. | [`cmake/copy_lean_compression_engines.cmake`](../cmake/copy_lean_compression_engines.cmake) + [`cmake/gerar_distribuicoes_limpas.ps1`](../cmake/gerar_distribuicoes_limpas.ps1) + `#error` explícito em `library/*.iss` se `rarreg.key` ou contaminação WinRAR/Standart for detectada. |
| **MSVC CRT vs. MinGW Runtime** | Isolamento absoluto de toolchains: MSVC emprega implantação *app-local* (`Microsoft.VC145.CRT`: 5 DLLs) **sem embutir `vc_redist.x64.exe`**; MinGW emprega DLLs GCC `15.1.0` (`GPLv3` + *GCC Runtime Library Exception 3.1*) sem DLLs legadas do kit Qt. | Verificação cruzada por `#error` nos `.iss` (proibindo DLLs MinGW em pacotes MSVC e vice-versa, além de proibir `vc_redist*.exe`) e cópia incondicional das DLLs GCC `15.1.0` em `CMakeLists.txt`. |
| **Higiene Documental (`source/assets/`)** | Apenas documentação `.txt` e os 12–13 ícones efetivamente carregados em execução compõem `assets/`; documentos `.pdf` (ex.: `MS-Store-ADA-v8.9-FINAL-EN.pdf`) e banners da Store permanecem somente no repositório para submissão. | [`cmake/copy_lean_app_assets.cmake`](../cmake/copy_lean_app_assets.cmake) + guarda `#error` nos `.iss` + ausência de `PYTHON SOFTWARE FOUNDATION LICENSE.txt`. |

---

## 3. Conformidade com Políticas da Microsoft Store (Políticas v7.20 & MSIX)

O manifesto [`library/AppxManifest.xml`](../library/AppxManifest.xml) e os ativos textuais em [`source/assets/`](../source/assets/) foram estruturados para atender às diretrizes de certificação da **Microsoft Store**:

| Política Microsoft Store | Requisito | Como o Compression Manager Atende |
|---|---|---|
| **Política 10.1 (Descrição Precisa & Edição)** | Descrição fiel das funcionalidades sem alegações enganosas ou conflito de edição. | Textos diferenciados para **Edição Standart** e **Edição Full** via `#if ENABLE_RAR_SUPPORT` na interface (`ginter_20_exibir_sobre.cpp`, `ui_07_Manual.cpp`) e na documentação. |
| **Política 10.2 (Segurança, Sem Malware & Integridade)** | Binários x64 limpos, integridade de pacote e zero comportamento oculto. | `<uap10:PackageIntegrity><uap10:Content Enforcement="on" /></uap10:PackageIntegrity>` em `AppxManifest.xml`; zero serviços em segundo plano; exclusão de `vc_redist.x64.exe` e `rarreg.key`. |
| **Política 10.2.7 (Desinstalação Limpa)** | Remoção completa sem rastros residuais ou processos residentes. | Garantida em [`README.md`](../README.md#microsoft-store-privacidade-suporte), [`docs/privacy-policy.md`](privacy-policy.md#ptbr) e EULA; preferência/log somente em `%LOCALAPPDATA%\CompressionManager\`. |
| **Política 10.5 (Privacidade & Declaração Independente)** | App `runFullTrust` obrigatoriamente informa Política de Privacidade própria e independência frente à Microsoft. | [`docs/privacy-policy.md`](privacy-policy.md#ptbr) e `source/assets/PRIVACY_POLICY/Privacy_Policy_*.txt` declaram operação 100% offline e independência frente à Microsoft Privacy Statement. |
| **Política 10.6 (Capacidades Restritas)** | Declaração mínima e justificativa clara de `<rescap:Capability Name="runFullTrust" />`. | Única capacidade declarada em `AppxManifest.xml`, justificada exclusivamente pela execução desktop nativa Win32 Qt 6 e diálogos locais de arquivo (veja [`docs/licensing.md`](licensing.md#ptbr)). |

---

## 4. Auditoria de Versionamento e Validação de Empacotamento

1. **Consistência de Versão e Datas (`CalVer`):** Todos os arquivos em `source/assets/` (`ABOUT`, `CLC`, `COPYRIGHT`, `EULA`, `NOTICES`, `PRIVACY_POLICY`), `ginter_20_exibir_sobre.cpp`, `README.md` (incluindo o badge `Latest Release`), `MANUAL.md`, `OBSERVACAO.md`, `docs/*.md`, `library/AppxManifest.xml`, `library/Compression_Manager_cpp.txt` e `library/*.iss` seguem padrão validável automaticamente pelo utilitário [`mocks/find_VersionEditor.cpp`](../mocks/find_VersionEditor.cpp) (`find_VersionEditor.exe --check`).
2. **Qualidade de Traduções:** Os catálogos `source/language/translations/manager_compression_{pt_BR,en_US}.{ts,qm}` são validados e compilados via [`mocks/CompileTranslationsTerminal.cpp`](../mocks/CompileTranslationsTerminal.cpp).
3. **Verificação Integrada de Distribuições e Instaladores:** O script [`cmake/gerar_distribuicoes_limpas.ps1`](../cmake/gerar_distribuicoes_limpas.ps1) e os scripts `.iss` em `library/` atuam como *gates* de verificação antes da geração dos instaladores executáveis finais.
4. **Suíte Automatizada de Engenharia (`tests/`):** Dez executáveis `Qt6::Test` integrados ao CTest (`build/build_suite_msvc` e `build/build_suite_mingw`, sem distribuição externa) validam carregamento de plugins Qt, traduções ao vivo, conformidade de licenças e ativos, motores 7-Zip/WinRAR ponta a ponta, segregação estrita Full vs. Standart e integridade do repositório. *(Veja o inventário completo e ADRs em [`docs/technical-audit-and-roadmap.md`](technical-audit-and-roadmap.md).)*

---

## 5. Caráter Auxiliar Gratuito e Responsabilidade pelos Dados ("AS IS")

- O **Compression Manager** é um aplicativo gratuito de uso auxiliar, fornecido **"NO ESTADO EM QUE SE ENCONTRA" ("AS IS")**, sem garantias explícitas ou implícitas ([EULA Oficial](../source/assets/EULA/EULA_pt_BR%20-%20Compression%20Manager.txt), Lei nº 9.609/1998 e [GNU AGPLv3](../LICENSE) Seções 15–16).
- O usuário deve sempre validar pacotes importantes com o recurso **Testar Integridade** e manter cópias de segurança independentes de seus arquivos críticos antes de operações de arquivamento em massa.

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details open>
<summary>Click to expand / collapse compliance in English</summary><br>

# Compression Manager — Technical, Regulatory & Distribution Compliance

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  
**Author:** Fernando Nillsson Cidade  

---

## 1. Archive Format Compliance & Edition Segregation

**Compression Manager** enforces strict compile-time separation between proprietary and open archive formats via `ENABLE_RAR_SUPPORT` (`CMakeLists.txt`), providing predictable licensing and compliance boundaries across personal and institutional workstations:

| Format / Operation | Full Edition (`ENABLE_RAR_SUPPORT=1`) | Standart Edition (`ENABLE_RAR_SUPPORT=0`) | Backing Engine | Supported Levels |
|---|---|---|---|---|
| **`.rar` Archiving** | Supported | **Strictly Absent** *(zero code, icon, or binary)* | `WinRAR.exe` (external) | `0` (Store) to `5` (Best) |
| **`.zip` Archiving** | Supported | Supported | `7zG.exe` / `7z.exe` | `0`, `1`, `3`, `5`, `7`, `9` |
| **`.7z` Archiving** | Supported | Supported | `7zG.exe` / `7z.exe` | `0`, `1`, `3`, `5`, `7`, `9` |
| **`.tar` Archiving** | Supported | Supported | `7zG.exe` / `7z.exe` | `0` (Store) |
| **`.wim` Archiving** | Supported | Supported | `7zG.exe` / `7z.exe` | `0` (Store) |
| **Multiformat Extraction** | `.rar`, `.zip`, `.7z`, `.tar`, `.wim`, `.iso`, `.gz`, `.bz2`, `.xz`, etc. | `.zip`, `.7z`, `.tar`, `.wim`, `.iso`, `.gz`, `.bz2`, `.xz`, etc. | `7zG.exe` / `7z.exe` | Preserves original tree |
| **Integrity Verification (`t`)** | Supported (block-by-block checksum without writing to disk) | Supported (block-by-block checksum without writing to disk) | `7zG.exe` / `7z.exe` | Pass/Fail validation |

---

## 2. Licensing & Third-Party Binary Hygiene Matrix

To produce auditable, clean, and compact binary distributions on both **MSVC x64 (`14.52.37025` / Windows SDK `10.0.28000.0`)** and **MinGW-w64 x64 (`GCC 15.1.0`)**, CMake scripts (`cmake/*.cmake`) and Inno Setup scripts (`library/*.iss`) enforce automated hygiene gates:

| Component / Artifact | Compliance Rule | Automated Verification Gate |
|---|---|---|
| **Compression Manager Authored Code** | Dual Licensing: **GNU AGPLv3** for public source code ([`LICENSE`](../LICENSE)) + **Official EULA / CLC** for author-signed official binaries. | Full texts stored at root and [`source/assets/`](../source/assets/LICENSES/License.txt), replicated to `<dist>/assets/` via `copy_lean_app_assets.cmake`. |
| **Qt 6.12.0 (`Core`, `Gui`, `Widgets`)** | **GNU LGPLv3** compliance via **dynamic linking** (shared `.dll` libraries + `qwindows.dll`, `qmodernwindowsstyle.dll`, `qico.dll`), preserving user ABI replacement rights under Section 4(d)(1). | `windeployqt.exe` + `prune_optional_root_runtime_dlls.cmake`; exact `6.12.0.0` version checked via `#if GetVersionNumbersString` in `library/*.iss` and `gerar_distribuicoes_limpas.ps1`. |
| **7-Zip (`plugins/7Z/`)** | Complies with Igor Pavlov's distribution terms (GNU LGPL v2.1+, BSD-2-Clause, BSD-3-Clause, unRAR restriction): bundled alongside upstream `License.txt` under `plugins/7Z/` and full attributions in `NOTICES/` and `LICENSES/License.txt`. | Selective lean copy (`7zG.exe`, `7z.exe`, `7z.dll`, `License.txt`) via [`cmake/copy_lean_compression_engines.cmake`](../cmake/copy_lean_compression_engines.cmake), verified by `#if !FileExists` in `.iss` scripts. |
| **WinRAR (`plugins/WinRAR/` — Full Only)** | Strict segregation and zero key-file inclusion: `WinRAR.exe` + `License.txt` copied **only** when `ENABLE_RAR_SUPPORT=ON`; `rarreg.key`, `.chm`, shell extensions, and SFX stubs are unconditionally blocked. | [`cmake/copy_lean_compression_engines.cmake`](../cmake/copy_lean_compression_engines.cmake) + [`cmake/gerar_distribuicoes_limpas.ps1`](../cmake/gerar_distribuicoes_limpas.ps1) + `#error` compile checks in `library/*.iss`. |
| **MSVC CRT vs. MinGW Runtime** | Strict toolchain isolation: MSVC uses **app-local** deployment (`Microsoft.VC145.CRT`: 5 DLLs) **without embedding `vc_redist.x64.exe`**; MinGW uses GCC `15.1.0` runtime DLLs (`GPLv3` + *GCC Runtime Library Exception 3.1*) avoiding older DLLs inside Qt's bin directory. | Cross-contamination `#error` checks in `library/*.iss` (rejecting MinGW DLLs in MSVC bundles and vice versa, plus rejecting `vc_redist*.exe`) and explicit GCC `15.1.0` DLL overwrite in `CMakeLists.txt`. |
| **Document Assets Hygiene (`source/assets/`)** | Only `.txt` legal docs and the 12–13 runtime icons ship in `<dist>/assets/`; `.pdf` reference artifacts (`MS-Store-ADA-v8.9-FINAL-EN.pdf`) and Store marketing banners remain repository-only. | [`cmake/copy_lean_app_assets.cmake`](../cmake/copy_lean_app_assets.cmake) + `#error` guards in `library/*.iss` + removal of obsolete `PYTHON SOFTWARE FOUNDATION LICENSE.txt`. |

---

## 3. Microsoft Store Policies (v7.20) & MSIX Manifest Compliance

[`library/AppxManifest.xml`](../library/AppxManifest.xml) and legal assets under [`source/assets/`](../source/assets/) are aligned with **Microsoft Store Policies v7.20**:

| Microsoft Store Policy | Requirement | How Compression Manager Complies |
|---|---|---|
| **Policy 10.1 (Accurate Description & Edition)** | Product listing accurately represents features without edition ambiguity. | Distinct **Standart** vs. **Full** copy driven by `#if ENABLE_RAR_SUPPORT` in UI (`ginter_20_exibir_sobre.cpp`, `ui_07_Manual.cpp`) and documentation. |
| **Policy 10.2 (Security, Clean Package Integrity)** | Clean x64 binaries, content integrity enforcement, and zero hidden actions. | `<uap10:PackageIntegrity><uap10:Content Enforcement="on" /></uap10:PackageIntegrity>` in `AppxManifest.xml`; zero background services; excludes `vc_redist.x64.exe` and `rarreg.key`. |
| **Policy 10.2.7 (Clean Uninstall)** | Clean removal leaving zero background services or residual hooks. | Documented in [`README.md`](../README.md#microsoft-store-privacy-support), [`docs/privacy-policy.md`](privacy-policy.md#enus), and EULA; settings/logs strictly under `%LOCALAPPDATA%\CompressionManager\`. |
| **Policy 10.5 (Independent Developer Privacy Policy)** | `runFullTrust` desktop apps must provide an independent Privacy Policy. | [`docs/privacy-policy.md`](privacy-policy.md#enus) and `source/assets/PRIVACY_POLICY/Privacy_Policy_*.txt` declare 100% offline operation and non-coverage by Microsoft's Privacy Statement. |
| **Policy 10.6 (Restricted Capabilities)** | Minimal capability declaration with clear certification note for `runFullTrust`. | Only `<rescap:Capability Name="runFullTrust" />` declared in `AppxManifest.xml`, justified strictly by native Win32 Qt 6 UI and local archive dialogs (see [`docs/licensing.md`](licensing.md#enus)). |

---

## 4. Version Auditing & Packaging Validation

1. **CalVer Consistency (`find_VersionEditor`):** All legal files under `source/assets/` (`ABOUT`, `CLC`, `COPYRIGHT`, `EULA`, `NOTICES`, `PRIVACY_POLICY`), `ginter_20_exibir_sobre.cpp`, `README.md` (including its `Latest Release` badge), `MANUAL.md`, `OBSERVACAO.md`, `docs/*.md`, `library/AppxManifest.xml`, `library/Compression_Manager_cpp.txt`, and `library/*.iss` are automatically audited for version/date synchronization via [`mocks/find_VersionEditor.cpp`](../mocks/find_VersionEditor.cpp) (`find_VersionEditor.exe --check`).
2. **Translation Catalog Integrity:** `.ts`/`.qm` resources under `source/language/translations/` are validated and compiled with [`mocks/CompileTranslationsTerminal.cpp`](../mocks/CompileTranslationsTerminal.cpp).
3. **End-to-End Distribution & Installer Gates:** [`cmake/gerar_distribuicoes_limpas.ps1`](../cmake/gerar_distribuicoes_limpas.ps1) and the Inno Setup scripts in `library/` serve as hard validation gates prior to publishing standalone installers.
4. **Automated Engineering Test Suite (`tests/`):** Ten `Qt6::Test` executables integrated with CTest (`build/build_suite_msvc` and `build/build_suite_mingw`, zero external distribution) validate Qt plugins, live localization, license/legal asset compliance, E2E 7-Zip/WinRAR execution, strict Full vs. Standart isolation, and repository hygiene. *(See the complete technical inventory and ADRs in [`docs/technical-audit-and-roadmap.md`](technical-audit-and-roadmap.md).)*

---

## 5. Free Auxiliary Utility Scope & Backup Responsibility ("AS IS")

- **Compression Manager** is a free auxiliary utility provided **"AS IS"** without express or implied warranties ([Official EULA](../source/assets/EULA/EULA_en_US%20-%20Compression%20Manager.txt), Brazilian Law No. 9.609/1998, and [GNU AGPLv3](../LICENSE) Sections 15–16).
- Users should always verify important archives via **Test Integrity** and maintain independent backup copies of critical data prior to bulk archiving or extraction workflows.

</details>
