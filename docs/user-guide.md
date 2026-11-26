# Guia do Usuário / User Guide — Compression Manager

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details open>
<summary>Clique para expandir / recolher o guia do usuário em português</summary><br>

# Compression Manager — Guia do Usuário e Manual de Operações

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  
**Autor:** Fernando Nillsson Cidade  

---

> [!NOTE]
> **Edição Full vs. Edição Standart:**  
> - **Edição Full:** Suporta empacotamento em **RAR**, **ZIP**, **7Z**, **TAR** e **WIM** (via **7-Zip** + **WinRAR**) e extração ampla incluindo `.rar`.  
> - **Edição Standart:** Suporta empacotamento exclusivamente em formatos abertos **ZIP**, **7Z**, **TAR** e **WIM** (via **7-Zip** puro, sem referências ou arquivos do WinRAR/RAR).

## 1. Visão Geral da Interface (`InterfaceGrafica`)

A janela principal organiza-se de forma simples e direta em quatro blocos integrados:

1. **Barra de Menus Superior:**
   - **Arquivo:**
     - **Adicionar Pastas:** / **Adicionar Arquivos:** — seleção múltipla contínua de origens;
     - **Testar Integridade:** — executa verificação bloco a bloco sem extração em disco;
     - **Limpar Entrada:** / **Limpar Todas Saídas:** — esvazia a lista de entrada ou todas as caixas de saída;
     - **Especificar Saída:** — define pastas de destino para `.RAR` *(somente Full)*, `.ZIP`, `.7Z`, `.TAR`, `.WIM` e `Extração`;
     - **Armazenar:** — inicia compactação em `.RAR` *(somente Full)*, `.ZIP`, `.7Z`, `.TAR` ou `.WIM` diretamente pela barra de menus;
     - **Extrair Arquivos:** / **Limpar Saída:** / **Sair**.
   - **Configurações:**
     - **Método de Compressão:** submenus por formato com seleção persistida em `config.json`:
       - `RAR (0-5)` *(somente Edição Full)*: `0 - Armazenar` a `5 - Ótimo`;
       - `ZIP (0-9)` e `7Z (0-9)`: `0 - Armazenar`, `1 - Mais Rápido`, `3 - Rápido`, `5 - Normal`, `7 - Máximo`, `9 - Ultra`;
       - `TAR (0)` e `WIM (0)`: `0 - Armazenar`.
     - **Layouts:** alterna entre **Primeiro Layout** *(padrão)* e **Segundo Layout** *(Layout Duplo paralelo)*;
     - **Idiomas:** troca ao vivo entre **Português (Brasil)** e **English (United States)** salvando em `language.json`.
   - **Opções:**
     - **Manual:** abre o manual embutido adaptado automaticamente à edição em uso (**Full** ou **Standart**);
     - **Sobre:** exibe **Sobre**, **Histórico**, **Licenças** *(com links oficiais)*, **Avisos**, **Política de Privacidade** e **Notas de Versão**, com busca textual instantânea.
2. **Área Superior de Entrada e Controles (`Layout 1` / `Layout 2`):** Lista de pastas e arquivos de entrada (compatível com **Arrastar e Soltar** direto do Windows Explorer), checkbox **`Atualizar arquivos existentes`** e botões rápidos (`Adicionar Pastas`, `Adicionar Arquivos`, `Testar Integridade`, `Limpar Entrada`, `Limpar Todas Saídas`).
3. **Seletor de Métodos de Compressão:** Checkboxes para exibir ou ocultar dinamicamente os quadros de `.RAR` *(Full)*, `.ZIP`, `.7Z`, `.TAR`, `.WIM` e **Extração**.
4. **Área Rolável de Destinos e Execução:** Para cada formato marcado, exibe os botões alinhados (**Especificar Saída**, **Armazenar / Extrair Arquivos**, **Limpar Saída**) e uma caixa individual de diretórios de saída com suporte a *Drag & Drop*.

