#include "ui_07_Manual.hpp"
#include "source/utils/LogManager.hpp"
#include "source/language/tr_01_gerenciadorTraducao.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"

#include <QCoreApplication>
#include <QEvent>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>

#if ENABLE_RAR_SUPPORT

const QString MANUAL_TEXT_PT_BR =
    "<style>"
    "body { font-family: 'Segoe UI', Tahoma, Arial, sans-serif; line-height: 1.5; color: #222; margin: 12px; }"
    "h2 { color: #005a9e; border-bottom: 2px solid #005a9e; padding-bottom: 4px; margin-top: 0; }"
    "h3 { color: #106ebe; margin-top: 18px; margin-bottom: 6px; }"
    "h4 { color: #2b579a; margin-top: 12px; margin-bottom: 4px; }"
    "ul, ol { margin-top: 4px; margin-bottom: 8px; padding-left: 22px; }"
    "li { margin-bottom: 4px; }"
    "b { color: #003a6c; }"
    ".note { background: #f3f2f1; border-left: 4px solid #005a9e; padding: 8px 12px; margin: 10px 0; border-radius: 0 4px 4px 0; }"
    "</style>"
    "<h2>COMPRESSION MANAGER (Edição Full) — Manual do Usuário</h2>"
    "<p>Bem-vindo ao <b>Compression Manager (Edição Full)</b>, seu utilitário completo para compactação, extração e verificação de arquivos e diretórios em múltiplos formatos.</p>"
    
    "<h3>1. Como Empacotar Arquivos e Pastas</h3>"
    "<p>O processo de empacotamento é simples e flexível:</p>"
    "<ol>"
    "    <li><b>Adição de Itens:</b> Clique nos botões <i>'Adicionar Arquivos'</i> ou <i>'Adicionar Pastas'</i> para selecionar o conteúdo desejado. Você também pode arrastar e soltar arquivos e pastas diretamente do Windows Explorer para dentro da lista de entrada.</li>"
    "    <li><b>Seleção do Formato:</b> No painel de métodos ou no menu superior, escolha o formato de sua preferência: <b>RAR</b>, <b>ZIP</b>, <b>7Z</b>, <b>TAR</b> ou <b>WIM</b>.</li>"
    "    <li><b>Configuração do Método de Compressão:</b> Em <i>Configurações &gt; Selecionar Método de Compressão</i>, selecione o formato e defina o nível desejado:"
    "        <ul>"
    "            <li><b>Store (0):</b> Sem compressão (armazenamento instantâneo).</li>"
    "            <li><b>Fastest (1) / Fast (3):</b> Compactação rápida com baixo consumo de processamento.</li>"
    "            <li><b>Normal (5):</b> Equilíbrio ideal entre tempo e taxa de redução.</li>"
    "            <li><b>Maximum (7) / Ultra (9):</b> Máxima redução de tamanho para economia de espaço.</li>"
    "        </ul>"
    "    </li>"
    "    <li><b>Opção de Atualização:</b> Marque a opção <i>'Atualizar arquivos existentes'</i> se quiser acrescentar ou atualizar arquivos em um pacote já existente sem precisar recriá-lo do zero.</li>"
    "    <li><b>Diretórios de Destino:</b> Escolha o diretório onde o arquivo compactado será salvo. É possível cadastrar e alternar entre múltiplos destinos independentes.</li>"
    "    <li><b>Início da Compressão:</b> Clique no botão <i>'Armazenar [Formato]'</i> (por exemplo, <i>Armazenar RAR</i> ou <i>Armazenar ZIP</i>). A tarefa será processada em segundo plano na fila assíncrona, mantendo a interface ágil e responsiva.</li>"
    "</ol>"

    "<h3>2. Como Extrair Arquivos Compactados</h3>"
    "<ol>"
    "    <li>Adicione o arquivo compactado desejado (<b>.rar</b>, <b>.zip</b>, <b>.7z</b>, <b>.tar</b>, <b>.wim</b>, <b>.iso</b>, etc.) à lista de entrada.</li>"
    "    <li>No quadrante ou aba de Extração, defina a pasta de destino onde deseja salvar os itens extraídos.</li>"
    "    <li>Clique no botão <i>'Extrair Arquivos'</i>. O aplicativo processará a extração preservando a estrutura original de pastas e arquivos.</li>"
    "</ol>"

    "<h3>3. Como Verificar a Integridade de Pacotes</h3>"
    "<p>Para certificar-se de que um arquivo compactado não foi corrompido durante transferências ou armazenamento:</p>"
    "<ul>"
    "    <li>Selecione o arquivo compactado na lista de entrada.</li>"
    "    <li>Acione o botão de <b>Verificação de Integridade</b>. O teste de integridade é realizado bloco a bloco em segundo plano, validando as somas de verificação sem descompactar nada no disco.</li>"
    "    <li>Ao final, uma mensagem informativa confirma se o pacote está íntegro ou relata eventuais falhas.</li>"
    "</ul>"

    "<h3>4. Modos de Layout e Adaptação Responsiva</h3>"
    "<p>O aplicativo oferece dois modos de visualização para acomodar suas necessidades de trabalho:</p>"
    "<ul>"
    "    <li><b>Layout Padrão (Layout 1):</b> Disposição clássica focada em fluxo individual, ideal para operações rápidas e telas compactas.</li>"
    "    <li><b>Segundo Layout / Layout Duplo (Layout 2):</b> Para ativar, acesse o menu superior em <i>Configurações &gt; Layouts &gt; Segundo Layout</i> (ou utilize o botão de alternância de layout). A tela divide-se em conjuntos paralelos de quadrantes, permitindo configurar e acompanhar dois formatos de compressão distintos simultaneamente (por exemplo, empacotar em RAR e em ZIP lado a lado) sem sobreposição de abas.</li>"
    "    <li><b>Adaptação Automática de Orientação:</b> No Segundo Layout, a interface monitora o espaço disponível na janela. Ao redimensionar a janela ou utilizá-la maximizada, os painéis reorganizam-se de forma dinâmica na horizontal (lado a lado em telas largas) ou na vertical (empilhados em janelas mais estreitas), proporcionando rolagem suave e ergonomia visual.</li>"
    "    <li><b>Alternância com Janela Maximizada:</b> Você pode ativar e desativar o Segundo Layout livremente a qualquer momento, inclusive com a janela maximizada em tela cheia. O aplicativo mantém o estado maximizado e preserva integralmente todas as suas seleções de arquivos, listas de pastas e configurações de compressão.</li>"
    "</ul>"

    "<h3>5. Idioma e Preferências do Usuário</h3>"
    "<ul>"
    "    <li><b>Troca Instantânea de Idioma:</b> Acesse <i>Configurações &gt; Idiomas</i> no menu superior e alterne entre <b>Português (Brasil)</b> e <b>Inglês (Estados Unidos)</b> a qualquer momento, sem precisar reiniciar o aplicativo.</li>"
    "    <li><b>Persistência Segura e Modo Offline:</b> Todas as suas configurações, últimos formatos utilizados e listas de destinos são salvos de forma segura no seu perfil local do usuário. O aplicativo funciona de modo 100% offline, garantindo total privacidade aos seus dados.</li>"
    "    <li><b>Detecção dos Motores:</b> O sistema busca automaticamente os motores <b>7-Zip</b> e <b>WinRAR</b> nas pastas padrão do Windows e na pasta local do aplicativo.</li>"
    "</ul>";

