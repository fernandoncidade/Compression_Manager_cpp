#include "ui_08_Release_en_US.hpp"
#include "source/utils/ApplicationPathUtils.hpp"
#include "source/utils/LogManager.hpp"

namespace ReleaseNotesEnUS {

namespace {

#if ENABLE_RAR_SUPPORT
constexpr const char* kReleaseNotesFileNameEnUS = "RELEASE NOTES_en_US.txt";
#else
constexpr const char* kReleaseNotesFileNameEnUS = "RELEASE NOTES_Standart_en_US.txt";
#endif

constexpr const char* kReleaseNotesFolder = "RELEASE";

} // namespace

QString obter_release_notes_en_US() {
    const QString fileName = QString::fromUtf8(kReleaseNotesFileNameEnUS);
    const QString folder = QString::fromUtf8(kReleaseNotesFolder);
    const QString content = load_text_file(fileName, folder);
    if (content.trimmed().isEmpty()) {
        LogManager::warning(
            QString("Release notes (en_US) unavailable or empty at '%1/%2'.")
                .arg(folder, fileName));
    }
    return content;
}

} // namespace ReleaseNotesEnUS
