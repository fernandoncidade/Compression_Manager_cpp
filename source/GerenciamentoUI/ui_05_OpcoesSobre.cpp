#include "ui_05_OpcoesSobre.hpp"
#include "ui_08_Release_en_US.hpp"
#include "ui_09_Lancamento_pt_BR.hpp"
#include "source/utils/ApplicationPathUtils.hpp"
#include "source/utils/LogManager.hpp"

namespace OpcoesSobre {

const QString SITE_LICENSES =
    "https://www.gnu.org/licenses/lgpl-3.0.html.en\n"
    "https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.en\n"
    "https://opensource.org/licenses/BSD-2-Clause\n"
    "https://opensource.org/licenses/BSD-3-Clause\n"
    "https://www.apache.org/licenses/LICENSE-2.0\n"
    "https://python-pillow.org/\n"
    "https://opensource.org/licenses/MIT\n"
    "https://opensource.org/license/isc-license-txt\n";

QString LICENSE_OWNER;
QString LICENSE_TEXT_PT_BR;
QString LICENSE_TEXT_EN_US;
QString NOTICE_TEXT_PT_BR;
QString NOTICE_TEXT_EN_US;
QString ABOUT_TEXT_PT_BR;
QString ABOUT_TEXT_EN_US;
QString Privacy_Policy_pt_BR;
QString Privacy_Policy_en_US;
QString History_APP_pt_BR;
QString History_APP_en_US;
QString RELEASE_NOTES_pt_BR;
QString RELEASE_NOTES_en_US;

static bool s_loaded = false;

void carregar_textos_sobre() {
    try {
        // Notas de lançamento são recarregadas dinamicamente em disco a cada abertura/atualização
        RELEASE_NOTES_pt_BR = LancamentoPtBR::obter_notas_lancamento_pt_BR();
        RELEASE_NOTES_en_US = ReleaseNotesEnUS::obter_release_notes_en_US();

        if (s_loaded) return;

        LICENSE_OWNER = load_text_file("License.txt", "LICENSES");

        LICENSE_TEXT_PT_BR = load_text_file("EULA_pt_BR - Compression Manager.txt", "EULA");
        LICENSE_TEXT_EN_US = load_text_file("EULA_en_US - Compression Manager.txt", "EULA");

        NOTICE_TEXT_PT_BR = load_text_file("NOTICE_pt_BR.txt", "NOTICES");
        NOTICE_TEXT_EN_US = load_text_file("NOTICE_en_US.txt", "NOTICES");

        ABOUT_TEXT_PT_BR = load_text_file("ABOUT_pt_BR.txt", "ABOUT");
        ABOUT_TEXT_EN_US = load_text_file("ABOUT_en_US.txt", "ABOUT");

        Privacy_Policy_pt_BR = load_text_file("Privacy_Policy_pt_BR.txt", "PRIVACY_POLICY");
        Privacy_Policy_en_US = load_text_file("Privacy_Policy_en_US.txt", "PRIVACY_POLICY");

        History_APP_pt_BR = load_text_file("History_APP_pt_BR.txt", "ABOUT");
        History_APP_en_US = load_text_file("History_APP_en_US.txt", "ABOUT");

        s_loaded = true;
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao carregar textos de opções sobre: %1").arg(e.what()));
    }
}

} // namespace OpcoesSobre
