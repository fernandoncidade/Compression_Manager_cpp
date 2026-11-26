cmake_minimum_required(VERSION 3.28)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(Compression_Manager_Mocks LANGUAGES CXX)
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)
    find_package(Qt6 6.12 REQUIRED COMPONENTS Core Gui Widgets PrintSupport)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
foreach(_cfg IN ITEMS DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${_cfg} "${CMAKE_BINARY_DIR}")
endforeach()

if(WIN32)
    enable_language(RC)
endif()

find_package(Qt6 6.12 QUIET COMPONENTS LinguistTools)

function(mocks_resolve_qt6_target_location TARGET_NAME OUT_VAR)
    set(_resolved "")
    if(TARGET "${TARGET_NAME}")
        foreach(_prop IN ITEMS
                IMPORTED_LOCATION_RELEASE
                IMPORTED_LOCATION_RELWITHDEBINFO
                IMPORTED_LOCATION
                IMPORTED_LOCATION_DEBUG)
            get_target_property(_val "${TARGET_NAME}" "${_prop}")
            if(_val AND NOT _val STREQUAL "${_prop}-NOTFOUND")
                set(_resolved "${_val}")
                break()
            endif()
        endforeach()
    endif()
    set("${OUT_VAR}" "${_resolved}" PARENT_SCOPE)
endfunction()

mocks_resolve_qt6_target_location(Qt6::Core MOCKS_QT_CORE_IMPORTED_LOCATION)
mocks_resolve_qt6_target_location(Qt6::qmake MOCKS_QT_QMAKE_IMPORTED_LOCATION)
mocks_resolve_qt6_target_location(Qt6::lrelease MOCKS_LRELEASE_EXECUTABLE)

if(MOCKS_QT_CORE_IMPORTED_LOCATION)
    get_filename_component(MOCKS_QT_BIN_DIR "${MOCKS_QT_CORE_IMPORTED_LOCATION}" DIRECTORY)
    get_filename_component(MOCKS_QT_PREFIX_DIR "${MOCKS_QT_BIN_DIR}" DIRECTORY)
elseif(MOCKS_QT_QMAKE_IMPORTED_LOCATION)
    get_filename_component(MOCKS_QT_BIN_DIR "${MOCKS_QT_QMAKE_IMPORTED_LOCATION}" DIRECTORY)
    get_filename_component(MOCKS_QT_PREFIX_DIR "${MOCKS_QT_BIN_DIR}" DIRECTORY)
else()
    get_filename_component(MOCKS_QT_PREFIX_DIR "${Qt6_DIR}/../../.." ABSOLUTE)
    set(MOCKS_QT_BIN_DIR "${MOCKS_QT_PREFIX_DIR}/bin")
endif()

set(MOCKS_WINDEPLOYQT_EXECUTABLE "${MOCKS_QT_BIN_DIR}/windeployqt.exe")
set(MOCKS_QTPATHS_EXECUTABLE "${MOCKS_QT_BIN_DIR}/qtpaths6.exe")
if(NOT EXISTS "${MOCKS_QTPATHS_EXECUTABLE}" AND EXISTS "${MOCKS_QT_BIN_DIR}/qtpaths.exe")
    set(MOCKS_QTPATHS_EXECUTABLE "${MOCKS_QT_BIN_DIR}/qtpaths.exe")
endif()

