# Modelo de Segurança e Resiliência / Security Model — Compression Manager

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details open>
<summary>Clique para expandir / recolher o modelo de segurança em português</summary><br>

# Compression Manager — Modelo de Segurança, Integridade de Dados e Resiliência

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  
**Autor:** Fernando Nillsson Cidade  

---

## 1. Princípios de Segurança e Filosofia de Design

O **Compression Manager** foi concebido para operar localmente sobre arquivos e pastas sob controle direto do usuário em estações de trabalho Windows 10/11 x64. Sua arquitetura fundamenta-se em cinco pilares de segurança e higiene de software:

1. **Soberania de Dados e Operação 100% Local-First / Air-Gapped:** O aplicativo não possui conexões de rede, telemetria, coleta em nuvem ou contas de usuário; nas distribuições enxutas, até mesmo `Qt6Network.dll` e os plugins `networkinformation` / `tls` são removidos intencionalmente do pacote distribuído.
2. **Higiene Rigorosa contra Chaves e Artefatos Proibidos (`rarreg.key`):** A cópia de chaves pessoais/licenças proprietárias de terceiros (`plugins/WinRAR/rarreg.key`) é bloqueada em múltiplas camadas redundantes (`cmake/copy_lean_compression_engines.cmake`, `cmake/gerar_distribuicoes_limpas.ps1` e `#error` explícito em todos os scripts `library/Compression_Manager*.iss`).
3. **Isolamento Estrito de Motores e Edições (`Full` vs. `Standart`):** Na Edição Standart (`ENABLE_RAR_SUPPORT=0`), nenhuma rota de código, binário ou arquivo ligado ao WinRAR é compilado ou copiado; tanto `7zG.exe`/`7z.exe` quanto `WinRAR.exe` (na Full) são executados localmente sob os caminhos padrão ou da subpasta `plugins/`.
4. **Isolamento App-Local de Runtimes e Verificação de Versão:** Builds MSVC implantam suas 5 DLLs da `Microsoft.VC145.CRT` localmente sem embutir o instalador redistribuível completo (`vc_redist.x64.exe`), enquanto builds MinGW-w64 `15.1.0` sobrescrevem DLLs GCC antigas eventualmente oriundas da pasta `bin/` do Qt, validando `Qt6Core.dll == 6.12.0.0` e bloqueando contaminação cruzada entre runtimes antes da geração dos instaladores Inno Setup.
5. **Gerenciamento Seguro de UI, Memória Qt e Elevação UIPI:** Uso de `std::unique_ptr` e `QPointer` (`layouts_compressao`, `gerenciador_traducao`, `dialog_manual`, `dialog_sobre`), diagnóstico de nível de privilégio no início da aplicação e tratamento restrito de `WM_DROPFILES` em sessões elevadas sem abertura irrestrita de processos.

---

## 2. Superfície de Ataque e Modelo de Ameaças