const QString MANUAL_TEXT_EN_US =
    "<style>"
    "body { font-family: 'Segoe UI', Tahoma, Arial, sans-serif; line-height: 1.5; color: #222; margin: 12px; }"
    "h2 { color: #005a9e; border-bottom: 2px solid #005a9e; padding-bottom: 4px; margin-top: 0; }"
    "h3 { color: #106ebe; margin-top: 18px; margin-bottom: 6px; }"
    "h4 { color: #2b579a; margin-top: 12px; margin-bottom: 4px; }"
    "ul, ol { margin-top: 4px; margin-bottom: 8px; padding-left: 22px; }"
    "li { margin-bottom: 4px; }"
    "b { color: #003a6c; }"
    ".note { background: #f3f2f1; border-left: 4px solid #005a9e; padding: 8px 12px; margin: 10px 0; border-radius: 0 4px 4px 0; }"
    "</style>"
    "<h2>COMPRESSION MANAGER (Full Edition) — User Manual</h2>"
    "<p>Welcome to <b>Compression Manager (Full Edition)</b>, your comprehensive desktop utility for archiving, extracting, and verifying files and directories across multiple formats.</p>"
    
    "<h3>1. How to Archive Files and Folders</h3>"
    "<p>Archiving is straightforward and flexible:</p>"
    "<ol>"
    "    <li><b>Adding Items:</b> Click <i>'Add Files'</i> or <i>'Add Folders'</i> to select content. You can also drag and drop files and folders directly from Windows Explorer into the input list.</li>"
    "    <li><b>Selecting Format:</b> In the methods quadrant or top menu, choose your preferred format: <b>RAR</b>, <b>ZIP</b>, <b>7Z</b>, <b>TAR</b>, or <b>WIM</b>.</li>"
    "    <li><b>Configuring Compression Method:</b> Under <i>Settings &gt; Select Compression Method</i>, choose the target format and set your desired compression level:"
    "        <ul>"
    "            <li><b>Store (0):</b> No compression (fastest storage).</li>"
    "            <li><b>Fastest (1) / Fast (3):</b> High speed with low CPU overhead.</li>"
    "            <li><b>Normal (5):</b> Optimal balance between speed and compression ratio.</li>"
    "            <li><b>Maximum (7) / Ultra (9):</b> Maximum space savings.</li>"
    "        </ul>"
    "    </li>"
    "    <li><b>Update Option:</b> Check <i>'Update existing archives'</i> to append or update files in an existing archive without rebuilding it from scratch.</li>"
    "    <li><b>Destination Directories:</b> Choose where the compressed archive will be stored. You can maintain multiple persistent destinations per format.</li>"
    "    <li><b>Starting Archiving:</b> Click the <i>'Store [Format]'</i> button (e.g., <i>Store RAR</i> or <i>Store ZIP</i>). The task is executed in the background queue, keeping the interface fluid and responsive.</li>"
    "</ol>"

    "<h3>2. How to Extract Archives</h3>"
    "<ol>"
    "    <li>Add the archive file (<b>.rar</b>, <b>.zip</b>, <b>.7z</b>, <b>.tar</b>, <b>.wim</b>, <b>.iso</b>, etc.) to the input list.</li>"
    "    <li>Specify your desired extraction output folder.</li>"
    "    <li>Click <i>'Extract Files'</i>. The engine extracts all files while preserving the original folder structure.</li>"
    "</ol>"

    "<h3>3. How to Check Archive Integrity</h3>"
    "<p>To verify that an archive is not corrupted before transfer or long-term storage:</p>"
    "<ul>"
    "    <li>Select the archive in the input list.</li>"
    "    <li>Click <b>Test Integrity</b>. Block-by-block checksum verification runs in the background without writing anything to disk.</li>"
    "    <li>A clear status dialog will confirm whether the archive is intact or contains errors.</li>"
    "</ul>"

    "<h3>4. Layout Modes and Responsive Adaptation</h3>"
    "<p>The application offers two flexible display modes to accommodate your workflow needs:</p>"
    "<ul>"
    "    <li><b>Standard Layout (Layout 1):</b> A streamlined single-stream view tailored for quick tasks and smaller screens.</li>"
    "    <li><b>Second Layout / Dual Layout (Layout 2):</b> To enable, open the top menu under <i>Settings &gt; Layouts &gt; Second Layout</i> (or click the layout toggle button). The workspace divides into parallel quadrant sets, allowing you to configure and monitor two distinct compression formats simultaneously (for example, archiving to RAR and ZIP side by side) without switching tabs.</li>"
    "    <li><b>Automatic Orientation Adaptation:</b> In Dual Layout, the interface continuously detects window size and screen aspect ratio. When resizing the window or maximizing it, panels automatically reorient horizontally (side-by-side on wide screens) or vertically (stacked on narrower displays), ensuring visual comfort and smooth scrolling.</li>"
    "    <li><b>Seamless Maximized Window Switching:</b> You can activate and deactivate the Second Layout at any time while the window is maximized. The application preserves its maximized state, all queued files, selected preferences, and target directories without interruption.</li>"
    "</ul>"

    "<h3>5. Languages and User Preferences</h3>"
    "<ul>"
    "    <li><b>Instant Language Switching:</b> Open <i>Settings &gt; Languages</i> to switch between <b>Brazilian Portuguese</b> and <b>English (US)</b> on the fly without restarting.</li>"
    "    <li><b>Secure Local Preferences:</b> All settings, active methods, and output paths are saved locally in your user profile. The app works 100% offline with zero cloud dependency.</li>"
    "    <li><b>Engine Detection:</b> Automatically detects <b>7-Zip</b> and <b>WinRAR</b> engines located in default system paths or bundled alongside the application.</li>"
    "</ul>";

