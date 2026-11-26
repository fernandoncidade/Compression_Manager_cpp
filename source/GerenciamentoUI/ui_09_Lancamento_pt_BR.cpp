#include "ui_09_Lancamento_pt_BR.hpp"
#include "source/utils/ApplicationPathUtils.hpp"
#include "source/utils/LogManager.hpp"

namespace LancamentoPtBR {

namespace {

#if ENABLE_RAR_SUPPORT
constexpr const char* kReleaseNotesFileNamePtBR = "RELEASE NOTES_pt_BR.txt";
#else
constexpr const char* kReleaseNotesFileNamePtBR = "RELEASE NOTES_Standart_pt_BR.txt";
#endif

constexpr const char* kReleaseNotesFolder = "RELEASE";

} // namespace

QString obter_notas_lancamento_pt_BR() {
    const QString fileName = QString::fromUtf8(kReleaseNotesFileNamePtBR);
    const QString folder = QString::fromUtf8(kReleaseNotesFolder);
    const QString content = load_text_file(fileName, folder);
    if (content.trimmed().isEmpty()) {
        LogManager::warning(
            QString("Notas de lançamento (pt_BR) indisponíveis ou vazias em '%1/%2'.")
                .arg(folder, fileName));
    }
    return content;
}

} // namespace LancamentoPtBR