---

## 2. Operações Passo a Passo

### 2.1. Compactando Arquivos e Pastas
1. **Adicione Origens:** Clique em **Adicionar Pastas** / **Adicionar Arquivos** ou arraste itens diretamente do Windows Explorer para a lista de entrada.
2. **Ajuste o Nível de Compressão:** Em **Configurações → Método de Compressão**, selecione o formato e o nível desejado (`0` para rapidez máxima sem compressão; `5` para equilíbrio; `9` ou `5 - Ótimo` no RAR para máxima redução).
3. *(Opcional)* Marque **Atualizar arquivos existentes** se quiser acrescentar/atualizar itens em pacotes já existentes com mesmo nome no destino em vez de apenas adicionar ao pacote padrão (`u` vs. `a`).
4. **Defina os Destinos:** Clique em **Especificar Saída** no quadro do formato desejado (é possível adicionar múltiplos diretórios em sequência) ou arraste pastas do Explorer diretamente para a caixa de saída do formato.
5. **Inicie a Compactação:** Clique no botão correspondente ao formato (`Armazenar .ZIP`, `Armazenar .7Z`, etc.) ou use **Arquivo → Armazenar**. Se outro trabalho do mesmo formato já estiver rodando, o novo pedido entra automaticamente na fila em segundo plano.

### 2.2. Extraindo Arquivos Compactados
1. Adicione os pacotes compactados desejados (`.rar`, `.zip`, `.7z`, `.tar`, `.wim`, `.bz2`, `.tar.xz`, `.tgz`, `.tar.gz`, `.zipx`, `.lzh`, `.iso`) à lista de entrada.
2. Marque **Extração** nos checkboxes de métodos e defina uma ou mais pastas em **Diretório(s) de destino para extração**.
3. Clique em **Extrair Arquivos** (ou **Arquivo → Extrair Arquivos**). Formatos encadeados como `.tar.xz`, `.tgz`, `.tar.gz` e `.bz2` são extraídos em duas etapas automaticamente via diretório temporário seguro, preservando a estrutura original de subpastas.

### 2.3. Testando Integridade sem Extração
1. Adicione arquivos compactados suportados à lista de entrada.
2. Clique em **Testar Integridade** (ou **Arquivo → Testar Integridade**).
3. O motor 7-Zip executa o comando `t` validando somas de verificação em segundo plano sem gravar arquivos extraídos no destino.

### 2.4. Usando o Segundo Layout (Layout Duplo Paralelo)
1. Acesse **Configurações → Layouts → Segundo Layout**.
2. Quando a janela estiver maximizada ou em formato amplo, o aplicativo exibe **duas colunas paralelas lado a lado**, cada qual com sua própria lista de entrada, destinos e fila de execução (em janelas muito estreitas/verticais, alterna automaticamente para empilhamento superior/inferior).
3. Ao alternar entre o layout simples e o duplo com a janela maximizada, o aplicativo preserva seu estado maximizado e mantém as configurações da primeira coluna intactas.

---

## 3. Persistência Local e Solução de Problemas Comuns