#else

const QString MANUAL_TEXT_PT_BR =
    "<style>"
    "body { font-family: 'Segoe UI', Tahoma, Arial, sans-serif; line-height: 1.5; color: #222; margin: 12px; }"
    "h2 { color: #005a9e; border-bottom: 2px solid #005a9e; padding-bottom: 4px; margin-top: 0; }"
    "h3 { color: #106ebe; margin-top: 18px; margin-bottom: 6px; }"
    "h4 { color: #2b579a; margin-top: 12px; margin-bottom: 4px; }"
    "ul, ol { margin-top: 4px; margin-bottom: 8px; padding-left: 22px; }"
    "li { margin-bottom: 4px; }"
    "b { color: #003a6c; }"
    ".note { background: #f3f2f1; border-left: 4px solid #005a9e; padding: 8px 12px; margin: 10px 0; border-radius: 0 4px 4px 0; }"
    "</style>"
    "<h2>COMPRESSION MANAGER (Edição Standart) — Manual do Usuário</h2>"
    "<p>Bem-vindo ao <b>Compression Manager (Edição Standart)</b>, seu utilitário leve e robusto para compactação, extração e verificação de integridade de arquivos e diretórios em formatos abertos consolidados.</p>"
    
    "<h3>1. Como Empacotar Arquivos e Pastas</h3>"
    "<p>O processo de empacotamento é simples e direto:</p>"
    "<ol>"
    "    <li><b>Adição de Itens:</b> Clique nos botões <i>'Adicionar Arquivos'</i> ou <i>'Adicionar Pastas'</i> para selecionar o conteúdo desejado. Você também pode arrastar e soltar arquivos e pastas diretamente do Windows Explorer para dentro da lista de entrada.</li>"
    "    <li><b>Seleção do Formato:</b> No painel de métodos ou no menu superior, escolha o formato desejado: <b>ZIP</b>, <b>7Z</b>, <b>TAR</b> ou <b>WIM</b>.</li>"
    "    <li><b>Configuração do Método de Compressão:</b> Em <i>Configurações &gt; Selecionar Método de Compressão</i>, selecione o formato e defina o nível desejado:"
    "        <ul>"
    "            <li><b>Store (0):</b> Sem compressão (armazenamento instantâneo).</li>"
    "            <li><b>Fastest (1) / Fast (3):</b> Compactação rápida com baixo consumo de processamento.</li>"
    "            <li><b>Normal (5):</b> Equilíbrio ideal entre tempo e taxa de redução de tamanho.</li>"
    "            <li><b>Maximum (7) / Ultra (9):</b> Máxima redução de tamanho para economia de espaço.</li>"
    "        </ul>"
    "    </li>"
    "    <li><b>Opção de Atualização:</b> Marque a opção <i>'Atualizar arquivos existentes'</i> se desejar acrescentar ou atualizar itens em um pacote já existente sem precisar recriá-lo do zero.</li>"
    "    <li><b>Diretórios de Destino:</b> Escolha o diretório onde o arquivo compactado será salvo. É possível cadastrar e alternar entre múltiplos destinos independentes.</li>"
    "    <li><b>Início da Compressão:</b> Clique no botão <i>'Armazenar [Formato]'</i> (por exemplo, <i>Armazenar ZIP</i> ou <i>Armazenar 7Z</i>). A tarefa será processada em segundo plano na fila assíncrona, mantendo a interface ágil e responsiva.</li>"
    "</ol>"

    "<h3>2. Como Extrair Arquivos Compactados</h3>"
    "<ol>"
    "    <li>Adicione o arquivo compactado desejado (<b>.zip</b>, <b>.7z</b>, <b>.tar</b>, <b>.wim</b>, <b>.iso</b>, etc.) à lista de entrada.</li>"
    "    <li>No quadrante ou aba de Extração, defina a pasta de destino onde deseja salvar os itens extraídos.</li>"
    "    <li>Clique no botão <i>'Extrair Arquivos'</i>. O aplicativo processará a extração preservando a estrutura original de pastas e arquivos.</li>"
    "</ol>"

    "<h3>3. Como Verificar a Integridade de Pacotes</h3>"
    "<p>Para certificar-se de que um arquivo compactado não foi corrompido durante transferências ou armazenamento:</p>"
    "<ul>"
    "    <li>Selecione o arquivo compactado na lista de entrada.</li>"
    "    <li>Acione o botão de <b>Verificação de Integridade</b>. O teste de integridade é realizado bloco a bloco em segundo plano, validando as somas de verificação sem descompactar nada no disco.</li>"
    "    <li>Ao final, uma mensagem informativa confirma se o pacote está íntegro ou relata eventuais falhas.</li>"
    "</ul>"

    "<h3>4. Modos de Layout e Adaptação Responsiva</h3>"
    "<p>O aplicativo oferece dois modos de visualização para acomodar suas necessidades de trabalho:</p>"
    "<ul>"
    "    <li><b>Layout Padrão (Layout 1):</b> Disposição clássica focada em fluxo individual, ideal para operações rápidas e telas compactas.</li>"
    "    <li><b>Segundo Layout / Layout Duplo (Layout 2):</b> Para ativar, acesse o menu superior em <i>Configurações &gt; Layouts &gt; Segundo Layout</i> (ou utilize o botão de alternância de layout). A tela divide-se em conjuntos paralelos de quadrantes, permitindo configurar e acompanhar dois formatos de compressão distintos simultaneamente (por exemplo, empacotar em ZIP e em 7Z lado a lado) sem sobreposição de abas.</li>"
    "    <li><b>Adaptação Automática de Orientação:</b> No Segundo Layout, a interface monitora o espaço disponível na janela. Ao redimensionar a janela ou utilizá-la maximizada, os painéis reorganizam-se de forma dinâmica na horizontal (lado a lado em telas largas) ou na vertical (empilhados em janelas mais estreitas), proporcionando rolagem suave e ergonomia visual.</li>"
    "    <li><b>Alternância com Janela Maximizada:</b> Você pode ativar e desativar o Segundo Layout livremente a qualquer momento, inclusive com a janela maximizada em tela cheia. O aplicativo mantém o estado maximizado e preserva integralmente todas as suas seleções de arquivos, listas de pastas e configurações de compressão.</li>"
    "</ul>"

    "<h3>5. Idioma e Preferências do Usuário</h3>"
    "<ul>"
    "    <li><b>Troca Instantânea de Idioma:</b> Acesse <i>Configurações &gt; Idiomas</i> no menu superior e alterne entre <b>Português (Brasil)</b> e <b>Inglês (Estados Unidos)</b> a qualquer momento, sem precisar reiniciar o aplicativo.</li>"
    "    <li><b>Persistência Segura e Modo Offline:</b> Todas as suas configurações, últimos formatos utilizados e listas de destinos são salvos de forma segura no seu perfil local do usuário. O aplicativo funciona de modo 100% offline, garantindo total privacidade aos seus dados.</li>"
    "    <li><b>Detecção do Motor:</b> O sistema busca automaticamente o motor <b>7-Zip</b> nas pastas padrão do Windows e na pasta local do aplicativo.</li>"
    "</ul>";