set(MOCKS_CMAKE_SCRIPTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

# Por padrão, os artefatos de mocks residem exclusivamente em build/ e em D:/MISCELANEAS/VSCode/Compression_Manager (distribuicao externa limpa).
# Nao gerar distribuicoes internas dentro de mocks/ para nao poluir o diretorio de codigo-fonte.
set(MOCKS_DEFAULT_EXPORT_DIR "")

set(
    MOCKS_EXPORT_DIR
    "${MOCKS_DEFAULT_EXPORT_DIR}"
    CACHE PATH
    "Additional directory that receives the deployed mocks runtime"
    FORCE
)

if(MOCKS_EXPORT_DIR)
    file(TO_CMAKE_PATH "${MOCKS_EXPORT_DIR}" MOCKS_EXPORT_DIR_NORMALIZED)
else()
    set(MOCKS_EXPORT_DIR_NORMALIZED "")
endif()

set(MOCKS_WINDOWS_ICON_NAME "Manager-BackUp.ico")
set(MOCKS_WINDOWS_ICON "${CMAKE_CURRENT_SOURCE_DIR}/icones/${MOCKS_WINDOWS_ICON_NAME}")
if(NOT EXISTS "${MOCKS_WINDOWS_ICON}")
    set(MOCKS_WINDOWS_ICON "${CMAKE_CURRENT_SOURCE_DIR}/../source/assets/icones/${MOCKS_WINDOWS_ICON_NAME}")
endif()

set(MOCKS_RC_FILE)
if(WIN32)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/icones/mocks.rc")
        set(MOCKS_RC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/icones/mocks.rc")
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/icones/Compression_Manager.rc")
        set(MOCKS_RC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/icones/Compression_Manager.rc")
    endif()
endif()

function(mocks_apply_common_options TARGET_NAME)
    set_target_properties(
        "${TARGET_NAME}"
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
            RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}"
            RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}"
            RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}"
    )

    target_include_directories(
        "${TARGET_NAME}"
        PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}"
            "${CMAKE_CURRENT_SOURCE_DIR}/icones"
            "${CMAKE_CURRENT_SOURCE_DIR}/.."
            "${CMAKE_CURRENT_SOURCE_DIR}/../source"
    )

    target_compile_definitions(
        "${TARGET_NAME}"
        PRIVATE
            "MOCKS_WINDOWS_ICON_NAME=\"${MOCKS_WINDOWS_ICON_NAME}\""
            $<$<CONFIG:Release>:QT_NO_DEBUG_OUTPUT>
            $<$<CONFIG:Release>:QT_NO_INFO_OUTPUT>
    )

    if(MSVC)
        set_property(TARGET "${TARGET_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
        target_compile_options(
            "${TARGET_NAME}"
            PRIVATE
                /MP
                $<$<CONFIG:Release>:/O2>
                $<$<CONFIG:Release>:/Oi>
                $<$<CONFIG:Release>:/Ot>
        )
    elseif(MINGW)
        target_compile_options(
            "${TARGET_NAME}"
            PRIVATE
                $<$<CONFIG:Release>:-O2>
        )
        target_link_options(
            "${TARGET_NAME}"
            PRIVATE
                $<$<CONFIG:Release>:-s>
        )
    endif()
endfunction()

# Executáveis
add_executable(
    CompileTranslationsGUI
    WIN32
    CompileTranslationsGUI.cpp
    CompileTranslationsGUI.hpp
    ${MOCKS_RC_FILE}
)
target_link_libraries(CompileTranslationsGUI PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
mocks_apply_common_options(CompileTranslationsGUI)

add_executable(
    CompileTranslationsTerminal
    CompileTranslationsTerminal.cpp
    CompileTranslationsTerminal.hpp
    ${MOCKS_RC_FILE}
)
target_link_libraries(CompileTranslationsTerminal PRIVATE Qt6::Core)
if(WIN32)
    target_link_libraries(CompileTranslationsTerminal PRIVATE User32)
endif()
mocks_apply_common_options(CompileTranslationsTerminal)

add_executable(
    find_VersionEditor
    WIN32
    find_VersionEditor.cpp
    find_VersionEditor.hpp
    ${MOCKS_RC_FILE}
)
target_link_libraries(find_VersionEditor PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
mocks_apply_common_options(find_VersionEditor)

# Gerar qt.conf para garantir isolamento e carregamento local de plugins nos mocks
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    file(WRITE "${CMAKE_BINARY_DIR}/qt.conf"
        "[Paths]\nPrefix = .\nPlugins = plugins\nTranslations = translations\n"
    )
endif()

# Target para lrelease
if(TARGET Qt6::lrelease)
    if(MOCKS_LRELEASE_EXECUTABLE AND EXISTS "${MOCKS_LRELEASE_EXECUTABLE}")
        set(_lrelease_cmd "${MOCKS_LRELEASE_EXECUTABLE}")
    else()
        set(_lrelease_cmd Qt6::lrelease)
    endif()
    file(GLOB TS_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../source/language/translations/*.ts")
    set(QM_FILES)
    foreach(TS_FILE IN LISTS TS_FILES)
        get_filename_component(TS_NAME "${TS_FILE}" NAME_WE)
        set(QM_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../source/language/translations/${TS_NAME}.qm")
        add_custom_command(
            OUTPUT "${QM_FILE}"
            COMMAND "${_lrelease_cmd}" "${TS_FILE}" -qm "${QM_FILE}"
            DEPENDS "${TS_FILE}"
            COMMENT "Compilando ${TS_FILE} para ${QM_FILE}"
            VERBATIM
        )
        list(APPEND QM_FILES "${QM_FILE}")
    endforeach()
    if(QM_FILES)
        add_custom_target(CompileTranslationsQm ALL DEPENDS ${QM_FILES})
    endif()
endif()

set(MINGW_BIN_DIR "")
if(MINGW)
    get_filename_component(MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
endif()

set(MOCKS_MSVC_REDIST_ROOT_DETECTED "")
if(MSVC)
    if(DEFINED ENV{VCToolsRedistDir} AND NOT "$ENV{VCToolsRedistDir}" STREQUAL "")
        file(TO_CMAKE_PATH "$ENV{VCToolsRedistDir}" MOCKS_MSVC_REDIST_ROOT_DETECTED)
    else()
        foreach(_redist_candidate IN ITEMS
            "C:/Program Files (x86)/Microsoft Visual Studio/18/BuildTools/VC/Redist/MSVC/14.52.37025"
            "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Redist/MSVC/14.52.37025"
            "C:/Program Files (x86)/Microsoft Visual Studio/18/BuildTools/VC/Redist/MSVC"
            "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Redist/MSVC"
        )
            if(EXISTS "${_redist_candidate}")
                set(MOCKS_MSVC_REDIST_ROOT_DETECTED "${_redist_candidate}")
                break()
            endif()
        endforeach()
    endif()
endif()

# Comandos de deploy sequenciais e enxutos no target MocksDeploy
set(DEPLOY_COMMANDS)

# 1. Copiar somente os 2 icones efetivamente usados pelos mocks (Manager-BackUp.ico e Manager-BackUp.png)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/icones")
    list(APPEND DEPLOY_COMMANDS
        COMMAND "${CMAKE_COMMAND}"
            "-DSOURCE_ICONS_DIR=${CMAKE_CURRENT_SOURCE_DIR}/icones"
            "-DDEST_ICONS_DIR=${CMAKE_BINARY_DIR}/icones"
            -P "${MOCKS_CMAKE_SCRIPTS_DIR}/copy_lean_mock_icons.cmake"
    )
endif()

# 2. Executar windeployqt de forma serializada e enxuta para cada executavel GUI no mesmo diretorio
if(WIN32 AND EXISTS "${MOCKS_WINDEPLOYQT_EXECUTABLE}")
    foreach(_mock_target IN ITEMS find_VersionEditor CompileTranslationsGUI)
        set(_mock_deploy_args
            "-DWINDEPLOYQT_EXECUTABLE=${MOCKS_WINDEPLOYQT_EXECUTABLE}"
            "-DQTPATHS_EXECUTABLE=${MOCKS_QTPATHS_EXECUTABLE}"
            "-DTARGET_FILE=$<TARGET_FILE:${_mock_target}>"
            "-DRUNTIME_DIR=${CMAKE_BINARY_DIR}"
            "-DPLUGIN_DIR=${CMAKE_BINARY_DIR}/plugins"
            "-DTRANSLATION_DIR=${CMAKE_BINARY_DIR}/translations"
            "-DQT_PLUGIN_TYPES=styles,imageformats,platforms"
            "-DQT_SKIP_PLUGIN_TYPES=generic,iconengines,networkinformation,platforminputcontexts,tls"
            "-DQT_EXCLUDE_PLUGINS=qpdf,qsvg,qgif,qicns,qjpeg,qtga,qtiff,qwbmp,qwebp,qdirect2d,qminimal,qoffscreen"
            "-DNO_COMPILER_RUNTIME=ON"
        )
        if(MINGW AND MINGW_BIN_DIR)
            list(APPEND _mock_deploy_args "-DMINGW_BIN_DIR=${MINGW_BIN_DIR}")
        endif()
        list(APPEND DEPLOY_COMMANDS
            COMMAND "${CMAKE_COMMAND}" ${_mock_deploy_args} -P "${MOCKS_CMAKE_SCRIPTS_DIR}/deploy_optional_qt_binary.cmake"
        )
    endforeach()
endif()

list(APPEND DEPLOY_COMMANDS
    COMMAND "${CMAKE_COMMAND}" "-DDEPLOY_ROOT=${CMAKE_BINARY_DIR}" -P "${MOCKS_CMAKE_SCRIPTS_DIR}/remove_compiler_runtime_installer.cmake"
)

if(MSVC)
    list(APPEND DEPLOY_COMMANDS
        COMMAND "${CMAKE_COMMAND}" "-DDEPLOY_ROOT=${CMAKE_BINARY_DIR}" -DPRUNE_UNUSED_QT_MODULES=ON -DPRUNE_MINGW_RUNTIME=ON -P "${MOCKS_CMAKE_SCRIPTS_DIR}/prune_optional_root_runtime_dlls.cmake"
        COMMAND "${CMAKE_COMMAND}" "-DDEPLOY_ROOT=${CMAKE_BINARY_DIR}" -DMSVC_RUNTIME_ARCH=x64 "-DMSVC_REDIST_ROOT=${MOCKS_MSVC_REDIST_ROOT_DETECTED}" "-DCONFIG=$<CONFIG>" -P "${MOCKS_CMAKE_SCRIPTS_DIR}/copy_msvc_runtime.cmake"
    )
elseif(MINGW)
    list(APPEND DEPLOY_COMMANDS
        COMMAND "${CMAKE_COMMAND}" "-DDEPLOY_ROOT=${CMAKE_BINARY_DIR}" -DPRUNE_UNUSED_QT_MODULES=ON -DPRUNE_MSVC_RUNTIME=ON -P "${MOCKS_CMAKE_SCRIPTS_DIR}/prune_optional_root_runtime_dlls.cmake"
    )
endif()

# 3. Copia explicita de seguranca somente das 3 DLLs Qt 6.12 usadas (Core, Gui, Widgets)
set(CRITICAL_DLLS Qt6Core.dll Qt6Gui.dll Qt6Widgets.dll)
foreach(DLL_NAME IN LISTS CRITICAL_DLLS)
    if(EXISTS "${MOCKS_QT_BIN_DIR}/${DLL_NAME}")
        list(APPEND DEPLOY_COMMANDS
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "${MOCKS_QT_BIN_DIR}/${DLL_NAME}"
                "${CMAKE_BINARY_DIR}/${DLL_NAME}"
        )
    endif()
endforeach()

# 4. Garantir somente qwindows.dll, qmodernwindowsstyle.dll e qico.dll em plugins
if(EXISTS "${MOCKS_QT_PREFIX_DIR}/plugins/platforms/qwindows.dll")
    list(APPEND DEPLOY_COMMANDS
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/plugins/platforms"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${MOCKS_QT_PREFIX_DIR}/plugins/platforms/qwindows.dll"
            "${CMAKE_BINARY_DIR}/plugins/platforms/qwindows.dll"
    )
endif()

if(EXISTS "${MOCKS_QT_PREFIX_DIR}/plugins/styles/qmodernwindowsstyle.dll")
    list(APPEND DEPLOY_COMMANDS
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/plugins/styles"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${MOCKS_QT_PREFIX_DIR}/plugins/styles/qmodernwindowsstyle.dll"
            "${CMAKE_BINARY_DIR}/plugins/styles/qmodernwindowsstyle.dll"
    )
endif()

if(EXISTS "${MOCKS_QT_PREFIX_DIR}/plugins/imageformats/qico.dll")
    list(APPEND DEPLOY_COMMANDS
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/plugins/imageformats"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${MOCKS_QT_PREFIX_DIR}/plugins/imageformats/qico.dll"
            "${CMAKE_BINARY_DIR}/plugins/imageformats/qico.dll"
    )
endif()

# 5. Copiar runtimes reais do MinGW-w64 instalado (ex.: mingw1510_64) sobrescrevendo versoes antigas
if(MINGW)
    set(GCC_RUNTIMES libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll)
    foreach(GCC_DLL IN LISTS GCC_RUNTIMES)
        if(EXISTS "${MINGW_BIN_DIR}/${GCC_DLL}")
            list(APPEND DEPLOY_COMMANDS
                COMMAND "${CMAKE_COMMAND}" -E copy
                    "${MINGW_BIN_DIR}/${GCC_DLL}"
                    "${CMAKE_BINARY_DIR}/${GCC_DLL}"
            )
        elseif(EXISTS "${MOCKS_QT_BIN_DIR}/${GCC_DLL}")
            list(APPEND DEPLOY_COMMANDS
                COMMAND "${CMAKE_COMMAND}" -E copy
                    "${MOCKS_QT_BIN_DIR}/${GCC_DLL}"
                    "${CMAKE_BINARY_DIR}/${GCC_DLL}"
            )
        endif()
    endforeach()
endif()

# 6. Deploy enxuto para diretorio de distribuicao externa opcional (MOCKS_EXPORT_DIR)
if(MOCKS_EXPORT_DIR_NORMALIZED)
    list(APPEND DEPLOY_COMMANDS
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${MOCKS_EXPORT_DIR_NORMALIZED}"
        COMMAND "${CMAKE_COMMAND}"
            "-DSOURCE_ICONS_DIR=${CMAKE_CURRENT_SOURCE_DIR}/icones"
            "-DDEST_ICONS_DIR=${MOCKS_EXPORT_DIR_NORMALIZED}/icones"
            -P "${MOCKS_CMAKE_SCRIPTS_DIR}/copy_lean_mock_icons.cmake"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_BINARY_DIR}/find_VersionEditor.exe" "${MOCKS_EXPORT_DIR_NORMALIZED}/find_VersionEditor.exe"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_BINARY_DIR}/CompileTranslationsGUI.exe" "${MOCKS_EXPORT_DIR_NORMALIZED}/CompileTranslationsGUI.exe"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_BINARY_DIR}/CompileTranslationsTerminal.exe" "${MOCKS_EXPORT_DIR_NORMALIZED}/CompileTranslationsTerminal.exe"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_BINARY_DIR}/qt.conf" "${MOCKS_EXPORT_DIR_NORMALIZED}/qt.conf"
    )
    if(WIN32 AND EXISTS "${MOCKS_WINDEPLOYQT_EXECUTABLE}")
        foreach(_mock_export_exe IN ITEMS find_VersionEditor.exe CompileTranslationsGUI.exe)
            set(_mock_export_args
                "-DWINDEPLOYQT_EXECUTABLE=${MOCKS_WINDEPLOYQT_EXECUTABLE}"
                "-DQTPATHS_EXECUTABLE=${MOCKS_QTPATHS_EXECUTABLE}"
                "-DTARGET_FILE=${MOCKS_EXPORT_DIR_NORMALIZED}/${_mock_export_exe}"
                "-DRUNTIME_DIR=${MOCKS_EXPORT_DIR_NORMALIZED}"
                "-DPLUGIN_DIR=${MOCKS_EXPORT_DIR_NORMALIZED}/plugins"
                "-DTRANSLATION_DIR=${MOCKS_EXPORT_DIR_NORMALIZED}/translations"
                "-DQT_PLUGIN_TYPES=styles,imageformats,platforms"
                "-DQT_SKIP_PLUGIN_TYPES=generic,iconengines,networkinformation,platforminputcontexts,tls"
                "-DQT_EXCLUDE_PLUGINS=qpdf,qsvg,qgif,qicns,qjpeg,qtga,qtiff,qwbmp,qwebp,qdirect2d,qminimal,qoffscreen"
                "-DNO_COMPILER_RUNTIME=ON"
            )
            if(MINGW AND MINGW_BIN_DIR)
                list(APPEND _mock_export_args "-DMINGW_BIN_DIR=${MINGW_BIN_DIR}")
            endif()
            list(APPEND DEPLOY_COMMANDS
                COMMAND "${CMAKE_COMMAND}" ${_mock_export_args} -P "${MOCKS_CMAKE_SCRIPTS_DIR}/deploy_optional_qt_binary.cmake"
            )
        endforeach()
    endif()
    list(APPEND DEPLOY_COMMANDS
        COMMAND "${CMAKE_COMMAND}" "-DDEPLOY_ROOT=${MOCKS_EXPORT_DIR_NORMALIZED}" -P "${MOCKS_CMAKE_SCRIPTS_DIR}/remove_compiler_runtime_installer.cmake"
    )
    if(MSVC)
        list(APPEND DEPLOY_COMMANDS
            COMMAND "${CMAKE_COMMAND}" "-DDEPLOY_ROOT=${MOCKS_EXPORT_DIR_NORMALIZED}" -DPRUNE_UNUSED_QT_MODULES=ON -DPRUNE_MINGW_RUNTIME=ON -P "${MOCKS_CMAKE_SCRIPTS_DIR}/prune_optional_root_runtime_dlls.cmake"
            COMMAND "${CMAKE_COMMAND}" "-DDEPLOY_ROOT=${MOCKS_EXPORT_DIR_NORMALIZED}" -DMSVC_RUNTIME_ARCH=x64 "-DMSVC_REDIST_ROOT=${MOCKS_MSVC_REDIST_ROOT_DETECTED}" "-DCONFIG=$<CONFIG>" -P "${MOCKS_CMAKE_SCRIPTS_DIR}/copy_msvc_runtime.cmake"
        )
    elseif(MINGW)
        list(APPEND DEPLOY_COMMANDS
            COMMAND "${CMAKE_COMMAND}" "-DDEPLOY_ROOT=${MOCKS_EXPORT_DIR_NORMALIZED}" -DPRUNE_UNUSED_QT_MODULES=ON -DPRUNE_MSVC_RUNTIME=ON -P "${MOCKS_CMAKE_SCRIPTS_DIR}/prune_optional_root_runtime_dlls.cmake"
        )
    endif()
    foreach(DLL_NAME IN LISTS CRITICAL_DLLS)
        if(EXISTS "${MOCKS_QT_BIN_DIR}/${DLL_NAME}")
            list(APPEND DEPLOY_COMMANDS
                COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${MOCKS_QT_BIN_DIR}/${DLL_NAME}"
                    "${MOCKS_EXPORT_DIR_NORMALIZED}/${DLL_NAME}"
            )
        endif()
    endforeach()
    if(EXISTS "${MOCKS_QT_PREFIX_DIR}/plugins/platforms/qwindows.dll")
        list(APPEND DEPLOY_COMMANDS
            COMMAND "${CMAKE_COMMAND}" -E make_directory "${MOCKS_EXPORT_DIR_NORMALIZED}/plugins/platforms"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "${MOCKS_QT_PREFIX_DIR}/plugins/platforms/qwindows.dll"
                "${MOCKS_EXPORT_DIR_NORMALIZED}/plugins/platforms/qwindows.dll"
        )
    endif()
    if(EXISTS "${MOCKS_QT_PREFIX_DIR}/plugins/styles/qmodernwindowsstyle.dll")
        list(APPEND DEPLOY_COMMANDS
            COMMAND "${CMAKE_COMMAND}" -E make_directory "${MOCKS_EXPORT_DIR_NORMALIZED}/plugins/styles"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "${MOCKS_QT_PREFIX_DIR}/plugins/styles/qmodernwindowsstyle.dll"
                "${MOCKS_EXPORT_DIR_NORMALIZED}/plugins/styles/qmodernwindowsstyle.dll"
        )
    endif()
    if(EXISTS "${MOCKS_QT_PREFIX_DIR}/plugins/imageformats/qico.dll")
        list(APPEND DEPLOY_COMMANDS
            COMMAND "${CMAKE_COMMAND}" -E make_directory "${MOCKS_EXPORT_DIR_NORMALIZED}/plugins/imageformats"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "${MOCKS_QT_PREFIX_DIR}/plugins/imageformats/qico.dll"
                "${MOCKS_EXPORT_DIR_NORMALIZED}/plugins/imageformats/qico.dll"
        )
    endif()
    if(MINGW)
        set(GCC_RUNTIMES libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll)
        foreach(GCC_DLL IN LISTS GCC_RUNTIMES)
            if(EXISTS "${MINGW_BIN_DIR}/${GCC_DLL}")
                list(APPEND DEPLOY_COMMANDS
                    COMMAND "${CMAKE_COMMAND}" -E copy
                        "${MINGW_BIN_DIR}/${GCC_DLL}"
                        "${MOCKS_EXPORT_DIR_NORMALIZED}/${GCC_DLL}"
                )
            elseif(EXISTS "${MOCKS_QT_BIN_DIR}/${GCC_DLL}")
                list(APPEND DEPLOY_COMMANDS
                    COMMAND "${CMAKE_COMMAND}" -E copy
                        "${MOCKS_QT_BIN_DIR}/${GCC_DLL}"
                        "${MOCKS_EXPORT_DIR_NORMALIZED}/${GCC_DLL}"
                )
            endif()
        endforeach()
    endif()
endif()

# 7. Sincronizacao da distribuicao limpa externa de mocks para D:\MISCELANEAS\VSCode\Compression_Manager
if(MSVC)
    set(CURRENT_MOCKS_DIST_NAME "Mocks_msvc")
else()
    set(CURRENT_MOCKS_DIST_NAME "Mocks_mingw")
endif()

find_program(MOCKS_POWERSHELL_EXECUTABLE NAMES powershell.exe pwsh.exe powershell pwsh)
if(MOCKS_POWERSHELL_EXECUTABLE)
    list(APPEND DEPLOY_COMMANDS
        COMMAND "${CMAKE_COMMAND}" -E echo "Sincronizando distribuicao limpa externa de mocks (${CURRENT_MOCKS_DIST_NAME}) em D:/MISCELANEAS/VSCode/Compression_Manager..."
        COMMAND "${MOCKS_POWERSHELL_EXECUTABLE}" -ExecutionPolicy Bypass -File "${MOCKS_CMAKE_SCRIPTS_DIR}/gerar_distribuicoes_limpas.ps1" -DistributionName "${CURRENT_MOCKS_DIST_NAME}"
    )
endif()

add_custom_target(
    MocksDeploy ALL
    DEPENDS
        CompileTranslationsGUI
        CompileTranslationsTerminal
        find_VersionEditor
    ${DEPLOY_COMMANDS}
    COMMENT "Implantando dependencias enxutas Qt 6.12, runtimes e distribuicao externa para os executaveis de mocks"
    VERBATIM
)

add_custom_target(find_VersionEditorDeploy ALL DEPENDS MocksDeploy)
add_custom_target(CompileTranslationsGUIDeploy ALL DEPENDS MocksDeploy)
add_custom_target(CompileTranslationsTerminalDeploy ALL DEPENDS MocksDeploy)

# Executores
if(WIN32)
    set(MOCKS_DETACHED_GUI_LAUNCHER "${CMAKE_CURRENT_SOURCE_DIR}/launch_detached_gui.cmake")

    add_custom_target(
        MocksRun
        DEPENDS MocksDeploy
        COMMAND "${CMAKE_COMMAND}"
            "-DLAUNCH_FILE=${CMAKE_BINARY_DIR}/find_VersionEditor.exe"
            "-DLAUNCH_WORKING_DIR=${CMAKE_BINARY_DIR}"
            -P "${MOCKS_DETACHED_GUI_LAUNCHER}"
        COMMAND "${CMAKE_COMMAND}"
            "-DLAUNCH_FILE=${CMAKE_BINARY_DIR}/CompileTranslationsGUI.exe"
            "-DLAUNCH_WORKING_DIR=${CMAKE_BINARY_DIR}"
            -P "${MOCKS_DETACHED_GUI_LAUNCHER}"
        COMMENT "Iniciando ferramentas mocks (find_VersionEditor e CompileTranslationsGUI)"
        VERBATIM
        USES_TERMINAL
    )

    add_custom_target(
        find_VersionEditorRun
        DEPENDS find_VersionEditorDeploy
        COMMAND "${CMAKE_COMMAND}"
            "-DLAUNCH_FILE=${CMAKE_BINARY_DIR}/find_VersionEditor.exe"
            "-DLAUNCH_WORKING_DIR=${CMAKE_BINARY_DIR}"
            -P "${MOCKS_DETACHED_GUI_LAUNCHER}"
        VERBATIM
        USES_TERMINAL
    )

    add_custom_target(
        CompileTranslationsGUIRunAll
        DEPENDS CompileTranslationsGUIDeploy
        COMMAND "${CMAKE_COMMAND}"
            "-DLAUNCH_FILE=${CMAKE_BINARY_DIR}/CompileTranslationsGUI.exe"
            "-DLAUNCH_WORKING_DIR=${CMAKE_BINARY_DIR}"
            "-DLAUNCH_ARGS=--all"
            -P "${MOCKS_DETACHED_GUI_LAUNCHER}"
        VERBATIM
        USES_TERMINAL
    )

    add_custom_target(
        CompileTranslationsGUIRunCompression
        DEPENDS CompileTranslationsGUIDeploy
        COMMAND "${CMAKE_COMMAND}"
            "-DLAUNCH_FILE=${CMAKE_BINARY_DIR}/CompileTranslationsGUI.exe"
            "-DLAUNCH_WORKING_DIR=${CMAKE_BINARY_DIR}"
            "-DLAUNCH_ARGS=--module=compression"
            -P "${MOCKS_DETACHED_GUI_LAUNCHER}"
        VERBATIM
        USES_TERMINAL
    )
    add_custom_target(CompileTranslationsGUIRunEconomia DEPENDS CompileTranslationsGUIRunCompression)

    add_custom_target(
        CompileTranslationsTerminalRunAll
        DEPENDS CompileTranslationsTerminalDeploy
        COMMAND "${CMAKE_BINARY_DIR}/CompileTranslationsTerminal.exe" --all
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        VERBATIM
        USES_TERMINAL
    )

    add_custom_target(
        CompileTranslationsTerminalRunCompression
        DEPENDS CompileTranslationsTerminalDeploy
        COMMAND "${CMAKE_BINARY_DIR}/CompileTranslationsTerminal.exe" --module compression
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        VERBATIM
        USES_TERMINAL
    )
    add_custom_target(CompileTranslationsTerminalRunEconomia DEPENDS CompileTranslationsTerminalRunCompression)
endif()
