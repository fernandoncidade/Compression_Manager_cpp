# MANUAL DO USUÁRIO — Compression Manager

Versão documentada: `2026.11.22.0`
Data: `22 de novembro de 2026`

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">Português (BR)</a> |
  <a href="#enus">English (US)</a>
</p>

> [!NOTE]
> **Duplo Licenciamento / Dual Licensing & Isenção ("AS IS"):** Código-fonte aberto em **C++23** e **Qt 6.12.0** sob [**GNU AGPLv3**](LICENSE); pacotes oficiais assinados (Microsoft Store / instaladores do autor) regidos pelo [**EULA Oficial**](source/assets/EULA/EULA_pt_BR%20-%20Compression%20Manager.txt) e [**CLC**](source/assets/CLC/CLC_pt_BR%20-%20Compression%20Manager.txt). Utilize sempre o teste de integridade e mantenha cópias de segurança independentes dos seus arquivos críticos. Consulte também [`docs/user-guide.md`](docs/user-guide.md).

---

## <a id="ptbr"></a>Português (Brasil)

Este manual descreve a operação, funcionalidades e orientações de uso do **Compression Manager**, utilitário multiformato de empacotamento, extração e verificação de integridade desenvolvido nativamente em **C++23** com **Qt 6.12.0 Widgets**.

O aplicativo é disponibilizado em duas edições independentes: a **Edição Full** (`ENABLE_RAR_SUPPORT=1`) e a **Edição Standart** (`ENABLE_RAR_SUPPORT=0`). A seguir, cada edição é detalhada em sua respectiva seção exclusiva.

---

# PARTE 1: EDIÇÃO FULL

A **Edição Full** é a versão completa do aplicativo, contendo suporte a todos os formatos de compressão (incluindo o formato proprietário **RAR** via integração externa com **WinRAR**) e aos formatos abertos padrão (**ZIP**, **7Z**, **TAR** e **WIM** via **7-Zip**).

### 1.1. Recursos e Formatos da Edição Full
- **Formatos de Empacotamento:** RAR, ZIP, 7Z, TAR e WIM.
- **Formatos de Extração:** `.rar`, `.zip`, `.7z`, `.bz2`, `.tar`, `.wim`, `.tar.xz`, `.zipx`, `.tgz`, `.tar.gz`, `.lzh` e `.iso`.
- **Motores Integrados:** Suporte conjunto ao **7-Zip** (`7zG.exe` / `7z.exe`) e **WinRAR** (`WinRAR.exe`, sem `rarreg.key`).
- **Níveis de Compressão:**
  - **RAR (`0–5`):** `0 - Armazenar`, `1 - Mais Rápido`, `2 - Rápido`, `3 - Normal`, `4 - Bom`, `5 - Ótimo`.
  - **ZIP (`0–9`) e 7Z (`0–9`):** `0 - Armazenar`, `1 - Mais Rápido`, `3 - Rápido`, `5 - Normal`, `7 - Máximo`, `9 - Ultra`.
  - **TAR (`0`) e WIM (`0`):** `0 - Armazenar`.
