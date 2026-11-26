cmake_minimum_required(VERSION 3.28)

find_package(Qt6 6.12 REQUIRED COMPONENTS Core Gui Widgets Test)

# Fontes de producao compartilhados pelas bibliotecas de teste (sem main.cpp; zero edicao em source/)
set(CM_CORE_SOURCES
    "${CMAKE_SOURCE_DIR}/source/src_01_InicializadorMain.cpp"

    # utils
    "${CMAKE_SOURCE_DIR}/source/utils/LogManager.cpp"
    "${CMAKE_SOURCE_DIR}/source/utils/CaminhoPersistenteUtils.cpp"
    "${CMAKE_SOURCE_DIR}/source/utils/ApplicationPathUtils.cpp"
    "${CMAKE_SOURCE_DIR}/source/utils/IconUtils.cpp"

    # language
    "${CMAKE_SOURCE_DIR}/source/language/tr_01_gerenciadorTraducao.cpp"
    "${CMAKE_SOURCE_DIR}/source/language/tr_02_compileTranslations.cpp"

    # GerenciamentoUI
    "${CMAKE_SOURCE_DIR}/source/GerenciamentoUI/ui_01_layoutsCompressao.cpp"
    "${CMAKE_SOURCE_DIR}/source/GerenciamentoUI/ui_02_gerenteGUILayouts.cpp"
    "${CMAKE_SOURCE_DIR}/source/GerenciamentoUI/ui_03_dragDrop.cpp"
    "${CMAKE_SOURCE_DIR}/source/GerenciamentoUI/ui_04_dialogTraducao.cpp"
    "${CMAKE_SOURCE_DIR}/source/GerenciamentoUI/ui_05_OpcoesSobre.cpp"
    "${CMAKE_SOURCE_DIR}/source/GerenciamentoUI/ui_06_SobreDialog.cpp"
    "${CMAKE_SOURCE_DIR}/source/GerenciamentoUI/ui_07_Manual.cpp"
    "${CMAKE_SOURCE_DIR}/source/GerenciamentoUI/ui_08_Release_en_US.cpp"
    "${CMAKE_SOURCE_DIR}/source/GerenciamentoUI/ui_09_Lancamento_pt_BR.cpp"

    # MotoresCompressao
    "${CMAKE_SOURCE_DIR}/source/MotoresCompressao/mtcomp_01_metodosCompressao.cpp"
    "${CMAKE_SOURCE_DIR}/source/MotoresCompressao/mtcomp_02_motoresCompressao.cpp"

    # InterfaceCore
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/intercore_01_InterfaceGrafica.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_01_init_menu.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_02_update_compression_menus.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_03_mudar_idioma.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_04_retranslateUi.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_05_init_ui.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_06_create_widget_with_layout.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_07_create_main_layout_1.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_08_create_first_quadrant.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_09_create_second_quadrant.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_10_rebuild_method_layouts.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_11_create_main_layout_2.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_12_create_third_quadrant.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_13_create_scroll_area.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_14_create_button.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_15_create_method_checkboxes.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_16_on_method_toggled.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_17_adjust_scroll_area.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_18_remove_layout_widgets.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_19_closeEvent.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_20_exibir_sobre.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_21_toggle_segundo_layout.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_22_create_scroll_area_2.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_23_create_first_quadrant_2.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_24_create_second_quadrant_2.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_25_create_method_checkboxes_2.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_26_on_method_toggled_2.cpp"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_27_adapt_layout_orientation.cpp"
)

set(CM_CORE_INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/source"
    "${CMAKE_SOURCE_DIR}/source/GerenciamentoUI"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore"
    "${CMAKE_SOURCE_DIR}/source/InterfaceCore/GerenciadorInterfaceGrafica"
    "${CMAKE_SOURCE_DIR}/source/MotoresCompressao"
    "${CMAKE_SOURCE_DIR}/source/language"
    "${CMAKE_SOURCE_DIR}/source/utils"
)