| Vetor Potencial | Nível de Risco | Medida Implementada / Observação Técnica |
|---|:---:|---|
| **Vazamento ou Redistribuição Indevida de Chaves (`rarreg.key`)** | Inexistente / Bloqueado | Bloqueio triplo em CMake, PowerShell e Inno Setup (`#error` se presente); somente `WinRAR.exe` e `License.txt` são permitidos na Edição Full. |
| **Exfiltração de Arquivos ou Telemetria Oculta** | Inexistente | Zero uso de sockets ou HTTP(S); módulos `Qt6Network` e `tls` ausentes nas distribuições finais; `config.json`, `language.json` e `logs\` estritamente locais em `%LOCALAPPDATA%\CompressionManager\`. |
| **Arquivos Compactados Maliciosos / *Zip Slip* / *Decompression Bomb*** | Médio (Mitigado pelo motor 7-Zip / WinRAR + validação prévia) | Descompactação delegada aos motores atualizados do usuário/sistema ou pacote (`7zG.exe` / `7z.exe`), recomendando-se executar **Testar Integridade** (`t`) e inspecionar arquivos de origem não confiável antes da extração automática (`-y`). Arquivos de saída existentes em extrações diretas são confirmados/sobrescritos conforme política da flag `-y` passada ao motor 7-Zip. |
| **Construção de Linha de Comando para Subprocessos (`QProcess`)** | Baixo / Em Endurecimento | Atualmente `mtcomp_02_motoresCompressao.cpp` constrói strings entre aspas para `QProcess::startCommand()`. Como boa prática de segurança de processos (Issue 2 em [`docs/suggested-issues.md`](suggested-issues.md#ptbr)), recomenda-se evitar caracteres de controle/aspas em nomes de pastas e planeja-se migrar para `QProcess::start(exe, QStringList{...})` (sem shell parser). |
| **Drag & Drop sob Elevação (Bypass Controlado UIPI)** | Baixo | `ui_03_dragDrop.cpp` habilita `ChangeWindowMessageFilterEx(MSGFLT_ALLOW)` apenas para mensagens padrão de arrastar arquivos (`WM_DROPFILES`, `WM_COPYDATA`, `WM_COPYGLOBALDATA`) e exclusivamente quando `processo_esta_elevado()` retorna verdadeiro, adicionando apenas caminhos locais válidos filtrados para `QFileInfo`. |
| **Contaminação Cruzada de DLLs MSVC / MinGW ou DLL Planting** | Baixo | `qt.conf` gerada junto ao executável trava o carregamento local (`Prefix = .\nPlugins = plugins\nTranslations = translations`), `gerar_distribuicoes_limpas.ps1` e `library/*.iss` garantem que não haja mistura entre DLLs `msvc*140` e `libgcc_s_seh-1`/`libstdc++-6`. |
| **Persistência Local e Retenção de Logs (`%LOCALAPPDATA%`)** | Baixo | `LogManager` grava logs técnicos com carimbo de data/hora (`file_compression_yyyyMMdd_hhmmss.log`) em `%LOCALAPPDATA%\CompressionManager\logs\` contendo apenas mensagens diagnósticas operacionais (sem senhas ou conteúdo de arquivos); política de rotação automática de logs antigos e escrita atômica `QSaveFile` estão mapeadas no backlog ([`docs/suggested-issues.md`](suggested-issues.md#ptbr)). |

---

## 3. Boas Práticas Recomendadas ao Usuário

- **Execute sempre com privilégios de usuário padrão:** O Compression Manager não requer privilégios de Administrador para compactar ou extrair documentos comuns em pastas do usuário; execute como Administrador apenas quando precisar gravar pacotes diretamente em pastas protegidas do sistema.
- **Verifique pacotes de terceiros com `Testar Integridade`:** Antes de extrair arquivos grandes recebidos por e-mail ou rede, utilize a função **Testar Integridade** para validar integridade estrutural de cabeçalhos e somas de verificação sem extrair para o disco.
- **Preserve a integridade do motor `7-Zip` e do pacote:** Prefira distribuições oficiais assinadas na Microsoft Store / instaladores oficiais ou compilações auditadas a partir do código-fonte no GitHub, mantendo o sistema operacional e os utilitários `7-Zip`/`WinRAR` do sistema sempre atualizados.

---

## 4. Distinção Entre Segurança de Execução Local e Garantia de Dados ("AS IS")

Este modelo de segurança descreve as barreiras implementadas na operação local do software e nos scripts de distribuição. Conforme detalhado em [`README.md`](../README.md#ptbr) e no [EULA Oficial](../source/assets/EULA/EULA_pt_BR%20-%20Compression%20Manager.txt), o aplicativo é gratuito e fornecido **"NO ESTADO EM QUE SE ENCONTRA" ("AS IS")**, mantendo o usuário integralmente responsável pela conferência dos pacotes gerados e pela manutenção de backups independentes dos seus arquivos críticos.

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details open>
<summary>Click to expand / collapse security model in English</summary><br>

# Compression Manager — Security Model, Data Integrity & Resilience

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  
**Author:** Fernando Nillsson Cidade  

---

## 1. Security Principles & Design Philosophy

**Compression Manager** is engineered to run strictly locally over user-controlled files and directories on Windows 10/11 x64 workstations. Its security architecture rests on five structural practices:

1. **Data Sovereignty & 100% Local-First / Air-Gapped Execution:** Zero network connections, zero telemetry, zero cloud sync, and zero user accounts; lean distributions explicitly exclude `Qt6Network.dll` as well as `networkinformation` and `tls` plugins.
2. **Strict Guardrails Against Key/Artifact Leakage (`rarreg.key`):** Packaging personal third-party registration keys (`plugins/WinRAR/rarreg.key`) is blocked by three redundant gates (`cmake/copy_lean_compression_engines.cmake`, `cmake/gerar_distribuicoes_limpas.ps1`, and explicit `#error` preprocessor checks in every `library/Compression_Manager*.iss` installer script).
3. **Strict Engine & Edition Isolation (`Full` vs. `Standart`):** In the Standart Edition (`ENABLE_RAR_SUPPORT=0`), zero WinRAR code paths, icons, or binaries are compiled or deployed; both `7zG.exe`/`7z.exe` and `WinRAR.exe` (Full only) run strictly as local subprocesses.
4. **App-Local Runtime Isolation & Binary Version Verification:** MSVC builds ship only the 5 app-local `Microsoft.VC145.CRT` DLLs without bundling the full `vc_redist.x64.exe` installer, while MinGW-w64 `15.1.0` builds overwrite any older GCC DLLs copied from Qt's bin directory, verifying `Qt6Core.dll == 6.12.0.0` and blocking runtime cross-contamination prior to Inno Setup packaging.
5. **Memory-Safe Qt Patterns & Scoped UIPI Handling:** Systematic use of `std::unique_ptr` and `QPointer` (`layouts_compressao`, `gerenciador_traducao`, `dialog_manual`, `dialog_sobre`), startup privilege diagnostics, and tightly scoped `WM_DROPFILES` handling when running elevated.

---

## 2. Attack Surface & Threat Model

| Potential Threat Vector | Risk Level | Implemented Mitigation / Technical Observation |
|---|:---:|---|
| **Proprietary Key Leakage (`rarreg.key`)** | None / Blocked | Triple-blocked in CMake, PowerShell, and Inno Setup (`#error` if present); only `WinRAR.exe` and `License.txt` are permitted in Full Edition. |
| **Hidden Data Exfiltration / Telemetry** | None | Zero socket or HTTP(S) usage; `Qt6Network` and `tls` pruned from final deployments; `config.json`, `language.json`, and `logs\` reside solely under `%LOCALAPPDATA%\CompressionManager\`. |
| **Malformed Archives / Zip Slip / Decompression Bombs** | Medium (Mitigated via 7-Zip / WinRAR + pre-test) | Extraction delegates to current `7zG.exe`/`7z.exe` engines; users are advised to run **Test Integrity** (`t`) and verify untrusted archives prior to extraction (note that direct extraction invokes `-y` on 7-Zip). |
| **Subprocess Command Construction (`QProcess`)** | Low / Hardening Planned | `mtcomp_02_motoresCompressao.cpp` currently invokes `QProcess::startCommand()` with quoted string formatting. As tracked in Issue 2 of [`docs/suggested-issues.md`](suggested-issues.md#enus), migrating all engine calls to explicit `QProcess::start(program, QStringList args)` is planned for structured argument safety. |
| **Elevated Drag & Drop (Scoped UIPI Bridge)** | Low | `ui_03_dragDrop.cpp` calls `ChangeWindowMessageFilterEx(MSGFLT_ALLOW)` strictly for drag-and-drop file messages (`WM_DROPFILES`, `WM_COPYDATA`, `WM_COPYGLOBALDATA`) and only when `processo_esta_elevado()` is true, accepting local filesystem paths via `DragQueryFileW`. |
| **MSVC / MinGW Runtime Mixing or Search-Path Hijacking** | Low | Adjacent `qt.conf` anchors Qt plugin/translation paths locally (`Prefix = .`); `gerar_distribuicoes_limpas.ps1` and `library/*.iss` prohibit mixing `msvc*140` and `libgcc_s_seh-1`/`libstdc++-6` DLLs. |
| **Local Persistence & Log Hygiene (`%LOCALAPPDATA%`)** | Low | `LogManager` writes diagnostic timestamps/status messages to `%LOCALAPPDATA%\CompressionManager\logs\file_compression_yyyyMMdd_hhmmss.log` without passwords or file payloads; bounded log retention and atomic `QSaveFile` config writes are tracked on the roadmap ([`docs/suggested-issues.md`](suggested-issues.md#enus)). |

---

## 3. Recommended Operational Best Practices

- **Run with Standard User Privileges:** Compression Manager does not require Administrator rights for normal archiving/extraction; only run elevated when writing output archives directly into OS-protected directories.
- **Check External Archives with `Test Integrity`:** Run checksum verification before extracting large or untrusted third-party bundles.
- **Keep Engines Updated & Verify Sources:** Prefer official signed Microsoft Store / author installer releases or audited source builds from GitHub, keeping system `7-Zip` / `WinRAR` installations patched.

---

## 4. Software Security vs. Backup Responsibility ("AS IS")

This model documents runtime software security and packaging hygiene controls. As set forth in [`README.md`](../README.md#enus) and the [Official EULA](../source/assets/EULA/EULA_en_US%20-%20Compression%20Manager.txt), the free utility is supplied **"AS IS"** without warranties, leaving final data stewardship and independent backup redundancy upon the user.

</details>
