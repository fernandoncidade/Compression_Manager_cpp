# Modelo de Pull Request / Pull Request Template — Compression Manager

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

> [!NOTE]
> **Aviso ao Colaborador / Contributor Notice:**  
> 🇧🇷 *Este repositório é um projeto pessoal mantido pelo autor em ritmo próprio e caráter voluntário sob a licença **GNU AGPLv3**. Pull Requests são muito bem-vindos, porém a revisão e incorporação ocorrem conforme disponibilidade e sem compromisso de prazo. Você tem total liberdade para manter e evoluir suas alterações em um fork próprio nos termos da GNU AGPLv3.*  
> 🇺🇸 *This repository is a personal project maintained by the author on a voluntary basis at their own pace under **GNU AGPLv3**. Pull Requests are welcome, though review and merging happen according to availability without guaranteed timelines. You have full freedom to maintain and extend your improvements in your own fork under GNU AGPLv3 terms.*

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details>
<summary>Clique para expandir / recolher o template em português</summary><br>

## Descrição
Descreva sucintamente o objetivo deste Pull Request, quais módulos foram alterados (`InterfaceCore`, `GerenciamentoUI`, `MotoresCompressao`, `language`, `utils`, `cmake/`, `library/` ou `docs/`) e qual issue ele soluciona.

## Tipo de Alteração
- [ ] Correção de bug funcional ou de interface (ex.: Layout Duplo, janela maximizada, Drag & Drop)
- [ ] Nova funcionalidade ou melhoria nos motores de compressão (`7-Zip` / `WinRAR`)
- [ ] Endurecimento de segurança, filas assíncronas (`QThread`/`QProcess`) ou persistência local
- [ ] Otimização de build CMake / distribuições ultralimpas / scripts Inno Setup 7
- [ ] Atualização ou melhoria de documentação bilíngue e conformidade legal
- [ ] Internacionalização (`source/language/translations/*.ts` -> `.qm`)

## Compilação e Verificação
- [ ] Compilado limpo na Edição Full (`cmake --preset full-msvc-release` e/ou `full-mingw-release`)
- [ ] Compilado limpo na Edição Standart (`cmake --preset standart-msvc-release` e/ou `standart-mingw-release`) confirmando ausência total de WinRAR/RAR
- [ ] Executado `find_VersionEditor.exe --check` e validada ausência de `vc_redist.x64.exe` e `rarreg.key` nas distribuições/instaladores

## Conformidade Arquitetural e de Segurança
- [ ] A arquitetura permanece estritamente **100% local e offline**, sem introdução de telemetria ou conexões de rede.
- [ ] Nenhum arquivo proprietário de chave/licença (ex.: `rarreg.key`) ou DLL Qt desnecessária foi incluído no pacote de saída.
- [ ] A edição Standart (`ENABLE_RAR_SUPPORT=0`) preserva segregação total contra dependências proprietárias RAR/WinRAR.

## Issues Relacionadas
Corrige #(número da issue)

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details>
<summary>Click to expand / collapse template in English</summary><br>

## Description
Briefly describe the purpose of this Pull Request, affected subsystems (`InterfaceCore`, `GerenciamentoUI`, `MotoresCompressao`, `language`, `utils`, `cmake/`, `library/`, or `docs/`), and which issue it resolves.

## Type of Change
- [ ] Functional or UI bug fix (e.g., Dual Layout, maximized preservation, Drag & Drop)
- [ ] New feature or enhancement in compression engines (`7-Zip` / `WinRAR`)
- [ ] Security hardening, worker threading (`QThread`/`QProcess`), or local persistence
- [ ] CMake build / ultra-lean deployment / Inno Setup 7 packaging optimization
- [ ] Bilingual documentation or dual-licensing compliance update
- [ ] Internationalization / localization updates (`source/language/translations/*.ts` -> `.qm`)

## Build & Verification
- [ ] Clean build on Full Edition (`cmake --preset full-msvc-release` and/or `full-mingw-release`)
- [ ] Clean build on Standart Edition (`cmake --preset standart-msvc-release` and/or `standart-mingw-release`) verifying complete absence of WinRAR/RAR
- [ ] Run `find_VersionEditor.exe --check` and verified absence of `vc_redist.x64.exe` and `rarreg.key` in deployments/installers

## Architectural & Security Compliance
- [ ] Remains strictly **100% local-first and offline** with zero telemetry or network connections.
- [ ] No proprietary key files (e.g., `rarreg.key`) or unused Qt modules leak into runtime distributions.
- [ ] Standart Edition (`ENABLE_RAR_SUPPORT=0`) maintains complete isolation from WinRAR/RAR references.

## Related Issues
Fixes #(issue number)

</details>