function(cm_configure_core_library TARGET_NAME RAR_FLAG)
    add_library(${TARGET_NAME} STATIC ${CM_CORE_SOURCES})
    target_include_directories(${TARGET_NAME} PUBLIC ${CM_CORE_INCLUDE_DIRS})
    target_compile_definitions(${TARGET_NAME} PUBLIC ENABLE_RAR_SUPPORT=${RAR_FLAG})
    target_link_libraries(${TARGET_NAME}
        PUBLIC
            Qt6::Core
            Qt6::Gui
            Qt6::Widgets
    )
    if(WIN32)
        target_link_libraries(${TARGET_NAME} PUBLIC advapi32 ole32 shell32)
    endif()
    if(MSVC)
        set_property(TARGET ${TARGET_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    endif()
endfunction()

cm_configure_core_library(CompressionManagerCoreFull 1)
cm_configure_core_library(CompressionManagerCoreStandart 0)

if(MSVC)
    set(CM_TEST_TOOLCHAIN_NAME "MSVC")
else()
    set(CM_TEST_TOOLCHAIN_NAME "MinGW")
endif()

set(CM_SUITE_TEST_TARGETS)

function(add_cm_suite_test TARGET_NAME SOURCE_FILE CORE_LIB)
    add_executable(${TARGET_NAME} ${SOURCE_FILE} "${CMAKE_CURRENT_SOURCE_DIR}/test_common.hpp")
    set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}"
    )
    target_include_directories(${TARGET_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    target_link_libraries(${TARGET_NAME} PRIVATE ${CORE_LIB} Qt6::Test)
    target_compile_definitions(${TARGET_NAME}
        PRIVATE
            CM_PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
            CM_TEST_BINARY_DIR="${CMAKE_BINARY_DIR}"
            CM_TEST_TOOLCHAIN="${CM_TEST_TOOLCHAIN_NAME}"
            CM_QT_VERSION_EXPECTED="6.12.0"
    )
    if(MSVC)
        set_property(TARGET ${TARGET_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    elseif(MINGW)
        target_link_options(${TARGET_NAME} PRIVATE $<$<CONFIG:Release>:-s>)
    endif()

    add_test(
        NAME ${TARGET_NAME}
        COMMAND $<TARGET_FILE:${TARGET_NAME}>
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
    set_tests_properties(${TARGET_NAME}
        PROPERTIES
            ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
            TIMEOUT 180
    )

    set(_all "${CM_SUITE_TEST_TARGETS}")
    list(APPEND _all ${TARGET_NAME})
    set(CM_SUITE_TEST_TARGETS "${_all}" PARENT_SCOPE)
endfunction()

add_cm_suite_test(test_01_framework_plugins_env     test_01_framework_plugins_env.cpp     CompressionManagerCoreFull)
add_cm_suite_test(test_02_utils_paths_logging       test_02_utils_paths_logging.cpp       CompressionManagerCoreFull)
add_cm_suite_test(test_03_i18n_translations         test_03_i18n_translations.cpp         CompressionManagerCoreFull)
add_cm_suite_test(test_04_assets_legal_licensing    test_04_assets_legal_licensing.cpp    CompressionManagerCoreFull)
add_cm_suite_test(test_05_compression_methods_config test_05_compression_methods_config.cpp CompressionManagerCoreFull)
add_cm_suite_test(test_06_dragdrop_uipi_widgets     test_06_dragdrop_uipi_widgets.cpp     CompressionManagerCoreFull)
add_cm_suite_test(test_07_engines_e2e_archiving     test_07_engines_e2e_archiving.cpp     CompressionManagerCoreFull)
add_cm_suite_test(test_08_ui_full_edition           test_08_ui_full_edition.cpp           CompressionManagerCoreFull)
add_cm_suite_test(test_09_ui_standart_edition       test_09_ui_standart_edition.cpp       CompressionManagerCoreStandart)
add_cm_suite_test(test_10_repo_integrity_hygiene    test_10_repo_integrity_hygiene.cpp    CompressionManagerCoreFull)

# ==============================================================================
# Deploy local isolado SOMENTE dentro de build_suite_* (SEM distribuicao externa!)
# ==============================================================================
set(SUITE_SRC_7ZIP_DIR "")
if(EXISTS "C:/Program Files/7-Zip")
    set(SUITE_SRC_7ZIP_DIR "C:/Program Files/7-Zip")
elseif(EXISTS "C:/Program Files (x86)/7-Zip")
    set(SUITE_SRC_7ZIP_DIR "C:/Program Files (x86)/7-Zip")
endif()

set(SUITE_SRC_WINRAR_DIR "")
if(EXISTS "C:/Program Files/WinRAR")
    set(SUITE_SRC_WINRAR_DIR "C:/Program Files/WinRAR")
elseif(EXISTS "C:/Program Files (x86)/WinRAR")
    set(SUITE_SRC_WINRAR_DIR "C:/Program Files (x86)/WinRAR")
endif()

set(SUITE_MINGW_BIN_DIR "")
if(MINGW)
    get_filename_component(SUITE_MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
endif()

set(SUITE_MSVC_REDIST_ROOT_DETECTED "")
if(MSVC)
    if(DEFINED ENV{VCToolsRedistDir} AND NOT "$ENV{VCToolsRedistDir}" STREQUAL "")
        file(TO_CMAKE_PATH "$ENV{VCToolsRedistDir}" SUITE_MSVC_REDIST_ROOT_DETECTED)
    else()
        foreach(_redist_candidate IN ITEMS
            "C:/Program Files (x86)/Microsoft Visual Studio/18/BuildTools/VC/Redist/MSVC/14.52.37025"
            "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Redist/MSVC/14.52.37025"
            "C:/Program Files (x86)/Microsoft Visual Studio/18/BuildTools/VC/Redist/MSVC"
            "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Redist/MSVC"
        )
            if(EXISTS "${_redist_candidate}")
                set(SUITE_MSVC_REDIST_ROOT_DETECTED "${_redist_candidate}")
                break()
            endif()
        endforeach()
    endif()
endif()

set(SUITE_DEPLOY_COMMANDS
    COMMAND "${CMAKE_COMMAND}" -E echo "Deploying isolated local Qt 6.12 + QtTest + offscreen + engines inside ${CMAKE_BINARY_DIR} (no external distribution)..."
)

if(WIN32 AND EXISTS "${WINDEPLOYQT_EXECUTABLE}")
    set(_suite_windeploy_args
        "-DWINDEPLOYQT_EXECUTABLE=${WINDEPLOYQT_EXECUTABLE}"
        "-DQTPATHS_EXECUTABLE=${QT_QTPATHS_EXECUTABLE}"
        "-DTARGET_FILE=$<TARGET_FILE:test_01_framework_plugins_env>"
        "-DRUNTIME_DIR=${CMAKE_BINARY_DIR}"
        "-DPLUGIN_DIR=${CMAKE_BINARY_DIR}/plugins"
        "-DTRANSLATION_DIR=${CMAKE_BINARY_DIR}/translations"
        "-DQT_PLUGIN_TYPES=styles,imageformats,platforms"
        "-DQT_SKIP_PLUGIN_TYPES=generic,iconengines,networkinformation,platforminputcontexts,tls"
        "-DQT_EXCLUDE_PLUGINS=qpdf,qsvg,qgif,qicns,qjpeg,qtga,qtiff,qwbmp,qwebp,qdirect2d,qminimal"
        "-DNO_COMPILER_RUNTIME=ON"
    )
    if(MINGW AND SUITE_MINGW_BIN_DIR)
        list(APPEND _suite_windeploy_args "-DMINGW_BIN_DIR=${SUITE_MINGW_BIN_DIR}")
    endif()
    list(APPEND SUITE_DEPLOY_COMMANDS
        COMMAND "${CMAKE_COMMAND}" ${_suite_windeploy_args} -P "${CMAKE_SOURCE_DIR}/cmake/deploy_optional_qt_binary.cmake"
    )
endif()

list(APPEND SUITE_DEPLOY_COMMANDS
    COMMAND "${CMAKE_COMMAND}" "-DDEPLOY_ROOT=${CMAKE_BINARY_DIR}" -P "${CMAKE_SOURCE_DIR}/cmake/remove_compiler_runtime_installer.cmake"
)

if(MSVC)
    list(APPEND SUITE_DEPLOY_COMMANDS
        COMMAND "${CMAKE_COMMAND}" "-DDEPLOY_ROOT=${CMAKE_BINARY_DIR}" -DPRUNE_UNUSED_QT_MODULES=ON -DPRUNE_MINGW_RUNTIME=ON -P "${CMAKE_SOURCE_DIR}/cmake/prune_optional_root_runtime_dlls.cmake"
        COMMAND "${CMAKE_COMMAND}" "-DDEPLOY_ROOT=${CMAKE_BINARY_DIR}" -DMSVC_RUNTIME_ARCH=x64 "-DMSVC_REDIST_ROOT=${SUITE_MSVC_REDIST_ROOT_DETECTED}" "-DCONFIG=$<CONFIG>" -P "${CMAKE_SOURCE_DIR}/cmake/copy_msvc_runtime.cmake"
    )
elseif(MINGW)
    list(APPEND SUITE_DEPLOY_COMMANDS
        COMMAND "${CMAKE_COMMAND}" "-DDEPLOY_ROOT=${CMAKE_BINARY_DIR}" -DPRUNE_UNUSED_QT_MODULES=ON -DPRUNE_MSVC_RUNTIME=ON -P "${CMAKE_SOURCE_DIR}/cmake/prune_optional_root_runtime_dlls.cmake"
    )
endif()

# Copiar DLLs Qt 6.12 necessarias (incluindo Qt6Test.dll) e plugins (incluindo qoffscreen.dll apos prune!)
foreach(_qt_dll IN ITEMS Qt6Core.dll Qt6Gui.dll Qt6Widgets.dll Qt6Test.dll)
    if(EXISTS "${QT_BIN_DIR}/${_qt_dll}")
        list(APPEND SUITE_DEPLOY_COMMANDS
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "${QT_BIN_DIR}/${_qt_dll}"
                "${CMAKE_BINARY_DIR}/${_qt_dll}"
        )
    endif()
endforeach()

list(APPEND SUITE_DEPLOY_COMMANDS
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/plugins/platforms"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${QT_PREFIX_DIR}/plugins/platforms/qwindows.dll" "${CMAKE_BINARY_DIR}/plugins/platforms/qwindows.dll"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${QT_PREFIX_DIR}/plugins/platforms/qoffscreen.dll" "${CMAKE_BINARY_DIR}/plugins/platforms/qoffscreen.dll"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/plugins/styles"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${QT_PREFIX_DIR}/plugins/styles/qmodernwindowsstyle.dll" "${CMAKE_BINARY_DIR}/plugins/styles/qmodernwindowsstyle.dll"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/plugins/imageformats"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${QT_PREFIX_DIR}/plugins/imageformats/qico.dll" "${CMAKE_BINARY_DIR}/plugins/imageformats/qico.dll"
)

if(MINGW)
    foreach(_gcc_dll IN ITEMS libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll)
        if(EXISTS "${SUITE_MINGW_BIN_DIR}/${_gcc_dll}")
            list(APPEND SUITE_DEPLOY_COMMANDS
                COMMAND "${CMAKE_COMMAND}" -E copy
                    "${SUITE_MINGW_BIN_DIR}/${_gcc_dll}"
                    "${CMAKE_BINARY_DIR}/${_gcc_dll}"
            )
        elseif(EXISTS "${QT_BIN_DIR}/${_gcc_dll}")
            list(APPEND SUITE_DEPLOY_COMMANDS
                COMMAND "${CMAKE_COMMAND}" -E copy
                    "${QT_BIN_DIR}/${_gcc_dll}"
                    "${CMAKE_BINARY_DIR}/${_gcc_dll}"
            )
        endif()
    endforeach()
endif()

list(APPEND SUITE_DEPLOY_COMMANDS
    COMMAND "${CMAKE_COMMAND}"
        "-DSOURCE_DIR=${CMAKE_SOURCE_DIR}/source/language/translations"
        "-DDEST_DIR=${CMAKE_BINARY_DIR}/translations"
        -DCLEAN_DESTINATION=ON
        -P "${CMAKE_SOURCE_DIR}/cmake/copy_qm_translations.cmake"
    COMMAND "${CMAKE_COMMAND}"
        "-DSOURCE_ASSETS_DIR=${CMAKE_SOURCE_DIR}/source/assets"
        "-DDEST_ASSETS_DIR=${CMAKE_BINARY_DIR}/assets"
        "-DENABLE_RAR_SUPPORT=1"
        "-DINCLUDE_ALL_RELEASE_NOTES=ON"
        -P "${CMAKE_SOURCE_DIR}/cmake/copy_lean_app_assets.cmake"
    COMMAND "${CMAKE_COMMAND}"
        "-DDEST_PLUGINS_DIR=${CMAKE_BINARY_DIR}/plugins"
        "-DSRC_7ZIP_DIR=${SUITE_SRC_7ZIP_DIR}"
        "-DSRC_WINRAR_DIR=${SUITE_SRC_WINRAR_DIR}"
        "-DENABLE_RAR_SUPPORT=1"
        -P "${CMAKE_SOURCE_DIR}/cmake/copy_lean_compression_engines.cmake"
)

add_custom_target(SuiteDeploy ALL
    ${SUITE_DEPLOY_COMMANDS}
    DEPENDS ${CM_SUITE_TEST_TARGETS}
    COMMENT "Deploying local Qt6/QtTest/offscreen/assets/engines in build_suite_* (no external distribution)"
    VERBATIM
)

add_custom_target(SuiteRun
    DEPENDS SuiteDeploy
    COMMAND "${CMAKE_CTEST_COMMAND}" --output-on-failure
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Running complete Compression Manager engineering test suite via CTest"
    VERBATIM
    USES_TERMINAL
)
