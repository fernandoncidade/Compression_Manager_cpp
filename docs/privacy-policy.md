# Política de Privacidade / Privacy Policy — Compression Manager

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details open>
<summary>Clique para expandir / recolher a Política de Privacidade em português</summary><br>

# POLÍTICA DE PRIVACIDADE — COMPRESSION MANAGER

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  
**Desenvolvedor / Controlador:** Fernando Nillsson Cidade (Desenvolvedor Individual)  
**E-mail de Contato e Privacidade:** `linceu_lighthouse@outlook.com`  
**Repositório Oficial:** [https://github.com/fernandoncidade/Compression_Manager_cpp](https://github.com/fernandoncidade/Compression_Manager_cpp)

---

## 1. Declaração de Independência frente à Política de Privacidade da Microsoft

> [!IMPORTANT]
> **A Microsoft NÃO fornece uma política de privacidade padrão para o Compression Manager, e este aplicativo NÃO é coberto por nenhuma política de privacidade da Microsoft.**
> - Esta Política de Privacidade aplica-se exclusiva e integralmente ao software **Compression Manager** em todas as suas formas de distribuição (pacote MSIX da Microsoft Store, instaladores Windows Inno Setup ou compilação a partir do código-fonte).
> - Ao navegar, baixar, instalar, atualizar, desinstalar ou avaliar o aplicativo na **Microsoft Store**, ou utilizar diagnósticos do sistema operacional **Windows**, os dados tratados pela própria Microsoft são regidos pela [Política de Privacidade da Microsoft](https://www.microsoft.com/pt-br/privacy/privacystatement).
> - Além disso, a Microsoft não presta suporte técnico aos usuários do Compression Manager; dúvidas sobre privacidade ou uso devem ser enviadas diretamente ao desenvolvedor por e-mail ou pelo repositório oficial no GitHub.

---

## 2. Arquitetura Local-First e Ausência de Coleta de Dados Pessoais

O **Compression Manager** foi concebido para operar **100% localmente e offline** na estação Windows do usuário:

1. **Zero Coleta de Dados Pessoais (PII):** O software **NÃO** coleta, solicita, lê remotamente, envia, compartilha ou vende nome, CPF/CNPJ, endereço, telefone, e-mail, localização geográfica (GPS/IP), histórico de navegação ou identificadores publicitários.
2. **Zero Rede, Zero Telemetria e Zero Contas:** O código em C++23 e Qt 6.12.0 não exige login, não contém SDKs analíticos, rastreadores, anúncios, conexões com nuvem ou requisições HTTP/HTTPS em segundo plano *(além de não empacotar `Qt6Network.dll` nem plugins `tls`/`networkinformation` nas distribuições enxutas)*.
3. **Sem Coleta de Dados de Crianças ou Dados Sensíveis:** Em conformidade com as Políticas da Microsoft Store (Seções 10.5.5 a 10.5.8), não há coleta de dados financeiros, de saúde, biométricos ou informações pessoais de crianças.

---

## 3. Dados Processados e Persistidos Localmente

Todo processamento de empacotamento, descompactação e verificação de integridade ocorre exclusivamente na memória e no disco local sob comando direto do usuário:

- **Caminhos de Entrada e Destino Selecionados:** Pastas e arquivos adicionados explicitamente via diálogos nativos ou *Drag & Drop*, além dos diretórios de saída especificados por formato (`.rar` na Edição Full; `.zip`, `.7z`, `.tar`, `.wim` e `Extração` em ambas as edições).
- **Preferências Locais (`%LOCALAPPDATA%\CompressionManager\`):**
  - `language.json`: idioma ativo da interface (`pt_BR` ou `en_US`), gerenciado por `GerenciadorTraducao`;
  - `config.json`: métodos/níveis de compressão selecionados por formato (`compress_type_rar`, `compress_type_zip`, `compress_type_7z`, `compress_type_tar`, `compress_type_wim`), gerenciado por `MetodoCompressao`. *(Em execuções via MSIX, gravado no contêiner local virtualizado do pacote.)*
- **Logs Técnicos Locais:** Registros locais de execução gravados por `LogManager` em `%LOCALAPPDATA%\CompressionManager\logs\file_compression_yyyyMMdd_hhmmss.log`, estritamente restritos à máquina do usuário.
- **Invocação Local de Motores (`7-Zip` / `WinRAR`):** Os sub-processos locais recebem apenas os argumentos da operação solicitada pelo usuário e operam sem acesso de rede pelo aplicativo. Nenhum arquivo de licença pessoal `rarreg.key` acompanha as distribuições.

---

## 4. Capacidade Declarada no Manifesto MSIX (`runFullTrust`)

Em conformidade com a Política 10.6 da Microsoft Store, o manifesto [`library/AppxManifest.xml`](../library/AppxManifest.xml) (`fernandoncidade.COMPRESSIONMANAGER`) declara apenas a capacidade restrita `<rescap:Capability Name="runFullTrust" />`, necessária porque o Compression Manager é uma aplicação desktop nativa Win32 (C++23 / Qt 6.12.0) que aciona diálogos padrão de sistema e os executáveis locais do 7-Zip (e WinRAR na Edição Full). Nenhuma permissão de câmera, microfone, localização ou contatos é solicitada.

---

## 5. Direitos do Titular e Desinstalação Limpa (Política 10.2.7 / LGPD / GDPR)

O processamento local baseia-se exclusivamente na execução de funcionalidades solicitadas pelo usuário (art. 7º, V e IX, da LGPD — Lei nº 13.709/2018; execução e interesse legítimo sob o GDPR):

- **Acesso e Remoção de Dados Locais:** O usuário pode inspecionar, copiar, mover ou apagar seus arquivos compactados, `config.json`, `language.json` e `logs/` em `%LOCALAPPDATA%\CompressionManager\` a qualquer instante.
- **Desinstalação Limpa:** O aplicativo pode ser desinstalado de forma limpa a qualquer momento em **Configurações do Windows → Aplicativos → Aplicativos instalados** (ou Menu Iniciar → Desinstalar), removendo integralmente os binários e o estado MSIX sem deixar serviços, drivers ou agentes residentes em segundo plano.

---

## 6. Contato do Responsável pela Privacidade

- **Responsável:** Fernando Nillsson Cidade
- **E-mail:** `linceu_lighthouse@outlook.com`
- **Repositório / Issues:** [https://github.com/fernandoncidade/Compression_Manager_cpp/issues](https://github.com/fernandoncidade/Compression_Manager_cpp/issues)

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details open>
<summary>Click to expand / collapse Privacy Policy in English</summary><br>

# PRIVACY POLICY — COMPRESSION MANAGER

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  
**Developer / Controller:** Fernando Nillsson Cidade (Individual Developer)  
**Privacy & Support Email:** `linceu_lighthouse@outlook.com`  
**Official Repository:** [https://github.com/fernandoncidade/Compression_Manager_cpp](https://github.com/fernandoncidade/Compression_Manager_cpp)

---

## 1. Independence from Microsoft Privacy Statement

> [!IMPORTANT]
> **Microsoft does NOT provide a default privacy policy for Compression Manager, and this application is NOT covered by any Microsoft privacy policy.**
> - This Privacy Policy applies exclusively and in full to **Compression Manager** across all distribution formats (Microsoft Store MSIX package, Inno Setup Windows installers, or builds compiled from source code).
> - Data collected separately by Microsoft when browsing, downloading, installing, updating, uninstalling, or rating the app in the **Microsoft Store**, or via optional **Windows** OS diagnostic settings, is governed solely by the [Microsoft Privacy Statement](https://www.microsoft.com/en-us/privacy/privacystatement).
> - Microsoft does not provide customer support to Compression Manager users; privacy or operational questions should be sent directly to the developer via email or the official GitHub repository.

---

## 2. Local-First Architecture & Zero Personal Data Collection

**Compression Manager** is engineered to operate **100% locally and offline** on the user's Windows workstation:

1. **Zero Personal Data Collection (PII):** The software does **NOT** collect, request, remotely read, transmit, share, or sell names, national/taxpayer IDs, addresses, phone numbers, email addresses, geographic location (GPS/IP), browsing history, or advertising identifiers.
2. **Zero Network, Zero Telemetry, and Zero Accounts:** The C++23 / Qt 6.12.0 codebase requires no user login and contains zero analytics SDKs, trackers, ads, cloud servers, or background HTTP/HTTPS requests *(and lean deployments deliberately omit `Qt6Network.dll` and `tls`/`networkinformation` plugins)*.
3. **No Collection of Children's or Sensitive Data:** Compliant with Microsoft Store Policies (Sections 10.5.5 to 10.5.8), collecting no financial, health, biometric, or children's personal data.

---

## 3. Data Processed and Persisted Locally

All packaging, extraction, and checksum verification takes place strictly in local RAM and disk under direct user command:

- **User-Selected Input & Target Paths:** Folders and files explicitly added via file dialogs or Drag & Drop, along with user-selected output directories per format (`.rar` in Full Edition; `.zip`, `.7z`, `.tar`, `.wim`, and `Extraction` in both editions).
- **Local Preferences (`%LOCALAPPDATA%\CompressionManager\`):**
  - `language.json`: active UI language (`pt_BR` or `en_US`), persisted by `GerenciadorTraducao`;
  - `config.json`: selected per-format compression methods/levels (`compress_type_rar`, `compress_type_zip`, `compress_type_7z`, `compress_type_tar`, `compress_type_wim`), managed by `MetodoCompressao`. *(When running inside MSIX, stored in the package's isolated local app container.)*
- **Local Diagnostic Logs:** Execution logs written by `LogManager` to `%LOCALAPPDATA%\CompressionManager\logs\file_compression_yyyyMMdd_hhmmss.log`, strictly local to the user's workstation.
- **Local Engine Invocation (`7-Zip` / `WinRAR`):** Local child processes receive only the CLI arguments for user-triggered archive jobs with zero network activity from the app. No personal `rarreg.key` registration key is ever distributed.

---

## 4. Capability Declared in the MSIX Manifest (`runFullTrust`)

Pursuant to Microsoft Store Policy 10.6, [`library/AppxManifest.xml`](../library/AppxManifest.xml) (`fernandoncidade.COMPRESSIONMANAGER`) declares exclusively `<rescap:Capability Name="runFullTrust" />`, strictly needed because Compression Manager is a native Win32 C++23 / Qt 6.12.0 desktop application invoking standard Windows dialogs and bundled 7-Zip (plus WinRAR in the Full edition) subprocesses. No camera, microphone, location, or contacts permissions are declared or accessed.

---

## 5. Data Subject Rights & Clean Uninstallation (Policy 10.2.7 / LGPD / GDPR)

Local processing relies strictly on user-requested execution (Brazilian LGPD Art. 7, V and IX; performance and legitimate interest under GDPR):

- **Data Access and Manual Removal:** Users may freely inspect, copy, move, or delete created archives as well as `config.json`, `language.json`, and `logs/` inside `%LOCALAPPDATA%\CompressionManager\` at any time.
- **Clean Uninstallation:** Users may cleanly uninstall Compression Manager at any time via **Windows Settings → Apps → Installed Apps** (or Start Menu → Uninstall), completely removing binaries, plugins, and isolated MSIX storage without leaving background services, drivers, or startup agents behind.

---

## 6. Privacy & Support Contact

- **Developer:** Fernando Nillsson Cidade
- **Email:** `linceu_lighthouse@outlook.com`
- **Support / Issues:** [https://github.com/fernandoncidade/Compression_Manager_cpp/issues](https://github.com/fernandoncidade/Compression_Manager_cpp/issues)

</details>