- **Onde ficam as configurações?** Em `%LOCALAPPDATA%\CompressionManager\`:
  - `config.json` (métodos salvos por formato);
  - `language.json` (idioma `pt_BR` ou `en_US`);
  - `logs\file_compression_yyyyMMdd_hhmmss.log` (registros diagnósticos locais).
- **7-Zip ou WinRAR não encontrados:** Verifique se a pasta `plugins\7Z\` (`7zG.exe` e `7z.dll`) acompanha o executável na pasta de instalação ou se o 7-Zip está instalado em `C:\Program Files\7-Zip\`. Na Edição Full, o empacotamento `.rar` busca `WinRAR.exe` em `C:\Program Files\WinRAR\` ou `plugins\WinRAR\`.
- **Arrastar e Soltar quando executado como Administrador:** O programa inclui ponte automática `WM_DROPFILES` compatível com UIPI para aceitar itens arrastados do Explorer mesmo em sessão elevada, embora seja recomendável executar normalmente com conta de usuário padrão.

---

## 4. Desinstalação Limpa (Windows / Microsoft Store / Inno Setup)

Você pode remover o **Compression Manager** de forma limpa a qualquer momento em **Configurações do Windows → Aplicativos → Aplicativos instalados** (ou clicando com o botão direito no **Menu Iniciar → Desinstalar**), sem deixar processos ou serviços em segundo plano. Em instalações tradicionais via Inno Setup, se desejar apagar também preferências/logs antigos após desinstalar, remova manualmente a pasta `%LOCALAPPDATA%\CompressionManager\`. *(Mantenha sempre cópias de segurança dos seus arquivos importantes e confira arquivos compactados críticos com Testar Integridade.)*

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details open>
<summary>Click to expand / collapse user guide in English</summary><br>

# Compression Manager — User Guide & Operations Manual

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  
**Author:** Fernando Nillsson Cidade  

---

> [!NOTE]
> **Full Edition vs. Standart Edition:**  
> - **Full Edition:** Supports creating **RAR**, **ZIP**, **7Z**, **TAR**, and **WIM** archives (**7-Zip** + **WinRAR**) and broad extraction including `.rar`.  
> - **Standart Edition:** Strictly focuses on open formats **ZIP**, **7Z**, **TAR**, and **WIM** powered solely by **7-Zip**, containing zero WinRAR/RAR references or binaries.

## 1. Interface Overview (`InterfaceGrafica`)

The main window organizes workflows cleanly into four integrated areas:

1. **Top Menu Bar:**
   - **File (`Arquivo`):**
     - **Add Folders:** / **Add Files:** — sequential multi-selection of input sources;
     - **Test Integrity:** — runs block-by-block checksum testing without disk extraction;
     - **Clear Input:** / **Clear All Outputs:** — clears the input list or every active destination list;
     - **Specify Output:** — adds output directories per format (`.RAR` *Full only*, `.ZIP`, `.7Z`, `.TAR`, `.WIM`, and `Extraction`);
     - **Store:** — launches archiving directly into `.RAR` *(Full only)*, `.ZIP`, `.7Z`, `.TAR`, or `.WIM`;
     - **Extract Files:** / **Clear Output:** / **Exit**.
   - **Settings (`Configurações`):**
     - **Compression Method:** persistent submenus per format saved in `config.json`:
       - `RAR (0-5)` *(Full Edition only)*: `0 - Store` through `5 - Best`;
       - `ZIP (0-9)` and `7Z (0-9)`: `0 - Store`, `1 - Fastest`, `3 - Fast`, `5 - Normal`, `7 - Maximum`, `9 - Ultra`;
       - `TAR (0)` and `WIM (0)`: `0 - Store`.
     - **Layouts:** toggles between **First Layout** *(Standard)* and **Second Layout** *(parallel Dual Layout)*;
     - **Languages:** switches live between **Português (Brasil)** and **English (United States)** backed by `language.json`.
   - **Options (`Opções`):**
     - **Manual:** opens the built-in manual tailored specifically to your edition (**Full** or **Standart**);
     - **About:** opens the tabbed About window (**History**, **Details**, **Licenses**, **Notices**, **Privacy Policy**, and **Release Notes**) featuring live text search.
2. **Top Input & Action Controls (`Layout 1` / `Layout 2`):** Source file/folder list (supporting **Drag & Drop** directly from Windows Explorer), **`Update existing archives`** checkbox, and quick action buttons (`Add Folders`, `Add Files`, `Test Integrity`, `Clear Input`, `Clear All Outputs`).
3. **Format Selector:** Checkboxes to dynamically show/hide panels for `.RAR` *(Full)*, `.ZIP`, `.7Z`, `.TAR`, `.WIM`, and **Extraction**.
4. **Scrollable Output & Execution Area:** Displays aligned action buttons (**Specify Output**, **Store / Extract Files**, **Clear Output**) paired with per-format Drag & Drop output directory lists.

---

## 2. Step-by-Step Operations

### 2.1. Archiving Files and Folders
1. **Add Sources:** Click **Add Folders** / **Add Files**, or drag items directly from Windows Explorer into the input list.
2. **Choose Compression Level:** Open **Settings → Compression Method** and select your format's compression preset (`0` for uncompressed speed; `5` for balanced size/speed; `9` / RAR `5` for maximum reduction).
3. *(Optional)* Enable **Update existing archives** to update/append files inside matching destination archives (`u`) instead of standard add (`a`).
4. **Specify Destinations:** Click **Specify Output** in a format row (supports adding multiple directories sequentially) or drag target folders from Explorer straight into the format's output list.
5. **Run Archiving:** Click the format's store button (e.g., `Store .ZIP`, `Store .7Z`) or use **File → Store**. If another task for that exact format is active, the new job queues automatically in the background.

### 2.2. Extracting Archives
1. Add supported archives (`.rar`, `.zip`, `.7z`, `.tar`, `.wim`, `.bz2`, `.tar.xz`, `.tgz`, `.tar.gz`, `.zipx`, `.lzh`, `.iso`) to the input list.
2. Enable the **Extraction** checkbox and add one or more target directories under **Extraction destination directory(ies)**.
3. Click **Extract Files** (or **File → Extract Files**). Compound compressed tarballs (`.tar.xz`, `.tgz`, `.tar.gz`, `.bz2`) automatically extract cleanly in two stages via a temporary working folder while preserving folder structure.

### 2.3. Non-Destructive Integrity Testing
1. Add compressed files to the input list.
2. Click **Test Integrity** (or **File → Test Integrity**).
3. 7-Zip runs its `t` verification pass in a background thread to inspect headers and block checksums without writing extracted files to disk.

### 2.4. Using the Second Layout (Parallel Dual Layout)
1. Open **Settings → Layouts → Second Layout**.
2. On wide/maximized windows, two independent columns appear **side-by-side**, each managing its own input items, output paths, and background queue (on tall/narrow portrait windows, it automatically stacks vertically).
3. Toggling Dual Layout while maximized keeps the window maximized seamlessly and leaves Column 1 selections untouched.

---

## 3. Local Persistence & Troubleshooting

- **Where are settings stored?** Under `%LOCALAPPDATA%\CompressionManager\`:
  - `config.json` (saved method levels per format);
  - `language.json` (`pt_BR` or `en_US`);
  - `logs\file_compression_yyyyMMdd_hhmmss.log` (local diagnostic logs).
- **7-Zip or WinRAR Not Found:** Verify `plugins\7Z\` (`7zG.exe`, `7z.dll`) exists alongside `Compression_Manager.exe` or that 7-Zip is installed under `C:\Program Files\7-Zip\`. For `.rar` archiving in Full Edition, `WinRAR.exe` is resolved from `C:\Program Files\WinRAR\` or `plugins\WinRAR\`.
- **Drag & Drop Under Administrator Elevation:** A built-in UIPI-aware `WM_DROPFILES` event filter allows dragging from Explorer even when running elevated, though standard user privileges are recommended.

---

## 4. Clean Uninstallation (Windows / Microsoft Store / Inno Setup)

You may cleanly uninstall and remove **Compression Manager** at any time via **Windows Settings → Apps → Installed Apps** (or Start Menu → Uninstall), leaving zero background processes or services behind. For standalone Inno Setup installs, if you also wish to wipe local settings/logs afterwards, manually delete `%LOCALAPPDATA%\CompressionManager\`. *(Always keep independent backups of important data and verify critical archives using Test Integrity.)*

</details>
