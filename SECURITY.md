# Política de Segurança / Security Policy — Compression Manager

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details open>
<summary>Clique para expandir / recolher a política em português</summary><br>

# Política de Segurança

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  

## 1. Versões com Suporte Oficial

Correções de segurança são direcionadas às versões mais recentes:

| Versão | Suportada |
|:---:|:---:|
| `v2026.11.x` | :white_check_mark: |
| `< v2026.11` | :x: |

---

## 2. Como Relatar uma Vulnerabilidade

Levamos a integridade dos dados locais, a segurança no acionamento dos motores externos e a higiene de distribuição do **Compression Manager** com atenção rigorosa.

Caso identifique uma potencial vulnerabilidade de segurança (por exemplo, injeção de argumentos em chamadas `QProcess`, falha de travessia de diretórios em manipulação de pacotes, vazamento indevido em empacotamento, erro de memória em diálogos Qt ou contorno indevido no filtro nativo `WM_DROPFILES`):

1. **NÃO divulgue publicamente a vulnerabilidade em issues ou discussões abertas.**
2. Envie o relato de forma privada por **GitHub Security Advisories** na aba [Security](https://github.com/fernandoncidade/Compression_Manager_cpp/security/advisories/new) ou contate diretamente o mantenedor pelo e-mail `linceu_lighthouse@outlook.com`.
3. Inclua no relato:
   - Descrição detalhada da vulnerabilidade e componente afetado;
   - Edição (`Full` ou `Standart`) e toolchain (`MSVC` ou `MinGW`);
   - Passo a passo mínimo para reprodução;
   - Impacto potencial e sugestões de correção ou mitigação, se houver.

### Processo de Resposta
- O mantenedor envidará os melhores esforços para analisar e responder em tempo hábil, considerada a natureza voluntária do projeto.
- Após avaliação e correção, uma nova compilação/lançamento de segurança será publicado com os devidos créditos ao pesquisador, quando aplicável.

---

## 3. Garantias e Controles Arquiteturais de Segurança

- **100% Local-First e Offline:** O aplicativo não abre sockets, não inicia serviços em segundo plano e não envia telemetria nem nomes de arquivos pela rede; inclusive `Qt6Network.dll` e plugins `tls`/`networkinformation` são excluídos das distribuições enxutas.
- **Proteção Tripla contra Vazamento de Chaves (`rarreg.key`):** Arquivos de registro pessoal do WinRAR são bloqueados em `cmake/copy_lean_compression_engines.cmake`, `cmake/gerar_distribuicoes_limpas.ps1` e por erro fatal de compilação `#error` nos três scripts `library/Compression_Manager*.iss`.
- **Segregação Total entre Edições e Toolchains:** A Edição Standart (`ENABLE_RAR_SUPPORT=0`) exclui qualquer referência a RAR/WinRAR; distribuições MSVC utilizam CRT *app-local* (`Microsoft.VC145.CRT`) isolado de DLLs MinGW, enquanto distribuições MinGW implantam exclusivamente runtime GCC `15.1.0` com verificação de integridade de pacotes e versão exata `Qt6Core.dll == 6.12.0.0`.
- **Ancoragem Local de Plugins Qt (`qt.conf`):** Impede busca acidental de plugins em diretórios externos arbitrários durante execuções diretas.
- **Ponte Controlada para Drag & Drop UIPI (`ui_03_dragDrop.cpp`):** Mensagens `WM_DROPFILES` só têm filtro relaxado via `ChangeWindowMessageFilterEx(MSGFLT_ALLOW)` quando o processo já está elevado, processando exclusivamente caminhos de arquivos locais válidos.
- **Persistência Estritamente Local:** Preferências e logs permanecem em `%LOCALAPPDATA%\CompressionManager\` (`config.json`, `language.json` e `logs\file_compression_*.log`). *(Veja também o modelo completo e pontos de endurecimento planejados em [`docs/security-model.md`](docs/security-model.md#ptbr).)*

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details open>
<summary>Click to expand / collapse the policy in English</summary><br>

# Security Policy

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  

## 1. Supported Versions

Security updates target the latest release series:

| Version | Supported |
|:---:|:---:|
| `v2026.11.x` | :white_check_mark: |
| `< v2026.11` | :x: |

---

## 2. Reporting a Vulnerability

We take local data integrity, external process invocation safety, and packaging hygiene in **Compression Manager** seriously.

If you believe you have discovered a security vulnerability (for example, `QProcess` argument injection, path handling defects during archive processing, unintended artifact bundling, memory safety issues, or abuse of the `WM_DROPFILES` native event filter):

1. **Do NOT disclose security vulnerabilities through public GitHub issues or discussions.**
2. Report privately via **GitHub Security Advisories** under the repository's [Security tab](https://github.com/fernandoncidade/Compression_Manager_cpp/security/advisories/new) or contact the maintainer directly at `linceu_lighthouse@outlook.com`.
3. Please include:
   - Detailed description of the issue and affected source module;
   - Product edition (`Full` or `Standart`) and toolchain (`MSVC` or `MinGW`);
   - Minimal reproduction steps;
   - Potential impact and suggested remediation, if available.

### Response Process
- The maintainer will make best efforts to review and acknowledge reports promptly, respecting the voluntary maintenance model of the project.
- After assessment and remediation, an updated release will be published with appropriate reporter credit.

---

## 3. Core Architectural Security Controls

- **100% Local-First & Offline:** Zero network sockets, zero background services, zero telemetry, and zero file-path uploads; lean distributions exclude `Qt6Network.dll` as well as `tls` and `networkinformation` plugins.
- **Triple Guardrails Against Key Leakage (`rarreg.key`):** Personal registration keys are stripped and explicitly blocked across `cmake/copy_lean_compression_engines.cmake`, `cmake/gerar_distribuicoes_limpas.ps1`, and fatal `#error` checks in every `library/Compression_Manager*.iss` script.
- **Strict Edition & Runtime Isolation:** Standart Edition (`ENABLE_RAR_SUPPORT=0`) compiles with zero RAR/WinRAR code or assets; MSVC deployments use app-local `Microsoft.VC145.CRT` DLLs isolated from MinGW libraries, while MinGW deployments ship GCC `15.1.0` runtime DLLs with `Qt6Core.dll == 6.12.0.0` verification.
- **Local Plugin Anchoring (`qt.conf`):** Prevents unintended plugin lookup outside the application deployment directory.
- **Scoped Elevated Drag & Drop (`ui_03_dragDrop.cpp`):** `ChangeWindowMessageFilterEx(MSGFLT_ALLOW)` is applied solely for `WM_DROPFILES`, `WM_COPYDATA`, and `WM_COPYGLOBALDATA` when running elevated, parsing local filesystem drop paths via `DragQueryFileW`.
- **Strictly Local Preferences & Logs:** Settings and logs stay under `%LOCALAPPDATA%\CompressionManager\` (`config.json`, `language.json`, and `logs\file_compression_*.log`). *(See [`docs/security-model.md`](docs/security-model.md#enus) for complete details and planned hardenings.)*

</details>
