cmake_minimum_required(VERSION 3.28)

project(Compression_Manager_7Z_RAR LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
foreach(_cfg IN ITEMS DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${_cfg} "${CMAKE_BINARY_DIR}")
endforeach()

if(MSVC)
    add_compile_options(/MP)
endif()

# Opções de compilação separadas para app principal e mocks
option(BUILD_APP "Habilitar compilacao da aplicacao principal Compression Manager" ON)
option(BUILD_MOCKS "Habilitar compilacao das ferramentas de mocks" OFF)
option(ENABLE_RAR_SUPPORT "Habilitar suporte e modo RAR (Produto Full)" ON)
option(BUILD_TESTS "Habilitar compilacao da suite completa de testes" OFF)

find_package(Qt6 6.12 REQUIRED COMPONENTS Core Gui Widgets PrintSupport)
find_package(Qt6 6.12 QUIET COMPONENTS LinguistTools)

function(resolve_qt6_target_location TARGET_NAME OUT_VAR)
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

resolve_qt6_target_location(Qt6::Core QT_CORE_IMPORTED_LOCATION)
resolve_qt6_target_location(Qt6::qmake QT_QMAKE_IMPORTED_LOCATION)
resolve_qt6_target_location(Qt6::lrelease QT_LRELEASE_IMPORTED_LOCATION)

if(QT_CORE_IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_CORE_IMPORTED_LOCATION}" DIRECTORY)
    get_filename_component(QT_PREFIX_DIR "${QT_BIN_DIR}" DIRECTORY)
elseif(QT_QMAKE_IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_QMAKE_IMPORTED_LOCATION}" DIRECTORY)
    get_filename_component(QT_PREFIX_DIR "${QT_BIN_DIR}" DIRECTORY)
else()
    get_filename_component(QT_PREFIX_DIR "${Qt6_DIR}/../../.." ABSOLUTE)
    set(QT_BIN_DIR "${QT_PREFIX_DIR}/bin")
endif()

set(WINDEPLOYQT_EXECUTABLE "${QT_BIN_DIR}/windeployqt.exe")
set(QT_QTPATHS_EXECUTABLE "${QT_BIN_DIR}/qtpaths6.exe")
if(NOT EXISTS "${QT_QTPATHS_EXECUTABLE}" AND EXISTS "${QT_BIN_DIR}/qtpaths.exe")
    set(QT_QTPATHS_EXECUTABLE "${QT_BIN_DIR}/qtpaths.exe")
endif()

# Gerar arquivo qt.conf para garantir isolamento e carregamento nativo dos plugins Qt6
file(WRITE "${CMAKE_BINARY_DIR}/qt.conf"
    "[Paths]\nPrefix = .\nPlugins = plugins\nTranslations = translations\n"
)

if(BUILD_APP)
    set(APP_RC_FILE "")
    if(WIN32)
        enable_language(RC)
        set(APP_RC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/source/assets/icones/Compression_Manager.rc")
    endif()

    include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/source
        ${CMAKE_CURRENT_SOURCE_DIR}/source/GerenciamentoUI
        ${CMAKE_CURRENT_SOURCE_DIR}/source/InterfaceCore
        ${CMAKE_CURRENT_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica
        ${CMAKE_CURRENT_SOURCE_DIR}/source/MotoresCompressao
        ${CMAKE_CURRENT_SOURCE_DIR}/source/language
        ${CMAKE_CURRENT_SOURCE_DIR}/source/utils
    )

    set(COMPRESSION_SOURCES
        main.cpp
        source/src_01_InicializadorMain.cpp

        # utils
        source/utils/LogManager.cpp
        source/utils/CaminhoPersistenteUtils.cpp
        source/utils/ApplicationPathUtils.cpp
        source/utils/IconUtils.cpp

        # language
        source/language/tr_01_gerenciadorTraducao.cpp
        source/language/tr_02_compileTranslations.cpp

        # GerenciamentoUI
        source/GerenciamentoUI/ui_01_layoutsCompressao.cpp
        source/GerenciamentoUI/ui_02_gerenteGUILayouts.cpp
        source/GerenciamentoUI/ui_03_dragDrop.cpp
        source/GerenciamentoUI/ui_04_dialogTraducao.cpp
        source/GerenciamentoUI/ui_05_OpcoesSobre.cpp
        source/GerenciamentoUI/ui_06_SobreDialog.cpp
        source/GerenciamentoUI/ui_07_Manual.cpp
        source/GerenciamentoUI/ui_08_Release_en_US.cpp
        source/GerenciamentoUI/ui_09_Lancamento_pt_BR.cpp

        # MotoresCompressao
        source/MotoresCompressao/mtcomp_01_metodosCompressao.cpp
        source/MotoresCompressao/mtcomp_02_motoresCompressao.cpp

        # InterfaceCore
        source/InterfaceCore/intercore_01_InterfaceGrafica.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_01_init_menu.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_02_update_compression_menus.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_03_mudar_idioma.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_04_retranslateUi.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_05_init_ui.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_06_create_widget_with_layout.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_07_create_main_layout_1.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_08_create_first_quadrant.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_09_create_second_quadrant.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_10_rebuild_method_layouts.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_11_create_main_layout_2.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_12_create_third_quadrant.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_13_create_scroll_area.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_14_create_button.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_15_create_method_checkboxes.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_16_on_method_toggled.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_17_adjust_scroll_area.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_18_remove_layout_widgets.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_19_closeEvent.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_20_exibir_sobre.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_21_toggle_segundo_layout.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_22_create_scroll_area_2.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_23_create_first_quadrant_2.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_24_create_second_quadrant_2.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_25_create_method_checkboxes_2.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_26_on_method_toggled_2.cpp
        source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_27_adapt_layout_orientation.cpp
    )

    # Executável principal (nome do binário compilado: Compression_Manager.exe)
    add_executable(Compression_Manager WIN32 ${COMPRESSION_SOURCES} ${APP_RC_FILE})
    set_target_properties(Compression_Manager PROPERTIES OUTPUT_NAME "Compression_Manager")

    if(ENABLE_RAR_SUPPORT)
        target_compile_definitions(Compression_Manager PRIVATE ENABLE_RAR_SUPPORT=1)
    else()
        target_compile_definitions(Compression_Manager PRIVATE ENABLE_RAR_SUPPORT=0)
    endif()

    target_link_libraries(Compression_Manager
        PRIVATE
            Qt6::Core
            Qt6::Gui
            Qt6::Widgets
    )

    if(WIN32)
        target_link_libraries(Compression_Manager PRIVATE advapi32 ole32 shell32)
    endif()

    if(MSVC)
        set_property(TARGET Compression_Manager PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    elseif(MINGW)
        target_link_options(Compression_Manager PRIVATE $<$<CONFIG:Release>:-s>)
    endif()

    # Target para compatibilidade retroativa
    add_custom_target(Compression_Manager_7Z_RAR DEPENDS Compression_Manager)

    # Executável auxiliar de teste
    add_executable(teste teste.cpp source/utils/ApplicationPathUtils.cpp source/utils/LogManager.cpp)
    target_link_libraries(teste PRIVATE Qt6::Core)
    if(MSVC)
        set_property(TARGET teste PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    elseif(MINGW)
        target_link_options(teste PRIVATE $<$<CONFIG:Release>:-s>)
    endif()

    # Origens dos binários externos do 7-Zip e WinRAR para cópia enxuta automatizada
    set(SRC_7ZIP_DIR "")
    if(EXISTS "C:/Program Files/7-Zip")
        set(SRC_7ZIP_DIR "C:/Program Files/7-Zip")
    elseif(EXISTS "C:/Program Files (x86)/7-Zip")
        set(SRC_7ZIP_DIR "C:/Program Files (x86)/7-Zip")
    endif()

    set(SRC_WINRAR_DIR "")
    if(EXISTS "C:/Program Files/WinRAR")
        set(SRC_WINRAR_DIR "C:/Program Files/WinRAR")
    elseif(EXISTS "C:/Program Files (x86)/WinRAR")
        set(SRC_WINRAR_DIR "C:/Program Files (x86)/WinRAR")
    endif()

    # Copiar somente recursos, traduções e motores estritamente necessários no POST_BUILD
    add_custom_command(TARGET Compression_Manager POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E rm -f "${CMAKE_BINARY_DIR}/Compression_Manager_7Z_RAR.exe"
        COMMAND "${CMAKE_COMMAND}" -E rm -rf "${CMAKE_BINARY_DIR}/7Z" "${CMAKE_BINARY_DIR}/WinRAR"
        COMMAND "${CMAKE_COMMAND}"
            "-DDEST_PLUGINS_DIR=${CMAKE_BINARY_DIR}/plugins"
            "-DSRC_7ZIP_DIR=${SRC_7ZIP_DIR}"
            "-DSRC_WINRAR_DIR=${SRC_WINRAR_DIR}"
            "-DENABLE_RAR_SUPPORT=${ENABLE_RAR_SUPPORT}"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/copy_lean_compression_engines.cmake"
        COMMAND "${CMAKE_COMMAND}"
            "-DSOURCE_ASSETS_DIR=${CMAKE_CURRENT_SOURCE_DIR}/source/assets"
            "-DDEST_ASSETS_DIR=${CMAKE_BINARY_DIR}/assets"
            "-DENABLE_RAR_SUPPORT=${ENABLE_RAR_SUPPORT}"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/copy_lean_app_assets.cmake"
        COMMAND "${CMAKE_COMMAND}"
            "-DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/source/language/translations"
            "-DDEST_DIR=${CMAKE_BINARY_DIR}/translations"
            -DCLEAN_DESTINATION=ON
            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/copy_qm_translations.cmake"
        COMMENT "Copiando motores, recursos enxutos e traducoes do projeto..."
    )

    set(MINGW_BIN_DIR "")
    if(MINGW)
        get_filename_component(MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
    endif()

    set(MSVC_REDIST_ROOT_DETECTED "")
    if(MSVC)
        if(DEFINED ENV{VCToolsRedistDir} AND NOT "$ENV{VCToolsRedistDir}" STREQUAL "")
            file(TO_CMAKE_PATH "$ENV{VCToolsRedistDir}" MSVC_REDIST_ROOT_DETECTED)
        else()
            foreach(_redist_candidate IN ITEMS
                "C:/Program Files (x86)/Microsoft Visual Studio/18/BuildTools/VC/Redist/MSVC/14.52.37025"
                "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Redist/MSVC/14.52.37025"
                "C:/Program Files (x86)/Microsoft Visual Studio/18/BuildTools/VC/Redist/MSVC"
                "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Redist/MSVC"
            )
                if(EXISTS "${_redist_candidate}")
                    set(MSVC_REDIST_ROOT_DETECTED "${_redist_candidate}")
                    break()
                endif()
            endforeach()
        endif()
    endif()

    set(DEPLOY_COMMANDS
        COMMAND "${CMAKE_COMMAND}" -E echo "Deploying lean Qt 6.12 dependencies, plugins, translations, runtimes, and assets for Compression Manager..."
        COMMAND "${CMAKE_COMMAND}" -E rm -f "${CMAKE_BINARY_DIR}/Compression_Manager_7Z_RAR.exe"
    )

    if(WIN32 AND EXISTS "${WINDEPLOYQT_EXECUTABLE}")
        set(_deploy_args
            "-DWINDEPLOYQT_EXECUTABLE=${WINDEPLOYQT_EXECUTABLE}"
            "-DQTPATHS_EXECUTABLE=${QT_QTPATHS_EXECUTABLE}"
            "-DTARGET_FILE=$<TARGET_FILE:Compression_Manager>"
            "-DRUNTIME_DIR=${CMAKE_BINARY_DIR}"
            "-DPLUGIN_DIR=${CMAKE_BINARY_DIR}/plugins"
            "-DTRANSLATION_DIR=${CMAKE_BINARY_DIR}/translations"
            "-DQT_PLUGIN_TYPES=styles,imageformats,platforms"
            "-DQT_SKIP_PLUGIN_TYPES=generic,iconengines,networkinformation,platforminputcontexts,tls"
            "-DQT_EXCLUDE_PLUGINS=qpdf,qsvg,qgif,qicns,qjpeg,qtga,qtiff,qwbmp,qwebp,qdirect2d,qminimal,qoffscreen"
            "-DNO_COMPILER_RUNTIME=ON"
        )
        if(MINGW AND MINGW_BIN_DIR)
            list(APPEND _deploy_args "-DMINGW_BIN_DIR=${MINGW_BIN_DIR}")
        endif()
        list(APPEND DEPLOY_COMMANDS
            COMMAND "${CMAKE_COMMAND}" ${_deploy_args} -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/deploy_optional_qt_binary.cmake"
        )
    endif()

    list(APPEND DEPLOY_COMMANDS
        COMMAND "${CMAKE_COMMAND}" "-DDEPLOY_ROOT=${CMAKE_BINARY_DIR}" -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/remove_compiler_runtime_installer.cmake"
    )

    if(MSVC)
        list(APPEND DEPLOY_COMMANDS
            COMMAND "${CMAKE_COMMAND}" "-DDEPLOY_ROOT=${CMAKE_BINARY_DIR}" -DPRUNE_UNUSED_QT_MODULES=ON -DPRUNE_MINGW_RUNTIME=ON -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/prune_optional_root_runtime_dlls.cmake"
            COMMAND "${CMAKE_COMMAND}" "-DDEPLOY_ROOT=${CMAKE_BINARY_DIR}" -DMSVC_RUNTIME_ARCH=x64 "-DMSVC_REDIST_ROOT=${MSVC_REDIST_ROOT_DETECTED}" "-DCONFIG=$<CONFIG>" -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/copy_msvc_runtime.cmake"
        )
    elseif(MINGW)
        list(APPEND DEPLOY_COMMANDS
            COMMAND "${CMAKE_COMMAND}" "-DDEPLOY_ROOT=${CMAKE_BINARY_DIR}" -DPRUNE_UNUSED_QT_MODULES=ON -DPRUNE_MSVC_RUNTIME=ON -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/prune_optional_root_runtime_dlls.cmake"
        )
    endif()

    list(APPEND DEPLOY_COMMANDS
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/plugins/platforms"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${QT_PREFIX_DIR}/plugins/platforms/qwindows.dll" "${CMAKE_BINARY_DIR}/plugins/platforms/qwindows.dll"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/plugins/styles"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${QT_PREFIX_DIR}/plugins/styles/qmodernwindowsstyle.dll" "${CMAKE_BINARY_DIR}/plugins/styles/qmodernwindowsstyle.dll"
    )

    if(EXISTS "${QT_PREFIX_DIR}/plugins/imageformats/qico.dll")
        list(APPEND DEPLOY_COMMANDS
            COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/plugins/imageformats"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${QT_PREFIX_DIR}/plugins/imageformats/qico.dll" "${CMAKE_BINARY_DIR}/plugins/imageformats/qico.dll"
        )
    endif()

    # Copiar DLLs reais do MinGW-w64 instalado (ex.: mingw1510_64) sobrescrevendo eventuais versoes anteriores de Qt\6.12.0\mingw_64\bin
    if(MINGW)
        set(GCC_RUNTIMES libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll)
        foreach(GCC_DLL IN LISTS GCC_RUNTIMES)
            if(EXISTS "${MINGW_BIN_DIR}/${GCC_DLL}")
                list(APPEND DEPLOY_COMMANDS
                    COMMAND "${CMAKE_COMMAND}" -E copy
                        "${MINGW_BIN_DIR}/${GCC_DLL}"
                        "${CMAKE_BINARY_DIR}/${GCC_DLL}"
                )
            elseif(EXISTS "${QT_BIN_DIR}/${GCC_DLL}")
                list(APPEND DEPLOY_COMMANDS
                    COMMAND "${CMAKE_COMMAND}" -E copy
                        "${QT_BIN_DIR}/${GCC_DLL}"
                        "${CMAKE_BINARY_DIR}/${GCC_DLL}"
                )
            endif()
        endforeach()
    endif()

    list(APPEND DEPLOY_COMMANDS
        COMMAND "${CMAKE_COMMAND}"
            "-DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/source/language/translations"
            "-DDEST_DIR=${CMAKE_BINARY_DIR}/translations"
            -DCLEAN_DESTINATION=ON
            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/copy_qm_translations.cmake"
        COMMAND "${CMAKE_COMMAND}"
            "-DSOURCE_ASSETS_DIR=${CMAKE_CURRENT_SOURCE_DIR}/source/assets"
            "-DDEST_ASSETS_DIR=${CMAKE_BINARY_DIR}/assets"
            "-DENABLE_RAR_SUPPORT=${ENABLE_RAR_SUPPORT}"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/copy_lean_app_assets.cmake"
        COMMAND "${CMAKE_COMMAND}"
            "-DDEST_PLUGINS_DIR=${CMAKE_BINARY_DIR}/plugins"
            "-DSRC_7ZIP_DIR=${SRC_7ZIP_DIR}"
            "-DSRC_WINRAR_DIR=${SRC_WINRAR_DIR}"
            "-DENABLE_RAR_SUPPORT=${ENABLE_RAR_SUPPORT}"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/copy_lean_compression_engines.cmake"
        COMMAND "${CMAKE_COMMAND}" -E rm -rf
            "${CMAKE_BINARY_DIR}/platforms"
            "${CMAKE_BINARY_DIR}/styles"
            "${CMAKE_BINARY_DIR}/iconengines"
            "${CMAKE_BINARY_DIR}/imageformats"
            "${CMAKE_BINARY_DIR}/icones"
            "${CMAKE_BINARY_DIR}/source"
            "${CMAKE_BINARY_DIR}/7Z"
            "${CMAKE_BINARY_DIR}/WinRAR"
            "${CMAKE_BINARY_DIR}/ABOUT"
            "${CMAKE_BINARY_DIR}/CLC"
            "${CMAKE_BINARY_DIR}/COPYRIGHT"
            "${CMAKE_BINARY_DIR}/EULA"
            "${CMAKE_BINARY_DIR}/LICENSES"
            "${CMAKE_BINARY_DIR}/NOTICES"
            "${CMAKE_BINARY_DIR}/PRIVACY_POLICY"
            "${CMAKE_BINARY_DIR}/RELEASE"
    )

    # Sincronizacao da distribuicao limpa externa para D:\MISCELANEAS\VSCode\Compression_Manager
    if(ENABLE_RAR_SUPPORT)
        if(MSVC)
            set(CURRENT_DIST_NAME "Compression_Manager_Full_msvc")
        else()
            set(CURRENT_DIST_NAME "Compression_Manager_Full_mingw")
        endif()
    else()
        if(MSVC)
            set(CURRENT_DIST_NAME "Compression_Manager_Standart_msvc")
        else()
            set(CURRENT_DIST_NAME "Compression_Manager_Standart_mingw")
        endif()
    endif()

    find_program(POWERSHELL_EXECUTABLE NAMES powershell.exe pwsh.exe powershell pwsh)
    if(POWERSHELL_EXECUTABLE)
        list(APPEND DEPLOY_COMMANDS
            COMMAND "${CMAKE_COMMAND}" -E echo "Sincronizando distribuicao limpa externa (${CURRENT_DIST_NAME}) em D:/MISCELANEAS/VSCode/Compression_Manager..."
            COMMAND "${POWERSHELL_EXECUTABLE}" -ExecutionPolicy Bypass -File "${CMAKE_CURRENT_SOURCE_DIR}/cmake/gerar_distribuicoes_limpas.ps1" -DistributionName "${CURRENT_DIST_NAME}"
        )
    endif()

    add_custom_target(Compression_ManagerDeploy ALL
        ${DEPLOY_COMMANDS}
        DEPENDS Compression_Manager
        COMMENT "Deploying lean Qt 6.12 dependencies, modern style plugins, runtimes, assets, and external distribution for Compression Manager"
        VERBATIM
    )

    add_custom_target(Compression_Manager_7Z_RARDeploy
        DEPENDS Compression_ManagerDeploy
    )
endif()

# Subdiretório de Mocks
if(BUILD_MOCKS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/mocks/CMakeLists.txt")
    add_subdirectory(mocks)
endif()

# Subdiretório da Suite Completa de Testes (sem distribuição externa)
if(BUILD_TESTS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    enable_testing()
    add_subdirectory(tests)
endif()