const QString MANUAL_TEXT_EN_US =
    "<style>"
    "body { font-family: 'Segoe UI', Tahoma, Arial, sans-serif; line-height: 1.5; color: #222; margin: 12px; }"
    "h2 { color: #005a9e; border-bottom: 2px solid #005a9e; padding-bottom: 4px; margin-top: 0; }"
    "h3 { color: #106ebe; margin-top: 18px; margin-bottom: 6px; }"
    "h4 { color: #2b579a; margin-top: 12px; margin-bottom: 4px; }"
    "ul, ol { margin-top: 4px; margin-bottom: 8px; padding-left: 22px; }"
    "li { margin-bottom: 4px; }"
    "b { color: #003a6c; }"
    ".note { background: #f3f2f1; border-left: 4px solid #005a9e; padding: 8px 12px; margin: 10px 0; border-radius: 0 4px 4px 0; }"
    "</style>"
    "<h2>COMPRESSION MANAGER (Standart Edition) — User Manual</h2>"
    "<p>Welcome to <b>Compression Manager (Standart Edition)</b>, your lightweight and robust desktop utility for archiving, extracting, and verifying files and directories using standard open formats.</p>"
    
    "<h3>1. How to Archive Files and Folders</h3>"
    "<p>Archiving is straightforward and flexible:</p>"
    "<ol>"
    "    <li><b>Adding Items:</b> Click <i>'Add Files'</i> or <i>'Add Folders'</i> to select content. You can also drag and drop files and folders directly from Windows Explorer into the input list.</li>"
    "    <li><b>Selecting Format:</b> In the methods quadrant or top menu, choose your desired format: <b>ZIP</b>, <b>7Z</b>, <b>TAR</b>, or <b>WIM</b>.</li>"
    "    <li><b>Configuring Compression Method:</b> Under <i>Settings &gt; Select Compression Method</i>, choose the target format and set your desired compression level:"
    "        <ul>"
    "            <li><b>Store (0):</b> No compression (fastest storage).</li>"
    "            <li><b>Fastest (1) / Fast (3):</b> High speed with low CPU overhead.</li>"
    "            <li><b>Normal (5):</b> Optimal balance between speed and compression ratio.</li>"
    "            <li><b>Maximum (7) / Ultra (9):</b> Maximum space savings.</li>"
    "        </ul>"
    "    </li>"
    "    <li><b>Update Option:</b> Check <i>'Update existing archives'</i> to append or update files in an existing archive without rebuilding it from scratch.</li>"
    "    <li><b>Destination Directories:</b> Choose where the compressed archive will be stored. You can maintain multiple persistent destinations per format.</li>"
    "    <li><b>Starting Archiving:</b> Click the <i>'Store [Format]'</i> button (e.g., <i>Store ZIP</i> or <i>Store 7Z</i>). The task is executed in the background queue, keeping the interface fluid and responsive.</li>"
    "</ol>"

    "<h3>2. How to Extract Archives</h3>"
    "<ol>"
    "    <li>Add the archive file (<b>.zip</b>, <b>.7z</b>, <b>.tar</b>, <b>.wim</b>, <b>.iso</b>, etc.) to the input list.</li>"
    "    <li>Specify your desired extraction output folder.</li>"
    "    <li>Click <i>'Extract Files'</i>. The engine extracts all files while preserving the original folder structure.</li>"
    "</ol>"

    "<h3>3. How to Check Archive Integrity</h3>"
    "<p>To verify that an archive is not corrupted before transfer or long-term storage:</p>"
    "<ul>"
    "    <li>Select the archive in the input list.</li>"
    "    <li>Click <b>Test Integrity</b>. Block-by-block checksum verification runs in the background without writing anything to disk.</li>"
    "    <li>A clear status dialog will confirm whether the archive is intact or contains errors.</li>"
    "</ul>"

    "<h3>4. Layout Modes and Responsive Adaptation</h3>"
    "<p>The application offers two flexible display modes to accommodate your workflow needs:</p>"
    "<ul>"
    "    <li><b>Standard Layout (Layout 1):</b> A streamlined single-stream view tailored for quick tasks and smaller screens.</li>"
    "    <li><b>Second Layout / Dual Layout (Layout 2):</b> To enable, open the top menu under <i>Settings &gt; Layouts &gt; Second Layout</i> (or click the layout toggle button). The workspace divides into parallel quadrant sets, allowing you to configure and monitor two distinct compression formats simultaneously (for example, archiving to ZIP and 7Z side by side) without switching tabs.</li>"
    "    <li><b>Automatic Orientation Adaptation:</b> In Dual Layout, the interface continuously detects window size and screen aspect ratio. When resizing the window or maximizing it, panels automatically reorient horizontally (side-by-side on wide screens) or vertically (stacked on narrower displays), ensuring visual comfort and smooth scrolling.</li>"
    "    <li><b>Seamless Maximized Window Switching:</b> You can activate and deactivate the Second Layout at any time while the window is maximized. The application preserves its maximized state, all queued files, selected preferences, and target directories without interruption.</li>"
    "</ul>"

    "<h3>5. Languages and User Preferences</h3>"
    "<ul>"
    "    <li><b>Instant Language Switching:</b> Open <i>Settings &gt; Languages</i> to switch between <b>Brazilian Portuguese</b> and <b>English (US)</b> on the fly without restarting.</li>"
    "    <li><b>Secure Local Preferences:</b> All settings, active methods, and output paths are saved locally in your user profile. The app works 100% offline with zero cloud dependency.</li>"
    "    <li><b>Engine Detection:</b> Automatically detects the <b>7-Zip</b> engine located in default system paths or bundled alongside the application.</li>"
    "</ul>";

