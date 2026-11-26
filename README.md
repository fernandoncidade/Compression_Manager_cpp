<!-- Multilanguage README.md for Compression Manager -->

# Compression Manager — Gerenciador e Compactador de Arquivos Multiformato (Full & Standart)

**Utilitário desktop nativo de alto desempenho para Windows (Windows 10 `17763+` / Windows 11 x64), desenvolvido em C++23 moderno e Qt 6.12.0 Widgets para empacotamento, extração, atualização e teste de integridade de arquivos e pastas. Disponibilizado em duas edições estritamente segregadas — Edição Full (RAR, ZIP, 7Z, TAR, WIM via 7-Zip + WinRAR) e Edição Standart (formatos abertos ZIP, 7Z, TAR, WIM via 7-Zip puro) — com Layout Duplo paralelo, operação 100% offline, distribuições ultralimpas MSVC/MinGW sem `vc_redist.x64.exe` e modelo transparente de Duplo Licenciamento (`pt-BR` / `en-US`).**

[![Latest Release](https://img.shields.io/badge/Release-v2026.11.22.0-blue.svg)](https://github.com/fernandoncidade/Compression_Manager_cpp/releases)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-00599C?logo=c%2B%2B)](CMakeLists.txt)
[![Qt 6.12.0](https://img.shields.io/badge/Qt-6.12.0-41CD52?logo=qt)](CMakeLists.txt)
[![Toolchains](https://img.shields.io/badge/Toolchains-MSVC_14.52_%7C_MinGW_15.1.0-0078D4)](CMakePresets.json)
[![Editions](https://img.shields.io/badge/Editions-Full_%26_Standart-2ea44f.svg)](docs/compliance.md)
[![Platform Windows](https://img.shields.io/badge/Platform-Windows_11_%2F_10_x64-0078D4?logo=windows)](library/AppxManifest.xml)
[![License GNU AGPLv3](https://img.shields.io/badge/License-GNU_AGPLv3-blue.svg)](LICENSE)
[![Official Store EULA](https://img.shields.io/badge/License-Official_Store_EULA_%2B_CLC-purple.svg)](docs/licensing.md)
[![100% Offline](https://img.shields.io/badge/Security-100%25_Offline-success.svg)](docs/security-model.md)

<p align="center">
  <a href="docs/user-guide.md">📖 Guia do Usuário / User Guide</a> •
  <a href="docs/architecture.md">🏗️ Arquitetura / Architecture</a> •
  <a href="docs/compliance.md">📋 Conformidade / Compliance</a> •
  <a href="docs/security-model.md">🛡️ Privacidade &amp; Segurança</a> •
  <a href="docs/licensing.md">⚖️ Duplo Licenciamento / Dual Licensing</a> •
  <a href="docs/technical-audit-and-roadmap.md">🔬 Auditoria &amp; Roadmap</a><br>
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> •
  <a href="#enus">🇺🇸 English (US)</a>
</p>

---

<p align="center">
  <img src="docs/images/banner.png" alt="Compression Manager Banner" width="360">
</p>

---

## <a id="ptbr"></a><a id="português-br"></a>🇧🇷 Português (BR)

> **Observação:** Este repositório refere-se à versão oficial **v2026.11.22.0** do projeto **Compression Manager** (C++23, Qt 6.12.0, MSVC x64 `14.52.37025` / Windows SDK `10.0.28000.0` e MinGW-w64 `15.1.0`), disponibilizado sob arquitetura transparente de **Duplo Licenciamento** (**GNU AGPLv3** para o código-fonte público no GitHub e **EULA Oficial + CLC** para pacotes binários oficiais assinados na Microsoft Store e instaladores do autor, com execução 100% local e offline).

<details>
<summary><b>Clique para expandir / recolher a documentação em Português (BR)</b></summary><br>

# Compression Manager — Gerenciador e Compactador de Arquivos Multiformato

Versão: v2026.11.22.0<br>
Data técnica desta revisão: 22 de novembro de 2026<br>
Autor: Fernando Nillsson Cidade<br>

Esta versão oficial **2026.11.22.0** consolida a estabilidade definitiva do Layout Duplo paralelo, a modernização integral da toolchain para **C++23** e **Qt 6.12.0**, distribuições externas ultralimpas com CRT *app-local* sem `vc_redist.x64.exe` e a arquitetura formal de **Duplo Licenciamento**.

---

## ⚖️ Manifesto e Autorização Legal do Titular dos Direitos Autorais

> **Declaração Expressa de Fernando Nillsson Cidade:**  
> Eu, **Fernando Nillsson Cidade**, na qualidade de único autor, criador original e titular exclusivo dos direitos autorais do projeto **Compression Manager**, **autorizo o uso, estudo, cópia, modificação, compilação e distribuição do código-fonte público deste repositório** nos exatos termos do arquivo **[LICENSE](LICENSE)** (**GNU Affero General Public License v3 — GNU AGPLv3**, identificador SPDX `AGPL-3.0-only`), ressalvada a proteção nominal de nome e marca sob a Seção 7(e) (`"Compression Manager"` / `"COMPRESSION MANAGER"`).  
> Qualquer versão modificada, extensão ou obra derivada do código-fonte público deve preservar a atribuição da autoria original e disponibilizar o código-fonte correspondente sob a mesma licença **GNU AGPLv3**.  
> **Liberdade para Documentação Institucional em `source/assets/`:** Os textos de conformidade contidos em [`source/assets/`](source/assets/COPYRIGHT/AVISO%20DE%20COPYRIGHT%20E%20MARCA%20REGISTRA_pt_BR.txt) (`ABOUT`, `CLC`, `COPYRIGHT`, `EULA`, `LICENSES`, `NOTICES` e `PRIVACY_POLICY`) foram elaborados para atender às exigências de publicação na Microsoft Store e instaladores Windows; o autor **autoriza expressamente** que desenvolvedores e forks alterem, excluam, adicionem ou adaptem tais documentos em suas próprias distribuições, observada a legislação de suas jurisdições e diretrizes de cada plataforma.  
> **Limite do Binário Oficial Assinado (Microsoft Store e Instaladores Oficiais):** A edição empacotada e assinada digitalmente pelo autor é distribuída **gratuitamente** na Microsoft Store e nos canais oficiais do projeto sob o respectivo **[EULA Oficial](source/assets/EULA/EULA_pt_BR%20-%20Compression%20Manager.txt)** (em harmonia com os Termos de Licença Padrão para Aplicativos da Microsoft Store) e **[Esclarecimento Comercial — CLC](source/assets/CLC/CLC_pt_BR%20-%20Compression%20Manager.txt)**. É expressamente vedada a revenda comercial paga, a apropriação indevida da marca ou a republicação não autorizada por terceiros do pacote binário oficial assinado pelo autor.  
> **Canal de Suporte, Independência da Microsoft e Isenção Legal ("AS IS"):** A **Microsoft NÃO presta suporte ao cliente nem cobre este software sob a Política de Privacidade da Microsoft**. Todo eventual suporte técnico do autor é prestado **exclusivamente por e-mail (`linceu_lighthouse@outlook.com`) e pelas abas Issues/Discussions do GitHub**, **em regime voluntário de melhores esforços (best-effort), conforme disponibilidade e sem obrigação de urgência ou SLA**. O aplicativo gratuito é fornecido **"NO ESTADO EM QUE SE ENCONTRA" ("AS IS")**, sem garantias, cabendo ao usuário validar pacotes críticos com o teste de integridade e manter backups independentes. Consulte [`docs/licensing.md`](docs/licensing.md#ptbr).

---

## 📌 Ficha Técnica do Projeto

<p align="center">
  <img src="docs/images/logo.png" alt="Ícone Compression Manager" width="128">
</p>

- **Nome Oficial:** Compression Manager (`Compression_Manager_7Z_RAR` no CMake)
- **Autor e Mantenedor:** Fernando Nillsson Cidade *(Desenvolvedor Individual / Pessoa Física)*
- **E-mail Oficial de Suporte (voluntário / melhores esforços, sem SLA):** `linceu_lighthouse@outlook.com`
- **Repositório Oficial:** [https://github.com/fernandoncidade/Compression_Manager_cpp](https://github.com/fernandoncidade/Compression_Manager_cpp)
- **Identidade MSIX e Capacidades (`library/AppxManifest.xml`):** `fernandoncidade.COMPRESSIONMANAGER` (`x64`) · `<rescap:Capability Name="runFullTrust" />` · `<uap10:Content Enforcement="on" />`
- **Requisitos de Sistema:** Windows 10 (build `17763+`) / Windows 11 x64
- **Idiomas Suportados Dinamicamente em Tempo de Execução:** Português (Brasil — `pt_BR`) · English (United States — `en_US`)
- **Stack Tecnológica Atual:** **C++23**, **Qt 6.12.0** (`Core`, `Gui`, `Widgets`), **MSVC puro x64 `14.52.37025`** (Visual Studio 18 2026 / MSBuild e CRT *app-local* sem `vc_redist.x64.exe`), **MinGW-w64 `15.1.0`** (`mingw1510_64`, Ninja), **CMake `3.28+`**, **Inno Setup 7**
- **Executáveis Gerados pelos Presets (`CMakePresets.json`):**
  - `Compression_Manager.exe`: Aplicativo desktop principal em Edição **Full** (`ENABLE_RAR_SUPPORT=ON`) ou **Standart** (`ENABLE_RAR_SUPPORT=OFF`)
  - `find_VersionEditor.exe`: Utilitário GUI/CLI (`--check`) em `mocks/` para auditoria e sincronização CalVer entre `source/assets/`, `ginter_20_exibir_sobre.cpp`, documentação (`README.md`, `MANUAL.md`, `OBSERVACAO.md`, `docs/*.md`), `library/AppxManifest.xml`, `library/Compression_Manager_cpp.txt` e `library/*.iss`
  - `CompileTranslationsGUI.exe` e `CompileTranslationsTerminal.exe`: Ferramentas em `mocks/` para compilação/validação de catálogos `.ts` → `.qm`

---

## Duas Edições Independentes de Produto: Full e Standart

Originalmente prototipado em Python/PySide6 e integralmente reescrito para **C++23 nativo + Qt 6.12.0**, o **Compression Manager** elimina sobrecarga de interpretador, travamentos de GIL e dependências externas, sendo compilado em duas edições estritamente segregadas via `ENABLE_RAR_SUPPORT` (`CMakeLists.txt`):

### 1. Produto Full (Edição Full — `ENABLE_RAR_SUPPORT=ON`)
- **Formatos de Empacotamento:** **RAR** (`0–5`), **ZIP** (`0, 1, 3, 5, 7, 9`), **7Z** (`0, 1, 3, 5, 7, 9`), **TAR** (`0`) e **WIM** (`0`).
- **Motores Integrados:** **7-Zip** (`7zG.exe` / `7z.exe`) e **WinRAR** (`WinRAR.exe`, sem empacotar `rarreg.key`).
- **Distribuições Externas Ultralimpas:** `Compression_Manager_Full_msvc` (`35,20 MB` / instalador `13,05 MB`) e `Compression_Manager_Full_mingw` (`41,96 MB` / instalador `15,13 MB`).

### 2. Produto Standart (Edição Standart — `ENABLE_RAR_SUPPORT=OFF`)
- **Formatos de Empacotamento:** Exclusivamente formatos abertos — **ZIP**, **7Z**, **TAR** e **WIM**.
- **Motor Integrado e Segregação Total:** Utiliza somente **7-Zip** (`7zG.exe` / `7z.exe`), sem nenhuma referência de código, menu, ícone (`winrar3.png`) ou binário ligado a WinRAR/RAR.
- **Distribuições Externas Ultralimpas:** `Compression_Manager_Standart_msvc` (`31,96 MB` / instalador `11,57 MB`) e `Compression_Manager_Standart_mingw` (`38,72 MB` / instalador `13,63 MB`).

---

## Destaques técnicos da versão 2026.11.22.0

- **Layout Padrão + Segundo Layout Paralelo (Layout Duplo) Estabilizado (`ginter_21_toggle_segundo_layout.cpp`, `ginter_27_adapt_layout_orientation.cpp`):** Divisão em duas colunas independentes de entrada, destinos e fila de execução, com preservação rigorosa do estado maximizado (`isMaximized()`) e reorientação automática horizontal/vertical conforme a proporção da janela.
- **Empacotamento, Atualização e Extração em Segundo Plano (`MotoresCompressao`):** Filas assíncronas via `QThread` + `QProcess` mantendo a interface responsiva, com nível granular por formato, opção *Atualizar arquivos existentes* (`u` vs. `a`) e extração em duas passagens para `.tar.xz`, `.tgz`, `.tar.gz` e `.bz2`.
- **Verificação de Integridade Não Destrutiva (`t`):** Validação bloco a bloco de somas de verificação (`.rar`, `.zip`, `.7z`, `.bz2`, `.tar`, `.wim`, `.tar.xz`, `.zipx`, `.tgz`, `.tar.gz`, `.lzh`, `.iso`) sem gravar arquivos em disco.
- **Drag & Drop com Ponte Nativa UIPI (`ui_03_dragDrop.cpp`):** Arraste pastas e arquivos diretamente do Windows Explorer tanto para a lista de entrada quanto para os quadros de saída por formato, funcionando inclusive em sessões elevadas via filtro `WM_DROPFILES` ajustado por DPI (`devicePixelRatioF()`).
- **Internacionalização ao Vivo (`pt_BR` ↔ `en_US`) e Manual/Sobre Adaptados à Edição:** Troca imediata de idioma sem reiniciar, Manual embutido específico para Full/Standart e diálogo Sobre com busca textual instantânea.
- **Distribuições Enxutas Sem `vc_redist.x64.exe` e Zero Vazamento (`cmake/*.cmake`, `library/*.iss`):** Implantação *app-local* da `Microsoft.VC145.CRT` (MSVC menor que MinGW), exclusão de DLLs Qt não usadas (`Qt6Network`, `Qt6Pdf`, `Qt6Svg`, `Qt6PrintSupport`), remoção de banners/PDFs de loja dos pacotes de runtime e bloqueio absoluto contra `rarreg.key`.

---

## <a id="interface-pt-br"></a>🖼️ Interface Gráfica do Aplicativo (Português — `pt_BR`)

As capturas a seguir demonstram a interface nativa em **Qt 6.12.0 Widgets** nas duas edições (**Full** e **Standart**) com tema escuro unificado, internacionalização dinâmica em tempo de execução e alternância instantânea entre o **Layout Padrão** e o **Layout Duplo Paralelo** *(veja também a galeria correspondente em [English (US)](#interface-en-us))*.

### 1. Layout Padrão (Coluna Única) — Edição Full (`pt_BR`)

<p align="center">
  <img src="images/interfaces/pt_BR/Captura%20de%20tela%202026-11-25%20225228.png" alt="Compression Manager — Layout Padrão Edição Full (pt_BR)" width="960">
</p>

> **Arquivo:** [`images/interfaces/pt_BR/Captura de tela 2026-11-25 225228.png`](images/interfaces/pt_BR/Captura%20de%20tela%202026-11-25%20225228.png)

- **Janela Principal e Barra de Menus Superior:** Janela maximizada no tema escuro nativo, exibindo o título **`Compression Manager`** com o ícone oficial da aplicação na barra superior e o menu principal composto por **`Arquivo`**, **`Configurações`** e **`Opções`** (que centraliza seleção de idioma em tempo real, alternância para o Segundo Layout, níveis de compressão, políticas de atualização, Manual e diálogo Sobre).
- **Painel de Entrada e Controles de Ação Rápida:** Na metade superior encontra-se a lista **`Diretório(s) Pastas e Arquivos:`**, preparada para seleção múltipla ou arrastar e soltar diretamente do Windows Explorer (*Drag & Drop* com ponte UIPI). À esquerda estão agrupados os três botões com ícones dedicados — **`Adicionar Pastas:`**, **`Adicionar Arquivos:`** e **`Testar Integridade:`** (para validação bloco a bloco não destrutiva por *checksum*) — enquanto o canto superior direito concentra as ações rápidas **`Limpar Entrada:`** e **`Limpar Todas Saídas:`**.
- **Filtros Dinâmicos de Exibição por Método:** Logo abaixo do painel de entrada ficam as seis caixas de seleção da **Edição Full** todas ativas (**`☑ RAR`**, **`☑ ZIP`**, **`☑ 7Z`**, **`☑ TAR`**, **`☑ WIM`** e **`☑ EXTRAÇÃO`**), permitindo ocultar ou reexibir dinamicamente qualquer bloco sem perder as filas ou caminhos configurados.
- **Área Rolável `Métodos de compressão e extração`:** A metade inferior reúne em painel rolável os seis blocos operacionais na ordem **`.RAR`**, **`.ZIP`**, **`.7Z`**, **`.TAR`**, **`.WIM`** e **`Extração`**. Cada bloco possui seu próprio campo de destino (**`Diretório(s) de saída .RAR:`**, **`.ZIP:`**, **`.7Z:`**, **`.TAR:`**, **`.WIM:`** e **`Diretório(s) de destino para extração:`**) acompanhado por três controles dedicados à esquerda: **`Especificar Saída`** (seleção manual ou via *Drag & Drop*), o botão de disparo com o ícone do respectivo formato/motor (**`Armazenar .RAR:`**, **`Armazenar .ZIP:`**, **`Armazenar .7Z:`**, **`Armazenar .TAR:`**, **`Armazenar .WIM:`** ou **`Extrair Arquivos:`**) e **`Limpar Saída:`**.

### 2. Layout Duplo Paralelo (Duas Colunas Independentes) — Edição Full (`pt_BR`)

<p align="center">
  <img src="images/interfaces/pt_BR/Captura%20de%20tela%202026-11-25%20225321.png" alt="Compression Manager — Layout Duplo Paralelo Edição Full (pt_BR)" width="960">
</p>

> **Arquivo:** [`images/interfaces/pt_BR/Captura de tela 2026-11-25 225321.png`](images/interfaces/pt_BR/Captura%20de%20tela%202026-11-25%20225321.png)

- **Divisão Simétrica Preservando a Janela Maximizada:** Ao ativar o segundo layout, a janela mantém exatamente o estado maximizado e divide a área de trabalho horizontalmente em duas colunas equilibradas (`50%` / `50%`) abaixo da barra única de menus (**`Arquivo`**, **`Configurações`**, **`Opções`**), aproveitando monitores *widescreen* sem exigir múltiplas instâncias do processo.
- **Independência Completa de Entradas, Filtros e Limpeza:** Tanto a coluna esquerda quanto a coluna direita contam com sua própria caixa **`Diretório(s) Pastas e Arquivos:`**, seu próprio conjunto de botões **`Adicionar Pastas:`**, **`Adicionar Arquivos:`** e **`Testar Integridade:`**, seus próprios botões **`Limpar Entrada:`** e **`Limpar Todas Saídas:`**, e sua própria barra individual de caixas de seleção (**`☑ RAR`**, **`☑ ZIP`**, **`☑ 7Z`**, **`☑ TAR`**, **`☑ WIM`**, **`☑ EXTRAÇÃO`**).
- **Fluxos Paralelos de Compressão, Atualização e Extração:** Cada metade dispõe de sua própria área rolável **`Métodos de compressão e extração`** com todos os seis blocos (`.RAR`, `.ZIP`, `.7Z`, `.TAR`, `.WIM` e `Extração`, com botões **`Especificar Saída`**, **`Armazenar`** / **`Extrair Arquivos:`** e **`Limpar Saída:`**). Isso viabiliza operar duas filas totalmente independentes lado a lado — por exemplo, compactando pastas em `.7Z` ou `.RAR` em um painel enquanto extrai pacotes ou verifica a integridade no outro.

### 3. Layout Padrão (Coluna Única) — Edição Standart (`pt_BR`)

<p align="center">
  <img src="images/interfaces/pt_BR/Captura%20de%20tela%202026-11-25%20234257.png" alt="Compression Manager — Layout Padrão Edição Standart (pt_BR)" width="960">
</p>

> **Arquivo:** [`images/interfaces/pt_BR/Captura de tela 2026-11-25 234257.png`](images/interfaces/pt_BR/Captura%20de%20tela%202026-11-25%20234257.png)

- **Segregação Estrita para Formatos Abertos (`ENABLE_RAR_SUPPORT=OFF`):** Janela principal maximizada com tema escuro nativo, título **`Compression Manager`** e barra superior **`Arquivo`**, **`Configurações`**, **`Opções`**, construída sem nenhuma dependência, ícone (`winrar3.png`) ou bloco vinculado ao WinRAR proprietário.
- **Entrada e Barra Exclusiva com Cinco Filtros:** O painel superior mantém integralmente **`Diretório(s) Pastas e Arquivos:`**, os botões laterais **`Adicionar Pastas:`**, **`Adicionar Arquivos:`** e **`Testar Integridade:`**, além de **`Limpar Entrada:`** e **`Limpar Todas Saídas:`**. Na barra de filtros logo abaixo aparecem **exclusivamente cinco caixas de seleção** marcadas (**`☑ ZIP`**, **`☑ 7Z`**, **`☑ TAR`**, **`☑ WIM`** e **`☑ EXTRAÇÃO`**), omitindo por completo a opção `RAR`.
- **Cinco Blocos Operacionais do Motor 7-Zip Visíveis sem Rolagem:** A área **`Métodos de compressão e extração`** exibe integralmente na tela os cinco painéis abertos em ordem sequencial — **`.ZIP`**, **`.7Z`**, **`.TAR`**, **`.WIM`** e **`Extração`** — cada qual com sua lista de saída (**`Diretório(s) de saída .ZIP:`**, **`.7Z:`**, **`.TAR:`**, **`.WIM:`** e **`Diretório(s) de destino para extração:`**) e seus três botões (**`Especificar Saída`**, **`Armazenar`** / **`Extrair Arquivos:`** e **`Limpar Saída:`**).

### 4. Layout Duplo Paralelo (Duas Colunas Independentes) — Edição Standart (`pt_BR`)

<p align="center">
  <img src="images/interfaces/pt_BR/Captura%20de%20tela%202026-11-25%20234315.png" alt="Compression Manager — Layout Duplo Paralelo Edição Standart (pt_BR)" width="960">
</p>

> **Arquivo:** [`images/interfaces/pt_BR/Captura de tela 2026-11-25 234315.png`](images/interfaces/pt_BR/Captura%20de%20tela%202026-11-25%20234315.png)

- **Espaço Duplo Simétrico com Isolamento Total de Edição:** Preserva a janela maximizada dividindo o espaço horizontal em duas colunas paralelas (`50%` / `50%`) sob a barra de menus única (**`Arquivo`**, **`Configurações`**, **`Opções`**).
- **Dois Painéis Completos Sem WinRAR/RAR:** Tanto a coluna esquerda quanto a coluna direita disponibilizam instâncias independentes de **`Diretório(s) Pastas e Arquivos:`**, **`Adicionar Pastas:`**, **`Adicionar Arquivos:`**, **`Testar Integridade:`**, **`Limpar Entrada:`**, **`Limpar Todas Saídas:`** e cinco filtros abertos (**`☑ ZIP`**, **`☑ 7Z`**, **`☑ TAR`**, **`☑ WIM`**, **`☑ EXTRAÇÃO`**).
- **Cinco Blocos Abertos por Coluna Integrados na Mesma Tela:** Em ambos os lados, todos os cinco blocos (**`.ZIP`**, **`.7Z`**, **`.TAR`**, **`.WIM`** e **`Extração`**) ajustam-se com clareza na área útil da janela maximizada, permitindo empacotar múltiplos formatos abertos em paralelo enquanto extrai ou verifica integridade no outro fluxo.

---

## <a id="microsoft-store-privacidade-suporte"></a>🛡️ Distribuição Microsoft Store, Privacidade e Desinstalação Limpa

> [!IMPORTANT]
> **Conformidade Oficial Microsoft Store (Políticas v7.20 / MSIX):**
> - **Política de Privacidade Independente (Política 10.5):** **A Microsoft NÃO fornece uma política de privacidade padrão para o Compression Manager, e este software NÃO é coberto pela [Política de Privacidade da Microsoft](https://www.microsoft.com/pt-br/privacy/privacystatement).** Todo processamento local é regido exclusivamente pela [Política de Privacidade do Compression Manager](docs/privacy-policy.md#ptbr).
> - **Suporte do Desenvolvedor Independente:** **A Microsoft NÃO presta suporte ao cliente para o Compression Manager.** O suporte é oferecido voluntariamente apenas pelo autor via `linceu_lighthouse@outlook.com` e [GitHub Issues](https://github.com/fernandoncidade/Compression_Manager_cpp/issues).
> - **Capacidade Restrita `runFullTrust` (Política 10.6):** Declarada em [`library/AppxManifest.xml`](library/AppxManifest.xml) apenas para execução Win32 nativa em C++23 / Qt 6.12.0, diálogos padrão do Windows e acionamento local dos motores 7-Zip/WinRAR em caminhos escolhidos pelo usuário, sem telemetria ou rede.
> - **Garantia de Desinstalação Limpa (Política 10.2.7):** Remoção limpa a qualquer momento via **Configurações do Windows → Aplicativos → Aplicativos instalados** (ou Menu Iniciar → Desinstalar), sem deixar serviços em segundo plano.

| Disposição Regulatória | Referência Oficial no Repositório (`pt-BR`) |
|---|---|
| **Política de Privacidade Própria** | [docs/privacy-policy.md](docs/privacy-policy.md#ptbr) · [Asset Interno (.txt)](source/assets/PRIVACY_POLICY/Privacy_Policy_pt_BR.txt) |
| **Site Oficial do Desenvolvedor** | [https://github.com/fernandoncidade/Compression_Manager_cpp](https://github.com/fernandoncidade/Compression_Manager_cpp) |
| **Suporte Técnico (Best-Effort)** | `linceu_lighthouse@outlook.com` · [GitHub Issues](https://github.com/fernandoncidade/Compression_Manager_cpp/issues) *(sem suporte Microsoft; sem telefone/SLA)* |
| **Licença Oficial do Pacote** | [EULA Oficial (PT)](source/assets/EULA/EULA_pt_BR%20-%20Compression%20Manager.txt) · [CLC (PT)](source/assets/CLC/CLC_pt_BR%20-%20Compression%20Manager.txt) · [GNU AGPLv3 (Código-Fonte)](LICENSE) |
| **Dados Locais (`%LOCALAPPDATA%`)** | `config.json`, `language.json` e `logs\file_compression_*.log` em `%LOCALAPPDATA%\CompressionManager\` (LGPD art. 7º, V e IX / zero PII) |

---

## <a id="quick-start-compilacao"></a>🚀 Estrutura do Projeto, Compilação e Instaladores

### Estrutura Principal
```text
Compression_Manager_cpp/
├── CMakeLists.txt                   # Build principal C++23 / Qt 6.12.0 (Full & Standart)
├── CMakePresets.json                # Presets MSVC 14.52 e MinGW 15.1.0 (Full, Standart, Mocks, Suite)
├── LICENSE                          # Licença integral GNU AGPLv3
├── README.md / MANUAL.md / OBSERVACAO.md / CONTRIBUTING.md / CODE_OF_CONDUCT.md / SECURITY.md
├── .github/                         # Workflows e templates de Issues / Pull Requests
├── .vscode/                         # Kits, IntelliSense e tarefas MSVC/MinGW
├── cmake/                           # Scripts CMake enxutos e gerar_distribuicoes_limpas.ps1
├── docs/                            # Documentação técnica bilíngue e imagens (docs/images/)
├── images/                          # Capturas de tela da interface gráfica (pt_BR e en_US)
├── mocks/                           # find_VersionEditor e CompileTranslations{GUI,Terminal}
├── library/                         # AppxManifest.xml, notas de prompt e scripts Inno Setup 7
├── repo/                            # Especificações auxiliares de engenharia (`prompt_Compression_Manager_cpp.txt`)
├── tests/                           # Suíte de testes automatizados (10 executáveis Qt6::Test + CTest)
└── source/                          # Código-fonte modular C++23 e ativos legais em source/assets/
```

### Compilando via Presets Oficiais (`CMakePresets.json`)
```powershell
git clone https://github.com/fernandoncidade/Compression_Manager_cpp.git
cd Compression_Manager_cpp

# Edição Full (ENABLE_RAR_SUPPORT=ON) — MSVC puro (MSBuild) e MinGW 15.1.0 (Ninja)
cmake --preset full-msvc-release && cmake --build --preset full-msvc-release --target Compression_ManagerDeploy --parallel
cmake --preset full-mingw-release && cmake --build --preset full-mingw-release --target Compression_ManagerDeploy --parallel

# Edição Standart (ENABLE_RAR_SUPPORT=OFF) — MSVC puro (MSBuild) e MinGW 15.1.0 (Ninja)
cmake --preset standart-msvc-release && cmake --build --preset standart-msvc-release --target Compression_ManagerDeploy --parallel
cmake --preset standart-mingw-release && cmake --build --preset standart-mingw-release --target Compression_ManagerDeploy --parallel

# Utilitários Mocks (find_VersionEditor & CompileTranslations)
cmake --preset mocks-msvc-release && cmake --build --preset mocks-msvc-release --target MocksDeploy --parallel
cmake --preset mocks-mingw-release && cmake --build --preset mocks-mingw-release --target MocksDeploy --parallel

# Suíte de Testes Completa (build/build_suite_{msvc,mingw}; sem distribuição externa)
cmake --preset suite-msvc-release && cmake --build --preset suite-msvc-release --target SuiteDeploy --parallel && ctest --preset suite-msvc-release --output-on-failure
cmake --preset suite-mingw-release && cmake --build --preset suite-mingw-release --target SuiteDeploy --parallel && ctest --preset suite-mingw-release --output-on-failure

# Gerar distribuições externas ultralimpas + compilar todos os 4 instaladores Inno Setup 7
powershell -NoProfile -ExecutionPolicy Bypass -File .\cmake\gerar_distribuicoes_limpas.ps1 -DistributionName all
& "C:\Program Files\Inno Setup 7\ISCC.exe" .\library\Compression_Manager.iss
```
- **Distribuições geradas:** `D:\MISCELANEAS\VSCode\Compression_Manager\Compression_Manager_{Full,Standart}_{mingw,msvc}`
- **Instaladores gerados:** `D:\MISCELANEAS\VSCode\Compression_Manager\InnoSetupOutput\` *(MSVC menor que MinGW graças ao CRT app-local sem `vc_redist.x64.exe`)*

---

## 🏛️ Escopo Auxiliar Gratuito e Isenção ("AS IS")

> [!WARNING]
> **Uso Auxiliar e Boas Práticas de Backup:** O **Compression Manager** é distribuído gratuitamente como ferramenta utilitária auxiliar **"NO ESTADO EM QUE SE ENCONTRA" ("AS IS")**, sem garantias explícitas ou implícitas ([EULA Oficial](source/assets/EULA/EULA_pt_BR%20-%20Compression%20Manager.txt), Lei nº 9.609/1998 e [GNU AGPLv3](LICENSE) Seções 15–16, preservados eventuais direitos consumeristas irrenunciáveis). Execute sempre **Testar Integridade** após compactações importantes e mantenha cópias de segurança independentes dos seus arquivos críticos.

---

## 📚 Documentação Técnica e Conformidade

- 🏛️ [Arquitetura do Sistema e Isolamento de Runtimes](docs/architecture.md#ptbr)
- 📋 [Conformidade de Formatos, Licenças e Microsoft Store](docs/compliance.md#ptbr)
- 🔒 [Política de Privacidade e Soberania Local](docs/privacy-policy.md#ptbr)
- ⚖️ [Modelo de Duplo Licenciamento e Checklist MSIX](docs/licensing.md#ptbr)
- 🛡️ [Modelo de Segurança e Resiliência](docs/security-model.md#ptbr)
- 📖 [Guia do Usuário](docs/user-guide.md#ptbr) · [Manual Completo (MANUAL.md)](MANUAL.md#ptbr) · [Snapshot Técnico (OBSERVACAO.md)](OBSERVACAO.md)
- 💡 [Sugestões de Contribuição e Backlog Técnico](docs/suggested-issues.md#ptbr)
- 🔬 [Auditoria Técnica, ADRs, Métricas e Roadmap](docs/technical-audit-and-roadmap.md#ptbr)
- 🚀 [Notas de Lançamento da Versão 2026.11.22.0](docs/release-v2026.11.22.0.md#ptbr)

---

## ⚖️ Licenciamento e Atribuições de Terceiros

- **Código-Fonte Público (GitHub):** Licenciado sob **GNU Affero General Public License v3 (GNU AGPLv3)** ([`LICENSE`](LICENSE)).
- **Pacotes Binários Oficiais:** Regidos pelo **[EULA Oficial](source/assets/EULA/EULA_pt_BR%20-%20Compression%20Manager.txt)** e **[CLC](source/assets/CLC/CLC_pt_BR%20-%20Compression%20Manager.txt)**.
- **Terceiros:** Qt `6.12.0` (`GNU LGPLv3`), 7-Zip (`GNU LGPL`, `BSD-2-Clause`, `BSD-3-Clause` e restrição unRAR — Igor Pavlov, Apple Inc., Facebook Inc., Yann Collet, Alexander Roshal), MinGW-w64 `15.1.0` (`GPLv3 + GCC Runtime Exception`) / MSVC CRT, e WinRAR proprietário exclusivamente na Edição Full (sem `rarreg.key`) — detalhes completos em [`source/assets/NOTICES/NOTICE_pt_BR.txt`](source/assets/NOTICES/NOTICE_pt_BR.txt) e [`source/assets/LICENSES/License.txt`](source/assets/LICENSES/License.txt).
- **Como Contribuir:** Consulte [`CONTRIBUTING.md`](CONTRIBUTING.md#ptbr), [`CODE_OF_CONDUCT.md`](CODE_OF_CONDUCT.md#ptbr) e [`SECURITY.md`](SECURITY.md#ptbr).

</details>

---

## <a id="enus"></a><a id="english-us"></a>🇺🇸 English (US)

> **Note:** This repository refers to the official **v2026.11.22.0** release of the **Compression Manager** project (C++23, Qt 6.12.0, MSVC x64 `14.52.37025` / Windows SDK `10.0.28000.0`, and MinGW-w64 `15.1.0`), distributed under a transparent **Dual-Licensing** framework (**GNU AGPLv3** for public source code on GitHub and **Official EULA + CLC** for author-signed binary distributions on the Microsoft Store and desktop installers, running 100% locally and offline).

<details>
<summary><b>Click to expand / collapse English (US) documentation</b></summary><br>

# Compression Manager — Multiformat File Archiver and Compression Suite

Version: v2026.11.22.0<br>
Technical revision date: November 22, 2026<br>
Author: Fernando Nillsson Cidade<br>

This official release **2026.11.22.0** delivers stabilized parallel Dual Layout switching with preserved maximized window states, full toolchain migration to **C++23** and **Qt 6.12.0**, ultra-lean external distributions using app-local MSVC CRT without `vc_redist.x64.exe`, and a formal **Dual-Licensing** model.

---

## ⚖️ Copyright Holder Manifesto & Legal Authorization

> **Express Statement by Fernando Nillsson Cidade:**  
> I, **Fernando Nillsson Cidade**, as the sole author, original creator, and exclusive copyright holder of the **Compression Manager** project, **authorize the use, study, copying, modification, compilation, and distribution of the public source code in this repository** under the exact terms of **[LICENSE](LICENSE)** (**GNU Affero General Public License v3 — GNU AGPLv3**, SPDX identifier `AGPL-3.0-only`), reserving name and trademark protection under Section 7(e) (`"Compression Manager"` / `"COMPRESSION MANAGER"`).  
> Any modified version, extension, or derivative work derived from the public source code must preserve original authorship attribution and release its corresponding source code under **GNU AGPLv3**.  
> **Freedom for Institutional Documentation in `source/assets/`:** Compliance texts under [`source/assets/`](source/assets/COPYRIGHT/COPYRIGHT%20AND%20TRADEMARK%20NOTICE_en_US.txt) (`ABOUT`, `CLC`, `COPYRIGHT`, `EULA`, `LICENSES`, `NOTICES`, and `PRIVACY_POLICY`) were created to fulfill Microsoft Store and Windows installer requirements; the author **expressly grants developers and forks full freedom** to modify, remove, add, or adapt those documents in their own distributions provided they respect laws and platform guidelines in their jurisdictions.  
> **Boundary for Author's Signed Binaries (Microsoft Store & Official Installers):** The author's packaged and digitally signed distribution is provided **free of charge** under the author's **[Official EULA](source/assets/EULA/EULA_en_US%20-%20Compression%20Manager.txt)** (aligned with Microsoft Store Standard Application License Terms) and **[Commercial Licensing Clarification — CLC](source/assets/CLC/CLC_en_US%20-%20Compression%20Manager.txt)**. Paid commercial resale, trademark misappropriation, or unauthorized republication of the author's official signed binary package is strictly prohibited.  
> **Support Channel, Microsoft Non-Coverage & Disclaimer ("AS IS"):** **Microsoft does NOT provide customer support and does NOT cover this software under Microsoft's Privacy Statement.** Author support is offered **exclusively via email (`linceu_lighthouse@outlook.com`) and GitHub Issues/Discussions** on a **voluntary best-effort basis according to availability, with no urgency commitment or SLA**. Supplied **"AS IS"** without warranties; users should validate important archives using integrity testing and keep independent backups. See [`docs/licensing.md`](docs/licensing.md#enus).

---

## 📌 Project Fact Sheet

<p align="center">
  <img src="docs/images/logo.png" alt="Compression Manager Icon" width="128">
</p>

- **Official Name:** Compression Manager (`Compression_Manager_7Z_RAR` CMake project)
- **Author & Maintainer:** Fernando Nillsson Cidade *(Individual Developer)*
- **Official Support Email (voluntary / best-effort, no SLA):** `linceu_lighthouse@outlook.com`
- **Official Repository:** [https://github.com/fernandoncidade/Compression_Manager_cpp](https://github.com/fernandoncidade/Compression_Manager_cpp)
- **MSIX Package Identity (`library/AppxManifest.xml`):** `fernandoncidade.COMPRESSIONMANAGER` (`x64`) · `<rescap:Capability Name="runFullTrust" />` · `<uap10:Content Enforcement="on" />`
- **System Requirements:** Windows 10 (build `17763+`) / Windows 11 x64
- **Runtime Dynamic Languages:** English (United States — `en_US`) · Português (Brasil — `pt_BR`)
- **Current Tech Stack:** **C++23**, **Qt 6.12.0** (`Core`, `Gui`, `Widgets`), **pure MSVC x64 `14.52.37025`** (Visual Studio 18 2026 / MSBuild & app-local CRT without `vc_redist.x64.exe`), **MinGW-w64 `15.1.0`** (`mingw1510_64`, Ninja), **CMake `3.28+`**, **Inno Setup 7**
- **Built Targets via CMake Presets (`CMakePresets.json`):**
  - `Compression_Manager.exe`: Primary desktop application in **Full** (`ENABLE_RAR_SUPPORT=ON`) or **Standart** (`ENABLE_RAR_SUPPORT=OFF`) edition
  - `find_VersionEditor.exe`: GUI/CLI (`--check`) tool in `mocks/` synchronizing CalVer across `source/assets/`, `ginter_20_exibir_sobre.cpp`, docs (`README.md`, `MANUAL.md`, `OBSERVACAO.md`, `docs/*.md`), `library/AppxManifest.xml`, `library/Compression_Manager_cpp.txt`, and `library/*.iss`
  - `CompileTranslationsGUI.exe` & `CompileTranslationsTerminal.exe`: Utilities in `mocks/` compiling/auditing `.ts` → `.qm` localization catalogs

---

## Two Strictly Segregated Product Editions: Full and Standart

Originally prototyped in Python/PySide6 and completely rewritten into **native C++23 + Qt 6.12.0**, **Compression Manager** eliminates interpreter overhead, GIL contention, and external runtime dependencies while building two strictly isolated editions driven by `ENABLE_RAR_SUPPORT` (`CMakeLists.txt`):

### 1. Full Edition (`ENABLE_RAR_SUPPORT=ON`)
- **Supported Archiving Formats:** **RAR** (`0–5`), **ZIP** (`0, 1, 3, 5, 7, 9`), **7Z** (`0, 1, 3, 5, 7, 9`), **TAR** (`0`), and **WIM** (`0`).
- **Integrated Engines:** **7-Zip** (`7zG.exe` / `7z.exe`) and **WinRAR** (`WinRAR.exe`, zero `rarreg.key` bundled).
- **Lean External Distributions:** `Compression_Manager_Full_msvc` (`35.20 MB` / installer `13.05 MB`) and `Compression_Manager_Full_mingw` (`41.96 MB` / installer `15.13 MB`).

### 2. Standart Edition (`ENABLE_RAR_SUPPORT=OFF`)
- **Supported Archiving Formats:** Open standards only — **ZIP**, **7Z**, **TAR**, and **WIM**.
- **Pure Open Engine & Complete Isolation:** Powered exclusively by **7-Zip** (`7zG.exe` / `7z.exe`), containing zero WinRAR/RAR source code paths, menus, icons (`winrar3.png`), or binaries.
- **Lean External Distributions:** `Compression_Manager_Standart_msvc` (`31.96 MB` / installer `11.57 MB`) and `Compression_Manager_Standart_mingw` (`38.72 MB` / installer `13.63 MB`).

---

## Technical highlights in version 2026.11.22.0

- **Stabilized Standard & Parallel Dual Layout (`ginter_21_toggle_segundo_layout.cpp`, `ginter_27_adapt_layout_orientation.cpp`):** Side-by-side independent columns for input, per-format output folders, and task queues, strictly preserving `isMaximized()` during transitions and reorienting horizontally or vertically based on window aspect ratio.
- **Background Archiving, Updating & Extraction (`MotoresCompressao`):** Dedicated `QThread` + `QProcess` workers keep UI responsive with granular format levels, *Update existing archives* (`u` vs. `a`), and two-stage temporary extraction for `.tar.xz`, `.tgz`, `.tar.gz`, and `.bz2`.
- **Non-Destructive Checksum Integrity Testing (`t`):** Validates archives (`.rar`, `.zip`, `.7z`, `.bz2`, `.tar`, `.wim`, `.tar.xz`, `.zipx`, `.tgz`, `.tar.gz`, `.lzh`, `.iso`) block-by-block without writing files to disk.
- **UIPI-Aware Drag & Drop (`ui_03_dragDrop.cpp`):** Drag files/folders straight from Windows Explorer into input and output lists even across elevated sessions via HiDPI-aware `WM_DROPFILES` native event filtering.
- **Live Bilingual UI (`pt_BR` ↔ `en_US`) & Edition-Aware Manual/About:** Instant runtime language switching plus built-in Manual tailored to Full or Standart and live search inside the About dialog.
- **Lean Deployments Without `vc_redist.x64.exe` & Zero Leakage (`cmake/*.cmake`, `library/*.iss`):** App-local `Microsoft.VC145.CRT` deployment (MSVC installers smaller than MinGW), pruning of unused Qt DLLs (`Qt6Network`, `Qt6Pdf`, `Qt6Svg`, `Qt6PrintSupport`) and Store marketing banners/PDFs, plus triple guardrails blocking `rarreg.key`.

---

## <a id="interface-en-us"></a>🖼️ Graphical User Interface (English — `en_US`)

The screenshots below showcase the native **Qt 6.12.0 Widgets** interface across both product editions (**Full** and **Standart**), featuring a unified dark theme, live runtime localization, and instant switching between the **Standard Layout** and the **Parallel Dual Layout** *(see also the matching gallery in [Português (BR)](#interface-pt-br))*.

### 1. Standard Single-Column Layout — Full Edition (`en_US`)

<p align="center">
  <img src="images/interfaces/en_US/Captura%20de%20tela%202026-11-25%20225350.png" alt="Compression Manager — Standard Single-Column Layout Full Edition (en_US)" width="960">
</p>

> **File:** [`images/interfaces/en_US/Captura de tela 2026-11-25 225350.png`](images/interfaces/en_US/Captura%20de%20tela%202026-11-25%20225350.png)

- **Main Window & Localized Top Menu Bar:** Maximized native dark-mode desktop window displaying the **`Compression Manager`** title and application icon along with the localized top menu bar (**`File`**, **`Settings`**, **`Options`**), demonstrating seamless runtime translation into English (`en_US`) without restarting the application.
- **Upper Input Pane & Quick Action Controls:** The top section features the **`Directory(s) Folders and Files:`** multi-item drop target (supporting standard pickers as well as UIPI-aware Drag & Drop directly from Windows Explorer), flanked on the left by icon buttons (**`Add Folders:`**, **`Add Files:`**, and **`Test Integrity:`** for non-destructive checksum verification) and in the top-right corner by quick-reset controls (**`Clear Input:`** and **`Clear All Outputs:`**).
- **Dynamic Method Visibility Toggles:** Immediately below the input list sit all six **Full Edition** method checkboxes enabled (**`☑ RAR`**, **`☑ ZIP`**, **`☑ 7Z`**, **`☑ TAR`**, **`☑ WIM`**, **`☑ EXTRACTION`**), allowing each format panel to be toggled on or off dynamically.
- **Scrollable `Compression and extraction methods` Area:** The lower scrollable workspace presents all six operational sections (shown in this dynamically localized view in the order **`.7Z`**, **`Extraction`**, **`.RAR`**, **`.TAR`**, **`.WIM`**, **`.ZIP`**). Each section pairs its dedicated destination box (**`Output directory(s) .7Z:`**, **`Destination directory(s) for extraction:`**, **`Output directory(s) .RAR:`**, **`.TAR:`**, **`.WIM:`**, **`.ZIP:`**) with three left-aligned buttons: **`Specify Output`**, the format-specific action trigger (**`Store .7Z:`**, **`Extract Files:`**, **`Store .RAR:`**, **`Store .TAR:`**, **`Store .WIM:`**, **`Store .ZIP:`**), and **`Clear Output:`**.

### 2. Parallel Dual Layout (Two Independent Columns) — Full Edition (`en_US`)

<p align="center">
  <img src="images/interfaces/en_US/Captura%20de%20tela%202026-11-25%20225424.png" alt="Compression Manager — Parallel Dual Layout Full Edition (en_US)" width="960">
</p>

> **File:** [`images/interfaces/en_US/Captura de tela 2026-11-25 225424.png`](images/interfaces/en_US/Captura%20de%20tela%202026-11-25%20225424.png)

- **Balanced Split Preserving Maximized Geometry:** Enabling the second layout preserves the window's exact maximized state while splitting the workspace horizontally into two `50%` / `50%` side-by-side columns beneath the single menu bar (**`File`**, **`Settings`**, **`Options`**).
- **Complete Structural Isolation Between Columns:** Both left and right workspaces own their individual **`Directory(s) Folders and Files:`** input box, left-side action buttons (**`Add Folders:`**, **`Add Files:`**, **`Test Integrity:`**), top-right reset controls (**`Clear Input:`**, **`Clear All Outputs:`**), and independent format visibility checkboxes (**`☑ RAR`**, **`☑ ZIP`**, **`☑ 7Z`**, **`☑ TAR`**, **`☑ WIM`**, **`☑ EXTRACTION`**).
- **Independent Dual `Compression and extraction methods` Lanes:** Each column embeds its own scrollable **`Compression and extraction methods`** panel containing all six archiving and extraction blocks (**`Specify Output`**, **`Store`** / **`Extract Files:`**, **`Clear Output:`**). Underscoring true per-lane independence, the left lane retains its localized state order (`.7Z`, `Extraction`, `.RAR`, `.TAR`, `.WIM`, `.ZIP`) while the newly constructed right lane initializes in canonical Full Edition order (`.RAR`, `.ZIP`, `.7Z`, `.TAR`, `.WIM`, `Extraction`), enabling two concurrent, non-interfering packaging, update, extraction, or integrity-testing workflows.

### 3. Standard Single-Column Layout — Standart Edition (`en_US`)

<p align="center">
  <img src="images/interfaces/en_US/Captura%20de%20tela%202026-11-25%20234335.png" alt="Compression Manager — Standard Single-Column Layout Standart Edition (en_US)" width="960">
</p>

> **File:** [`images/interfaces/en_US/Captura de tela 2026-11-25 234335.png`](images/interfaces/en_US/Captura%20de%20tela%202026-11-25%20234335.png)

- **Strict Open-Format Segregation (`ENABLE_RAR_SUPPORT=OFF`):** Maximized native dark-mode desktop window titled **`Compression Manager`** with the localized menu bar (**`File`**, **`Settings`**, **`Options`**), compiled strictly without proprietary WinRAR binaries, source paths, or the `winrar3.png` asset.
- **Input Pane & Exclusive Five-Format Toggle Bar:** The top workspace retains **`Directory(s) Folders and Files:`**, left-hand action controls (**`Add Folders:`**, **`Add Files:`**, **`Test Integrity:`**), and top-right reset actions (**`Clear Input:`**, **`Clear All Outputs:`**). Directly underneath, **only the five open-format checkboxes** are rendered (**`☑ ZIP`**, **`☑ 7Z`**, **`☑ TAR`**, **`☑ WIM`**, **`☑ EXTRACTION`**), completely omitting `RAR`.
- **All Five 7-Zip Operational Panels Visible Without Scrolling:** The **`Compression and extraction methods`** workspace fits all five pure 7-Zip sections cleanly on screen (shown in this live-localized view as **`.7Z`**, **`Extraction`**, **`.TAR`**, **`.WIM`**, and **`.ZIP`**), each pairing its destination pane (**`Output directory(s) .7Z:`**, **`Destination directory(s) for extraction:`**, **`.TAR:`**, **`.WIM:`**, **`.ZIP:`**) with **`Specify Output`**, **`Store`** / **`Extract Files:`**, and **`Clear Output:`**.

### 4. Parallel Dual Layout (Two Independent Columns) — Standart Edition (`en_US`)

<p align="center">
  <img src="images/interfaces/en_US/Captura%20de%20tela%202026-11-25%20234400.png" alt="Compression Manager — Parallel Dual Layout Standart Edition (en_US)" width="960">
</p>

> **File:** [`images/interfaces/en_US/Captura de tela 2026-11-25 234400.png`](images/interfaces/en_US/Captura%20de%20tela%202026-11-25%20234400.png)

- **Side-by-Side 50%/50% Workspace Preserving Maximized State:** Splits the maximized Standart window horizontally under the shared **`File`**, **`Settings`**, **`Options`** menu bar without exit from fullscreen or state loss.
- **Two Fully Isolated Open-Format Columns:** Both left and right workspaces feature independent **`Directory(s) Folders and Files:`** drop boxes, **`Add Folders:`**, **`Add Files:`**, **`Test Integrity:`**, **`Clear Input:`**, **`Clear All Outputs:`**, and five open checkboxes (**`☑ ZIP`**, **`☑ 7Z`**, **`☑ TAR`**, **`☑ WIM`**, **`☑ EXTRACTION`**).
- **Zero-Scroll Parallel Execution Lanes:** All five open archiving and extraction panels fit visibly in both columns without scrolling — with the left lane retaining its localized order (`.7Z`, `Extraction`, `.TAR`, `.WIM`, `.ZIP`) and the right lane initialized in canonical Standart Edition order (`.ZIP`, `.7Z`, `.TAR`, `.WIM`, `Extraction`) — enabling independent simultaneous workflows using purely open archive formats.

---

## <a id="microsoft-store-privacy-support"></a>🛡️ Microsoft Store Distribution, Privacy & Clean Uninstallation

> [!IMPORTANT]
> **Official Microsoft Store Compliance (Policies v7.20 / MSIX):**
> - **Independent Privacy Policy (Policy 10.5):** **Microsoft does NOT provide a default privacy policy for Compression Manager, and this software is NOT covered by the [Microsoft Privacy Statement](https://www.microsoft.com/en-us/privacy/privacystatement).** All local operations are governed solely by [Compression Manager's Privacy Policy](docs/privacy-policy.md#enus).
> - **Independent Developer Support:** **Microsoft provides NO customer support for Compression Manager.** Free support takes place voluntarily via `linceu_lighthouse@outlook.com` and [GitHub Issues](https://github.com/fernandoncidade/Compression_Manager_cpp/issues).
> - **Restricted Capability `runFullTrust` (Policy 10.6):** Declared in [`library/AppxManifest.xml`](library/AppxManifest.xml) solely for native C++23 / Qt 6.12.0 desktop execution, standard Windows pickers, and local 7-Zip/WinRAR execution on user-selected paths, with zero network traffic or telemetry.
> - **Clean Uninstallation Guarantee (Policy 10.2.7):** Clean removal at any time via **Windows Settings → Apps → Installed Apps** (or Start Menu → Uninstall), leaving zero background services behind.

| Regulatory Provision | Official Repository Reference (`en-US`) |
|---|---|
| **Developer Privacy Policy** | [docs/privacy-policy.md](docs/privacy-policy.md#enus) · [In-App Asset (.txt)](source/assets/PRIVACY_POLICY/Privacy_Policy_en_US.txt) |
| **Developer Official Website** | [https://github.com/fernandoncidade/Compression_Manager_cpp](https://github.com/fernandoncidade/Compression_Manager_cpp) |
| **Customer Support (Best-Effort)** | `linceu_lighthouse@outlook.com` · [GitHub Issues](https://github.com/fernandoncidade/Compression_Manager_cpp/issues) *(no Microsoft support; no phone/SLA)* |
| **Official Package Licensing** | [Official EULA (EN)](source/assets/EULA/EULA_en_US%20-%20Compression%20Manager.txt) · [CLC (EN)](source/assets/CLC/CLC_en_US%20-%20Compression%20Manager.txt) · [GNU AGPLv3 (Source)](LICENSE) |
| **Local Data (`%LOCALAPPDATA%`)** | `config.json`, `language.json`, and `logs\file_compression_*.log` under `%LOCALAPPDATA%\CompressionManager\` (zero PII) |

---

## <a id="quick-start-build"></a>🚀 Project Layout, Building & Installers

### Core Directory Layout
```text
Compression_Manager_cpp/
├── CMakeLists.txt                   # Main C++23 / Qt 6.12.0 build configuration (Full & Standart)
├── CMakePresets.json                # MSVC 14.52 & MinGW 15.1.0 presets (Full, Standart, Mocks, Suite)
├── LICENSE                          # Full GNU AGPLv3 license text
├── README.md / MANUAL.md / OBSERVACAO.md / CONTRIBUTING.md / CODE_OF_CONDUCT.md / SECURITY.md
├── .github/                         # Workflows, Issue templates, and PR template
├── .vscode/                         # VS Code kits, IntelliSense, and build tasks
├── cmake/                           # Lean deploy CMake scripts & gerar_distribuicoes_limpas.ps1
├── docs/                            # Bilingual technical docs & images (docs/images/)
├── images/                          # Graphical user interface screenshots (pt_BR & en_US)
├── mocks/                           # find_VersionEditor and CompileTranslations{GUI,Terminal}
├── library/                         # AppxManifest.xml, release prompts & Inno Setup 7 scripts
├── repo/                            # Engineering specification notes (`prompt_Compression_Manager_cpp.txt`)
├── source/                          # Modular C++23 source tree & legal assets under source/assets/
└── tests/                           # Automated engineering test suite (10 Qt6::Test executables + CTest)
```

### Building with CMake Presets (`CMakePresets.json`)
```powershell
git clone https://github.com/fernandoncidade/Compression_Manager_cpp.git
cd Compression_Manager_cpp

# Full Edition (ENABLE_RAR_SUPPORT=ON) — Pure MSVC (MSBuild) & MinGW 15.1.0 (Ninja)
cmake --preset full-msvc-release && cmake --build --preset full-msvc-release --target Compression_ManagerDeploy --parallel
cmake --preset full-mingw-release && cmake --build --preset full-mingw-release --target Compression_ManagerDeploy --parallel

# Standart Edition (ENABLE_RAR_SUPPORT=OFF) — Pure MSVC (MSBuild) & MinGW 15.1.0 (Ninja)
cmake --preset standart-msvc-release && cmake --build --preset standart-msvc-release --target Compression_ManagerDeploy --parallel
cmake --preset standart-mingw-release && cmake --build --preset standart-mingw-release --target Compression_ManagerDeploy --parallel

# Auxiliary Mocks (find_VersionEditor & CompileTranslations)
cmake --preset mocks-msvc-release && cmake --build --preset mocks-msvc-release --target MocksDeploy --parallel
cmake --preset mocks-mingw-release && cmake --build --preset mocks-mingw-release --target MocksDeploy --parallel

# Full Engineering Test Suite (build/build_suite_{msvc,mingw}; no external distribution)
cmake --preset suite-msvc-release && cmake --build --preset suite-msvc-release --target SuiteDeploy --parallel && ctest --preset suite-msvc-release --output-on-failure
cmake --preset suite-mingw-release && cmake --build --preset suite-mingw-release --target SuiteDeploy --parallel && ctest --preset suite-mingw-release --output-on-failure

# Generate ultra-lean external distributions & compile all 4 Inno Setup 7 installers
powershell -NoProfile -ExecutionPolicy Bypass -File .\cmake\gerar_distribuicoes_limpas.ps1 -DistributionName all
& "C:\Program Files\Inno Setup 7\ISCC.exe" .\library\Compression_Manager.iss
```
- **External distributions:** `D:\MISCELANEAS\VSCode\Compression_Manager\Compression_Manager_{Full,Standart}_{mingw,msvc}`
- **Standalone installers:** `D:\MISCELANEAS\VSCode\Compression_Manager\InnoSetupOutput\` *(MSVC smaller than MinGW via app-local CRT with zero `vc_redist.x64.exe`)*

---

## 🏛️ Free Auxiliary Scope & Legal Disclaimer ("AS IS")

> [!WARNING]
> **Auxiliary Use & Backup Hygiene:** **Compression Manager** is supplied free of charge as an auxiliary desktop utility **"AS IS"** without express or implied warranties ([Official EULA](source/assets/EULA/EULA_en_US%20-%20Compression%20Manager.txt), Brazilian Law No. 9.609/1998, and [GNU AGPLv3](LICENSE) Sections 15–16, preserving non-waivable statutory consumer rights where applicable). Always validate important archives via **Test Integrity** and maintain independent backups of critical files.

---

## 📚 Technical & Compliance Documentation

- 🏛️ [System Architecture & Runtime Isolation](docs/architecture.md#enus)
- 📋 [Format Compliance, Licensing & Microsoft Store](docs/compliance.md#enus)
- 🔒 [Privacy Policy & Local Data Sovereignty](docs/privacy-policy.md#enus)
- ⚖️ [Dual Licensing & MSIX Partner Center Checklist](docs/licensing.md#enus)
- 🛡️ [Security Model & Resilience](docs/security-model.md#enus)
- 📖 [User Guide](docs/user-guide.md#enus) · [Operations Manual (MANUAL.md)](MANUAL.md#enus) · [Technical Snapshot (OBSERVACAO.md)](OBSERVACAO.md)
- 💡 [Suggested Issues & Technical Backlog](docs/suggested-issues.md#enus)
- 🔬 [Technical Audit, ADRs, Metrics & Roadmap](docs/technical-audit-and-roadmap.md#enus)
- 🚀 [Release Notes for v2026.11.22.0](docs/release-v2026.11.22.0.md#enus)

---

## ⚖️ Licensing & Third-Party Attributions

- **Public Source Code (GitHub):** Licensed under **GNU Affero General Public License v3 (GNU AGPLv3)** ([`LICENSE`](LICENSE)).
- **Official Binary Packages:** Governed by **[Official EULA](source/assets/EULA/EULA_en_US%20-%20Compression%20Manager.txt)** and **[CLC](source/assets/CLC/CLC_en_US%20-%20Compression%20Manager.txt)**.
- **Third-Party Software:** Qt `6.12.0` (`GNU LGPLv3`), 7-Zip (`GNU LGPL`, `BSD-2-Clause`, `BSD-3-Clause`, and unRAR restriction — Igor Pavlov, Apple Inc., Facebook Inc., Yann Collet, Alexander Roshal), MinGW-w64 `15.1.0` (`GPLv3 + GCC Runtime Exception`) / MSVC CRT, and proprietary WinRAR solely in the Full Edition (no `rarreg.key`) — full notices in [`source/assets/NOTICES/NOTICE_en_US.txt`](source/assets/NOTICES/NOTICE_en_US.txt) and [`source/assets/LICENSES/License.txt`](source/assets/LICENSES/License.txt).
- **Contributing:** See [`CONTRIBUTING.md`](CONTRIBUTING.md#enus), [`CODE_OF_CONDUCT.md`](CODE_OF_CONDUCT.md#enus), and [`SECURITY.md`](SECURITY.md#enus).

</details>
