# Licenciamento e Esclarecimento Comercial / Licensing & Commercial Model — Compression Manager

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details open>
<summary>Clique para expandir / recolher o documento de licenciamento em português</summary><br>

# Compression Manager — Licenciamento, Termos de Código Aberto e Esclarecimento Comercial

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  
**Licença do Código-Fonte:** GNU Affero General Public License v3 (GNU AGPLv3)  
**Autor e Detentor dos Direitos Autorais:** Fernando Nillsson Cidade  

---

## 1. Visão Geral do Modelo de Duplo Licenciamento

O **Compression Manager** adota um modelo aberto transparente que concilia a liberdade do ecossistema de software livre copyleft forte com a distribuição gratuita oficial assinada na Microsoft Store e via instaladores Windows:

| Componente | Licença / Instrumento | Usos Permitidos e Condições |
|---|---|---|
| **Código-Fonte Original (Repositório Completo)** | [GNU AGPLv3](../LICENSE) | 100% Livre: uso, estudo, auditoria, compilação (Edição Full e Edição Standart, MSVC ou MinGW), modificação e redistribuição sob termos copyleft da GNU AGPLv3. |
| **Documentação Institucional em `source/assets/`** | Autoria do Projeto (Uso Livre para Desenvolvedores) | Elaborada para conformidade com Microsoft Store e instaladores; desenvolvedores e forks têm liberdade total para alterar, excluir, adicionar e adaptar em suas próprias distribuições. |
| **Binários Compilados por Terceiros (via Código)** | GNU AGPLv3 | Livres para compilar, empacotar e redistribuir sob responsabilidade do desenvolvedor/distribuidor, cumprindo integralmente os termos da GNU AGPLv3 e as atribuições de terceiros. |
| **Binário Oficial Assinado do Autor (Microsoft Store / Instaladores)** | [EULA Oficial](../source/assets/EULA/EULA_pt_BR%20-%20Compression%20Manager.txt) + [CLC](../source/assets/CLC/CLC_pt_BR%20-%20Compression%20Manager.txt) | Distribuição gratuita oficial pelo autor (ou comercial quando formalizada via CLC). Vedada a revenda comercial paga por terceiros, republicação não autorizada ou apropriação de marca. |
| **Framework Qt 6.12.0** | [GNU LGPLv3](../source/assets/LICENSES/GNU%20LESSER%20GENERAL%20PUBLIC%20LICENSE%20Version%203.txt) | Linkagem dinâmica (`Qt6Core.dll`, `Qt6Gui.dll`, `Qt6Widgets.dll`), preservando integralmente o direito do usuário de substituir bibliotecas compatíveis. |
| **Motor 7-Zip (Full e Standart)** | GNU LGPL v2.1+ / BSD-2 / BSD-3 + restrição unRAR ([`License.txt`](../source/assets/LICENSES/License.txt)) | Binários `7zG.exe`, `7z.exe` e `7z.dll` acompanhados de documentação e licença original do projeto 7-Zip. |
| **Motor WinRAR (Apenas Edição Full)** | Licença Proprietária (win.rar GmbH / Alexander Roshal) | Orquestração local externa sem inclusão de `rarreg.key`. Estritamente ausente na **Edição Standart** (`ENABLE_RAR_SUPPORT=0`). |

---

## 2. Direitos de Código Aberto (GNU AGPLv3)

Todo o código-fonte autoral do Compression Manager em **C++23** e **Qt 6.12.0** — abrangendo a interface gráfica (`InterfaceCore`, `GerenciamentoUI`), a orquestração assíncrona dos motores (`MotoresCompressao`), traduções (`language`), utilitários (`utils`), ferramentas auxiliares (`mocks`), suíte de testes (`tests/`), scripts CMake (`cmake/`) e instaladores Inno Setup (`library/`) — está licenciado sob a **GNU Affero General Public License v3 (GNU AGPLv3)**:

```text
Compression Manager — Multiformat File Archiver and Compression Suite
Copyright (c) 2025-2026 Fernando Nillsson Cidade

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Affero General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>.
```

---

## 3. Documentação em `source/assets/` e Liberdade Total para Desenvolvedores

A documentação institucional e regulatória mantida no diretório `source/assets/` (abrangendo `ABOUT`, `CLC`, `COPYRIGHT`, `EULA`, `LICENSES`, `NOTICES` e `PRIVACY_POLICY`) possui finalidade e autorização específicas:

1. **Finalidade Original:** Elaborada por Fernando Nillsson Cidade para atender integralmente às exigências de conformidade, privacidade, acessibilidade e publicação oficial na **Microsoft Store** e nos instaladores desktop Windows (`Inno Setup 7` / `MSIX`).
2. **Autorização Expressa para Desenvolvedores:** Conforme registrado em [`AVISO DE COPYRIGHT E MARCA REGISTRA_pt_BR.txt`](../source/assets/COPYRIGHT/AVISO%20DE%20COPYRIGHT%20E%20MARCA%20REGISTRA_pt_BR.txt), o autor **autoriza expressamente e concede liberdade total** para que desenvolvedores, colaboradores e forks realizem quaisquer alterações, exclusões, adições ou modificações gerais nesses arquivos em suas próprias compilações.
3. **Condição de Responsabilidade:** Forks e redistribuições próprias devem observar as normas, licenças de componentes de terceiros e legislação vigente em seus respectivos países e plataformas de publicação.

---

## 4. Segregação de Edições (Full vs. Standart) e Licenciamento de Motores

A escolha entre as edições impacta diretamente os requisitos de conformidade em redistribuições binárias:

1. **Edição Standart (`ENABLE_RAR_SUPPORT=OFF` / `0`):**
   - Inclui apenas o motor aberto **7-Zip** (`7zG.exe`, `7z.exe`, `7z.dll`) e suporte aos formatos **ZIP**, **7Z**, **TAR** e **WIM** (além de extração multiformato).
   - **Zero** referência ou arquivo do **WinRAR** (nem `WinRAR.exe`, nem `winrar3.png`), tornando esta edição especialmente indicada para organizações e pacotes corporativos que exigem conformidade estrita livre de componentes proprietários externos de terceiros.
2. **Edição Full (`ENABLE_RAR_SUPPORT=ON` / `1`):**
   - Adiciona integração externa com **WinRAR** (`WinRAR.exe`) para criação e gerenciamento de arquivos proprietários **RAR**.
   - Por política estrita de segurança e licenciamento, o projeto **jamais empacota `rarreg.key`** nem arquivos auxiliares proprietários desnecessários (`*.chm`, extensões de shell ou desinstaladores do WinRAR), mantendo bloqueios explícitos tanto em [`copy_lean_compression_engines.cmake`](../cmake/copy_lean_compression_engines.cmake) quanto nos scripts Inno Setup (`library/Compression_Manager*.iss`).

---

## 5. Checklist de Conformidade e Submissão Microsoft Store (MSIX / Partner Center)

Este checklist consolida o mapeamento documental compatível com o manifesto [`library/AppxManifest.xml`](../library/AppxManifest.xml) e as **Microsoft Store Policies v7.20**:

| Item do Partner Center | Status Documental / Referência no Repositório | Observação Operacional |
|---|---|---|
| **Identidade MSIX** | `fernandoncidade.COMPRESSIONMANAGER` (`x64`) | Declarada em [`library/AppxManifest.xml`](../library/AppxManifest.xml) com publisher `CN=F1FFF36E-09E6-42F5-BAE5-3124C3054234`. |
| **Categoria (Obrigatório)** | Sugerido: *Utilitários e ferramentas* | Configurar na seção *Propriedades*. |
| **URL da Política de Privacidade (Obrigatório para `runFullTrust`)** | [`docs/privacy-policy.md`](privacy-policy.md#ptbr) e [`source/assets/PRIVACY_POLICY/Privacy_Policy_pt_BR.txt`](../source/assets/PRIVACY_POLICY/Privacy_Policy_pt_BR.txt) | Aplicativos Win32/`runFullTrust` exigem política própria (Política 10.5.1). **A Microsoft não fornece política padrão e não cobre este aplicativo sob a Privacy Statement da Microsoft.** |
| **Site Oficial (Recomendado)** | `https://github.com/fernandoncidade/Compression_Manager_cpp` | Aponta para o repositório oficial do desenvolvedor (distinto da página da Store). |
| **Suporte ao Cliente (Recomendado)** | `linceu_lighthouse@outlook.com` / GitHub Issues | **A Microsoft não presta suporte técnico ao cliente**; suporte voluntário best-effort pelo autor. |
| **Capacidade Restrita (`runFullTrust`)** | Declarada em [`library/AppxManifest.xml`](../library/AppxManifest.xml) | Necessária para aplicativo desktop nativo C++23 / Qt 6.12.0 e execução local dos binários `7zG.exe`/`7z.exe` (e `WinRAR.exe` na Full). |
| **Desinstalação Limpa (Política 10.2.7)** | Documentada em `README.md`, `privacy-policy.md` e `EULA` | Remoção limpa por Configurações do Windows / Menu Iniciar sem serviços em segundo plano. |
| **Listagens da Store (`pt-BR` e `en-US`)** | Diretório gráfico [`source/assets/icones/`](../source/assets/icones) + capturas em [`images/interfaces/`](../images/interfaces/pt_BR) | Fornecer descrição precisa das funcionalidades da edição empacotada (Standart ou Full), logotipos e capturas de tela da execução real. |

### Textos Prontos para Preenchimento no Partner Center

- **Justificativa para Capacidade Restrita (`runFullTrust` — Notes for Certification):**
  > *Compression Manager is a native Win32 C++23 / Qt 6.12.0 desktop file archiving, extraction, and checksum verification application using standard Windows file/folder pickers and local subprocess execution of bundled 7-Zip (and WinRAR in the Full edition) tools on user-selected paths. It operates 100% offline with zero telemetry, zero background services, and zero network requests.*

- **Direitos Autorais e Marcas Registradas (`pt-BR`, máx. 200 caracteres):**
  > `© 2025-2026 Fernando Nillsson Cidade. Código-fonte sob GNU AGPLv3; pacote oficial Microsoft Store sob EULA do autor. Qt (LGPLv3) e 7-Zip (LGPL/BSD).` *(149 caracteres)*

---

## 6. Esclarecimento Comercial (CLC), Uso Auxiliar e Isenção ("AS IS")

- **Licenciamento Comercial Sob Medida:** Empresas ou instituições que desejem exceção proprietária às cláusulas copyleft da GNU AGPLv3 para integração fechada do código-fonte autoral podem consultar condições comerciais diretamente pelo e-mail `linceu_lighthouse@outlook.com` (consulte [`CLC_pt_BR - Compression Manager.txt`](../source/assets/CLC/CLC_pt_BR%20-%20Compression%20Manager.txt)). Componentes de terceiros (Qt LGPLv3, 7-Zip, WinRAR) permanecem regidos por suas licenças de origem.
- **Escopo Auxiliar e Backup ("AS IS"):** O software gratuito é fornecido **"NO ESTADO EM QUE SE ENCONTRA" ("AS IS")**, sem garantias explícitas ou implícitas ([EULA Oficial](../source/assets/EULA/EULA_pt_BR%20-%20Compression%20Manager.txt), Lei nº 9.609/1998 e Seções 15–16 da [GNU AGPLv3](../LICENSE)). Recomenda-se sempre executar o teste de integridade integrado e manter cópias de segurança independentes para arquivos críticos.

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details open>
<summary>Click to expand / collapse licensing in English</summary><br>

# Compression Manager — Licensing, Open Source Terms & Commercial Clarification

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  
**Source Code License:** GNU Affero General Public License v3 (GNU AGPLv3)  
**Author and Copyright Holder:** Fernando Nillsson Cidade  

---

## 1. Dual-Licensing Architecture Overview

**Compression Manager** adopts a transparent open dual-licensing model that combines strong-copyleft open-source freedom on GitHub with official signed binary distribution on the Microsoft Store and Windows installers:

| Component | License / Instrument | Permitted Uses & Conditions |
|---|---|---|
| **Original Source Code (Full Repository)** | [GNU AGPLv3](../LICENSE) | 100% Free: use, study, inspect, compile (Full & Standart editions, MSVC or MinGW), modify, and redistribute under GNU AGPLv3 copyleft terms. |
| **Institutional Documentation in `source/assets/`** | Project Authorship (Free for Developers) | Created for Microsoft Store and installer compliance; developers and forks have complete freedom to alter, remove, add, and adapt them for their own distributions. |
| **Third-Party Builds Compiled from Source** | GNU AGPLv3 | Free to compile, package, and redistribute under the builder's own responsibility while complying with GNU AGPLv3 and third-party license attributions. |
| **Author's Official Signed Binary (Microsoft Store / Installers)** | [Official EULA](../source/assets/EULA/EULA_en_US%20-%20Compression%20Manager.txt) + [CLC](../source/assets/CLC/CLC_en_US%20-%20Compression%20Manager.txt) | Official free distribution by the author (or commercial when explicitly executed under a signed CLC agreement). Paid third-party resale or trademark misappropriation is prohibited. |
| **Qt 6.12.0 Framework** | [GNU LGPLv3](../source/assets/LICENSES/GNU%20LESSER%20GENERAL%20PUBLIC%20LICENSE%20Version%203.txt) | Dynamically linked (`Qt6Core.dll`, `Qt6Gui.dll`, `Qt6Widgets.dll`), preserving users' rights to replace compatible shared Qt libraries. |
| **7-Zip Engine (Full & Standart)** | GNU LGPL v2.1+ / BSD-2 / BSD-3 + unRAR restriction ([`License.txt`](../source/assets/LICENSES/License.txt)) | Binaries `7zG.exe`, `7z.exe`, and `7z.dll` distributed with complete upstream 7-Zip license documentation. |
| **WinRAR Engine (Full Edition Only)** | Proprietary License (win.rar GmbH / Alexander Roshal) | Local external orchestration without bundling `rarreg.key`. Strictly excluded from the **Standart Edition** (`ENABLE_RAR_SUPPORT=0`). |

---

## 2. Open Source Rights (GNU AGPLv3)

All original Compression Manager source code in **C++23** and **Qt 6.12.0** — including GUI layers (`InterfaceCore`, `GerenciamentoUI`), asynchronous compression orchestration (`MotoresCompressao`), translations (`language`), utilities (`utils`), developer tooling (`mocks`), test suite (`tests/`), CMake scripts (`cmake/`), and Inno Setup scripts (`library/`) — is licensed under the **GNU Affero General Public License v3 (GNU AGPLv3)**:

```text
Compression Manager — Multiformat File Archiver and Compression Suite
Copyright (c) 2025-2026 Fernando Nillsson Cidade

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Affero General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>.
```

---

## 3. Documentation in `source/assets/` and Full Freedom for Developers

Institutional and compliance documentation stored in `source/assets/` (`ABOUT`, `CLC`, `COPYRIGHT`, `EULA`, `LICENSES`, `NOTICES`, and `PRIVACY_POLICY`) carries clear permissions:

1. **Original Purpose:** Authored by Fernando Nillsson Cidade to satisfy official Microsoft Store publishing, privacy, accessibility, and Windows desktop installer compliance (`Inno Setup 7` / `MSIX`).
2. **Explicit Authorization for Developers:** As stated in [`COPYRIGHT AND TRADEMARK NOTICE_en_US.txt`](../source/assets/COPYRIGHT/COPYRIGHT%20AND%20TRADEMARK%20NOTICE_en_US.txt), the author **expressly authorizes and grants full freedom** for developers, contributors, and forks to modify, delete, add, or adapt these files in their own builds and repositories.
3. **Responsibility Condition:** Independent forks and distributions must comply with third-party component licenses and applicable laws in their target jurisdictions and platforms.

---

## 4. Edition Segregation (Full vs. Standart) & Engine Licensing

Build edition selection directly governs external engine dependencies in binary redistributions:

1. **Standart Edition (`ENABLE_RAR_SUPPORT=OFF` / `0`):**
   - Ships solely the open **7-Zip** engine (`7zG.exe`, `7z.exe`, `7z.dll`) supporting **ZIP**, **7Z**, **TAR**, and **WIM** archiving (plus wide open-format extraction).
   - **Zero** **WinRAR** binaries, icons (`winrar3.png`), or RAR references — recommended for organizations requiring strict exclusion of third-party proprietary archiving binaries.
2. **Full Edition (`ENABLE_RAR_SUPPORT=ON` / `1`):**
   - Adds external **WinRAR** (`WinRAR.exe`) integration for creating and updating proprietary **RAR** archives.
   - Under strict security and licensing hygiene, the project **never bundles `rarreg.key`** or extraneous WinRAR helper binaries (`*.chm`, shell extensions, uninstallers), enforced in both [`copy_lean_compression_engines.cmake`](../cmake/copy_lean_compression_engines.cmake) and the Inno Setup scripts (`library/Compression_Manager*.iss`).

---

## 5. Microsoft Store MSIX Submission, Privacy & Support Checklist

This checklist maps the documentation requirements aligned with [`library/AppxManifest.xml`](../library/AppxManifest.xml) and **Microsoft Store Policies v7.20**:

| Partner Center Field | Repository Reference / Status | Operational Notes |
|---|---|---|
| **MSIX Package Identity** | `fernandoncidade.COMPRESSIONMANAGER` (`x64`) | Declared in [`library/AppxManifest.xml`](../library/AppxManifest.xml) with publisher `CN=F1FFF36E-09E6-42F5-BAE5-3124C3054234`. |
| **Category (Required)** | Suggested: *Utilities & tools* | Configure under *Properties*. |
| **Privacy Policy URL (Required for `runFullTrust`)** | [`docs/privacy-policy.md`](privacy-policy.md#enus) and [`source/assets/PRIVACY_POLICY/Privacy_Policy_en_US.txt`](../source/assets/PRIVACY_POLICY/Privacy_Policy_en_US.txt) | Win32/`runFullTrust` apps require an explicit privacy policy (Policy 10.5.1). **Microsoft does not provide a default privacy policy and does not cover this app under Microsoft's Privacy Statement.** |
| **Website (Recommended)** | `https://github.com/fernandoncidade/Compression_Manager_cpp` | Points to the developer's repository/site — never the Store listing page. |
| **Customer Support (Recommended)** | `linceu_lighthouse@outlook.com` / GitHub Issues | **Microsoft does not provide customer support for this app**; voluntary best-effort support by the author. |
| **Restricted Capabilities (`runFullTrust`)** | Declared in [`library/AppxManifest.xml`](../library/AppxManifest.xml) | Required for native C++23 / Qt 6.12.0 desktop execution and local invocation of bundled `7zG.exe`/`7z.exe` (plus `WinRAR.exe` in Full). |
| **Clean Uninstall (Policy 10.2.7)** | Stated in `README.md`, `privacy-policy.md`, and `EULA` | Clean removal via Windows Settings / Start Menu without residual background services. |
| **Store Listings (`pt-BR` and `en-US`)** | Visual icons in [`source/assets/icones/`](../source/assets/icones) + screenshots in [`images/interfaces/`](../images/interfaces/en_US) | Ensure description accurately reflects the submitted edition (Standart or Full), plus localized screenshots. |

### Ready-to-Paste Texts for Partner Center

- **Restricted Capability Justification (`runFullTrust` — Notes for Certification):**
  > *Compression Manager is a native Win32 C++23 / Qt 6.12.0 desktop file archiving, extraction, and checksum verification application using standard Windows file/folder pickers and local subprocess execution of bundled 7-Zip (and WinRAR in the Full edition) tools on user-selected paths. It operates 100% offline with zero telemetry, zero background services, and zero network requests.*

- **Copyright & Trademark Info (`en-US`, max. 200 chars):**
  > `© 2025-2026 Fernando Nillsson Cidade. Open source under GNU AGPLv3; official Microsoft Store binary under author EULA. Qt (LGPLv3), 7-Zip (LGPL/BSD).` *(146 chars)*

---

## 6. Commercial Licensing Clarification (CLC), Auxiliary Scope & Disclaimer ("AS IS")

- **Custom Commercial Licensing:** Organizations or integrators seeking a proprietary exemption from GNU AGPLv3 copyleft terms for closed-source distribution of original Compression Manager code may contact `linceu_lighthouse@outlook.com` (see [`CLC_en_US - Compression Manager.txt`](../source/assets/CLC/CLC_en_US%20-%20Compression%20Manager.txt)). Third-party tools (Qt LGPLv3, 7-Zip, WinRAR) remain subject to their upstream terms.
- **Free Auxiliary Scope & Backups ("AS IS"):** Supplied **"AS IS"** without warranties ([Official EULA](../source/assets/EULA/EULA_en_US%20-%20Compression%20Manager.txt), Brazilian Law No. 9.609/1998, and [GNU AGPLv3](../LICENSE) Sections 15–16). Always run the built-in integrity check and maintain independent backups for important files.

</details>