#endif

ManualDialog::ManualDialog(QWidget* parent, const QString& idioma)
    : QDialog(parent)
{
    try {
        setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowSystemMenuHint |
                       Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
        setModal(false);

        auto layout = new QVBoxLayout(this);

        browser = new QTextBrowser(this);
        browser->setReadOnly(true);
        browser->setOpenExternalLinks(true);
        browser->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addWidget(browser);

        auto buttonLayout = new QHBoxLayout();
        ok_button = new QPushButton("OK", this);
        ok_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        connect(ok_button, &QPushButton::clicked, this, &ManualDialog::accept);

        buttonLayout->addStretch(1);
        buttonLayout->addWidget(ok_button);
        layout->addLayout(buttonLayout);

        atualizar_traducao(idioma);

        setMinimumSize(600, 400);
        resize(700, 500);
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao criar ManualDialog: %1").arg(e.what()));
    }
}

void ManualDialog::atualizar_traducao(const QString& idioma) {
#if ENABLE_RAR_SUPPORT
    QString titulo = (idioma == "pt_BR")
        ? QCoreApplication::translate("InterfaceGrafica", "Manual do Usuário (Edição Full) - COMPRESSION MANAGER")
        : QCoreApplication::translate("InterfaceGrafica", "User Manual (Full Edition) - COMPRESSION MANAGER");
#else
    QString titulo = (idioma == "pt_BR")
        ? QCoreApplication::translate("InterfaceGrafica", "Manual do Usuário (Edição Standart) - COMPRESSION MANAGER")
        : QCoreApplication::translate("InterfaceGrafica", "User Manual (Standart Edition) - COMPRESSION MANAGER");
#endif
    setWindowTitle(titulo);

    if (browser) {
        if (idioma == "en_US") {
            browser->setHtml(MANUAL_TEXT_EN_US);
        } else {
            browser->setHtml(MANUAL_TEXT_PT_BR);
        }
    }

    if (ok_button) {
        ok_button->setText(QCoreApplication::translate("Dialog", "OK"));
    }
}

void ManualDialog::changeEvent(QEvent* event) {
    if (event->type() == QEvent::ActivationChange) {
        if (isActiveWindow()) {
            raise();
        }
    }
    QDialog::changeEvent(event);
}

void exibir_manual(QWidget* app) {
    try {
        auto ig = qobject_cast<InterfaceGrafica*>(app);
        if (ig && ig->dialog_manual) {
            ig->dialog_manual->showNormal();
            ig->dialog_manual->raise();
            ig->dialog_manual->activateWindow();
            return;
        }

        QString idioma = "pt_BR";
        if (ig && ig->gerenciador_traducao) {
            idioma = ig->gerenciador_traducao->obter_idioma_atual();
        } else if (app) {
            auto prop = app->property("idioma_atual");
            if (prop.isValid()) {
                idioma = prop.toString();
            }
        }

        auto dialog = new ManualDialog(nullptr, idioma);
        if (ig) {
            ig->dialog_manual = dialog;
        }
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->showNormal();
        dialog->raise();
        dialog->activateWindow();
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao exibir manual: %1").arg(e.what()));
    }
}