- **Modos de Layout:** Layout Padrão (**Primeiro Layout**) e Layout Duplo Paralelo (**Segundo Layout**) com orientação adaptativa e preservação da janela maximizada.
- **Processamento Assíncrono:** Threads `QThread` + fila `QQueue` por formato para manter a interface responsiva.
- **Múltiplos Destinos:** Listas independentes de pastas de saída para cada formato e para extração.
- **Arrastar e Soltar (Drag & Drop):** Suporte direto do Windows Explorer (inclusive em execuções elevadas via ponte UIPI `WM_DROPFILES`).
- **Verificação de Integridade:** Teste de somas de verificação bloco a bloco (`t`) sem descompactar no disco.
- **Internacionalização:** Troca instantânea de idioma entre Português e Inglês sem reiniciar.
- **Privacidade e Modo Offline:** Execução 100% local com persistência em `%LOCALAPPDATA%\CompressionManager\`.

### 1.2. Como Usar a Edição Full

#### A. Empacotando Arquivos e Pastas em RAR, ZIP, 7Z, TAR ou WIM
1. **Adicionar Itens:**
   - Clique em **Adicionar Pastas** ou **Adicionar Arquivos** (ou no menu **Arquivo → Adicionar Pastas / Adicionar Arquivos**).
   - Ou arraste arquivos e pastas do Windows Explorer diretamente para a lista de entrada.
2. **Escolher o Formato e Painel:**
   - Marque o formato desejado nos checkboxes de métodos (**RAR**, **ZIP**, **7Z**, **TAR** ou **WIM**) para exibir o respectivo painel de saída.
3. **Ajustar Nível de Compressão:**
   - Acesse **Configurações → Método de Compressão**, escolha o formato e selecione o nível desejado (persistido automaticamente em `config.json`).
4. **Atualizar Arquivo Existente (Opcional):**
   - Marque a opção **Atualizar arquivos existentes** para acrescentar ou atualizar arquivos em pacotes já existentes com mesmo nome no destino (`u`) em vez de apenas adicionar (`a`).
5. **Definir Diretório(s) de Destino:**
   - Clique em **Especificar Saída** (ou **Arquivo → Especificar Saída**) para adicionar um ou mais diretórios de saída, ou arraste pastas do Explorer direto para a caixa de saída do formato.
6. **Disparar a Operação:**
   - Clique no botão de armazenamento do formato (ou **Arquivo → Armazenar**). A tarefa executa em segundo plano e, caso já exista outra tarefa ativa para o mesmo formato, é colocada em fila automaticamente.

#### B. Extraindo Arquivos Compactados (`.rar`, `.zip`, `.7z`, etc.)
1. Adicione o arquivo compactado desejado à lista de entrada.
2. Marque **Extração** e selecione ou arraste a(s) pasta(s) de destino em **Diretório(s) de destino para extração**.
3. Clique em **Extrair Arquivos** (ou **Arquivo → Extrair Arquivos**). Arquivos `.bz2`, `.tar.xz`, `.tgz` e `.tar.gz` são extraídos em duas etapas automaticamente preservando a estrutura de pastas.

#### C. Testando a Integridade de Arquivos Compactados
1. Adicione os pacotes compactados suportados na lista de entrada.
2. Clique em **Testar Integridade** (ou **Arquivo → Testar Integridade**).
3. O motor 7-Zip valida o arquivo bloco a bloco em segundo plano sem extrair dados para o disco.

#### D. Alternando entre Primeiro Layout e Segundo Layout (Layout Duplo)
1. Acesse **Configurações → Layouts → Segundo Layout**.
2. Em telas largas ou janela maximizada, duas colunas independentes são exibidas lado a lado (em janelas muito estreitas/verticais, o programa alterna automaticamente para empilhamento vertical).
3. Ao alternar com a janela maximizada, o aplicativo mantém `isMaximized()` e preserva integralmente suas filas e caminhos na primeira coluna.

#### E. Troca de Idioma e Desinstalação Limpa
- Em **Configurações → Idiomas**, escolha **Português (Brasil)** ou **English (United States)**.
- A desinstalação limpa ocorre por **Configurações do Windows → Aplicativos → Aplicativos instalados** (ou Menu Iniciar → Desinstalar).

---

# PARTE 2: EDIÇÃO STANDART

A **Edição Standart** é compilada com `ENABLE_RAR_SUPPORT=0`, dedicada exclusivamente a formatos abertos e livres (**ZIP**, **7Z**, **TAR** e **WIM** via **7-Zip** puro), sem qualquer código, menu, ícone ou dependência proprietária do WinRAR ou formato RAR.

### 2.1. Recursos e Formatos da Edição Standart
- **Formatos de Empacotamento:** ZIP, 7Z, TAR e WIM.
- **Formatos de Extração:** `.zip`, `.7z`, `.bz2`, `.tar`, `.wim`, `.tar.xz`, `.zipx`, `.tgz`, `.tar.gz`, `.lzh` e `.iso`.
- **Motor Integrado:** **7-Zip** (`7zG.exe` / `7z.exe`).
- **Níveis de Compressão:** ZIP/7Z (`0 - Armazenar`, `1 - Mais Rápido`, `3 - Rápido`, `5 - Normal`, `7 - Máximo`, `9 - Ultra`), TAR (`0 - Armazenar`) e WIM (`0 - Armazenar`).
- **Modos de Layout, Drag & Drop UIPI, Fila Assíncrona, Integridade (`t`), Idioma ao Vivo e Operação Offline:** Idênticos à Edição Full para os formatos abertos suportados.

### 2.2. Como Usar a Edição Standart
Siga os mesmos passos **A** a **E** descritos na Parte 1 utilizando os formatos abertos **ZIP**, **7Z**, **TAR** e **WIM** e a seção **Extração**.

---

## <a id="enus"></a>English (US)

This manual provides operational instructions for **Compression Manager**, a multiformat archiving, extraction, and integrity-testing desktop utility built natively in **C++23** with **Qt 6.12.0 Widgets**.

The software ships in two strictly segregated product editions: **Full Edition** (`ENABLE_RAR_SUPPORT=1`) and **Standart Edition** (`ENABLE_RAR_SUPPORT=0`).

---

# PART 1: FULL EDITION

The **Full Edition** includes full support for proprietary **RAR** archiving via external **WinRAR** integration alongside open formats (**ZIP**, **7Z**, **TAR**, and **WIM** via **7-Zip**).

### 1.1. Full Edition Features & Formats
- **Archiving Formats:** RAR, ZIP, 7Z, TAR, and WIM.
- **Extraction Formats:** `.rar`, `.zip`, `.7z`, `.bz2`, `.tar`, `.wim`, `.tar.xz`, `.zipx`, `.tgz`, `.tar.gz`, `.lzh`, and `.iso`.
- **Engines:** Dual **7-Zip** (`7zG.exe` / `7z.exe`) and **WinRAR** (`WinRAR.exe`, zero `rarreg.key`).
- **Compression Levels:**
  - **RAR (`0–5`):** `0 - Store` through `5 - Best`.
  - **ZIP (`0–9`) & 7Z (`0–9`):** `0 - Store`, `1 - Fastest`, `3 - Fast`, `5 - Normal`, `7 - Maximum`, `9 - Ultra`.
  - **TAR (`0`) & WIM (`0`):** `0 - Store`.
- **Layout Modes:** **First Layout** (Standard) and parallel **Second Layout** (Dual Layout) with responsive horizontal/vertical orientation and maximized state preservation.
- **Asynchronous Execution:** `QThread` + per-format `QQueue` queueing keeping the UI responsive.
- **Multiple Destinations, UIPI-Aware Drag & Drop, Non-Destructive Integrity Checks (`t`), Live Language Switching (`pt_BR` / `en_US`), and 100% Offline Privacy** under `%LOCALAPPDATA%\CompressionManager\`.

### 1.2. How to Use the Full Edition
1. **Add Sources:** Click **Add Folders** / **Add Files** or drag and drop directly from Windows Explorer.
2. **Select Formats & Levels:** Check active format boxes (**RAR**, **ZIP**, **7Z**, **TAR**, **WIM**, or **Extraction**) and adjust compression levels under **Settings → Compression Method**.
3. *(Optional)* Toggle **Update existing archives** (`u` vs. `a`).
4. **Specify Destinations:** Use **Specify Output** (or drag folders into a format output box) and trigger archiving (`Store ...`), **Extract Files**, or **Test Integrity**.
5. **Dual Layout & Languages:** Enable **Settings → Layouts → Second Layout** for side-by-side operation (preserving `isMaximized()`) and toggle UI language live under **Settings → Languages**. Clean uninstallation is available via **Windows Settings → Apps → Installed Apps**.

---

# PART 2: STANDART EDITION

The **Standart Edition** is compiled with `ENABLE_RAR_SUPPORT=0` and focuses exclusively on open standard formats (**ZIP**, **7Z**, **TAR**, and **WIM** via pure **7-Zip**), containing zero WinRAR binaries, icons, or RAR references.

### 2.1. Standart Edition Features & Formats
- **Archiving Formats:** ZIP, 7Z, TAR, and WIM.
- **Extraction Formats:** `.zip`, `.7z`, `.bz2`, `.tar`, `.wim`, `.tar.xz`, `.zipx`, `.tgz`, `.tar.gz`, `.lzh`, and `.iso`.
- **Integrated Engine:** **7-Zip** (`7zG.exe` / `7z.exe`) only.
- **Workflows:** Identical to the Full Edition across all supported open formats, Dual Layout switching, Drag & Drop, integrity testing, live localization, and clean offline operation.
