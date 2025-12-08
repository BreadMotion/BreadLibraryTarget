# BreadLibraryTarget - ヘッダーオンリーライブラリ用 CMakeLists
# 目的:
#  - add_subdirectory() で親プロジェクトから取り込める
#  - 単体で configure / install / export できる（モジュールとして汎用的）
#  - INTERFACE (ヘッダーオンリー) ターゲットを公開し、親プロジェクトが target_link_libraries() で利用できるようにする
#
# 特長:
#  - Windows 用の互換定義を伝播 (BREAD_TARGET_WIN)
#  - 複数のエイリアスを作成して、他プロジェクトの検出ロジックに合致させる
#  - オプションで Example の追加ビルドをサポート（存在する場合）
#
cmake_minimum_required(VERSION 3.12)
project(BreadLibraryTarget
  VERSION 1.0.0
  DESCRIPTION "BreadLibraryTarget - header-only basic types and target abstraction"
  LANGUAGES CXX
)

# オプション: Example をビルドするかどうか（Example が存在する場合のみ）
option(BREAD_BUILD_EXAMPLE "Build example(s) if provided in Example/ directory" OFF)

# ヘッダーオンリーなので INTERFACE ライブラリとして公開する
add_library(BreadLibraryTarget INTERFACE)

# インクルードパスを公開する（ビルド時はソースルート、インストール時は include）
target_include_directories(BreadLibraryTarget INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)

# プロジェクトによっては特定の C++ 標準を要求することがあるため、必要なら有効化します。
# ここでは Bread ライブラリ自体はヘッダオンリーのため強制はしませんが、
# 消費側で C++20 を使うことを想定する場合は以下をアンコメントしてください。
# target_compile_features(BreadLibraryTarget INTERFACE cxx_std_20)

# Windows 互換のための定義を伝播
if (WIN32)
  target_compile_definitions(BreadLibraryTarget INTERFACE BREAD_TARGET_WIN)
endif()

# 親プロジェクトの検出ロジックに合わせてエイリアスをいくつか用意する
# 例: "BreadLibraryTarget", "Bread::BreadLibraryTarget", "BreadLibrary", "Bread::BreadLibrary"
# 注意: ALIAS はターゲットが存在する状態でのみ作成可能
add_library(Bread::BreadLibraryTarget ALIAS BreadLibraryTarget)
add_library(BreadLibrary ALIAS BreadLibraryTarget)
add_library(Bread::BreadLibrary ALIAS BreadLibraryTarget)

# インストールパスのデフォルト（ユーザが上書き可能）
if(NOT DEFINED BREAD_INSTALL_INCLUDEDIR)
  set(BREAD_INSTALL_INCLUDEDIR "include" CACHE STRING "Installation include directory")
endif()
if(NOT DEFINED BREAD_INSTALL_LIBDIR)
  set(BREAD_INSTALL_LIBDIR "lib" CACHE STRING "Installation library directory")
endif()

# インストールとエクスポート設定
# ターゲットのインストール（INTERFACE でも EXPORT の登録は有益）
install(TARGETS BreadLibraryTarget EXPORT BreadLibraryTargetTargets
  INCLUDES DESTINATION ${BREAD_INSTALL_INCLUDEDIR}
)

# ヘッダをインストール（プロジェクトルートのヘッダを include にコピーする）
# パターンは必要に応じて調整してください（現状: *.h, *.hpp と Target/ サブディレクトリを対象）
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
  DESTINATION ${BREAD_INSTALL_INCLUDEDIR}
  FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
    PATTERN "Target/*"
)

# Export 情報をファイルに書き出す
install(EXPORT BreadLibraryTargetTargets
  FILE BreadLibraryTargetTargets.cmake
  NAMESPACE Bread::
  DESTINATION ${BREAD_INSTALL_LIBDIR}/cmake/BreadLibraryTarget
)

# Package Config (find_package 用) の簡易生成
include(CMakePackageConfigHelpers)

set(CONFIG_INSTALL_DIR "${BREAD_INSTALL_LIBDIR}/cmake/BreadLibraryTarget")

# Build 時点での Config ファイルを作る（テンプレートが無ければ最小構成を自動生成）
set(_config_out "${CMAKE_CURRENT_BINARY_DIR}/BreadLibraryTargetConfig.cmake")
set(_version_out "${CMAKE_CURRENT_BINARY_DIR}/BreadLibraryTargetConfigVersion.cmake")

# 最小構成: exported targets を include する内容を生成
file(WRITE "${_config_out}"
"@PACKAGE_INIT@\n"
"include(\"\${CMAKE_CURRENT_LIST_DIR}/BreadLibraryTargetTargets.cmake\")\n"
)

# バージョンファイル（プロジェクトの VERSION から作成）
write_basic_package_version_file(
  "${_version_out}"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

# インストールする Config / Version ファイル（上で作成したもの）
install(FILES
  "${_config_out}"
  "${_version_out}"
  DESTINATION ${CONFIG_INSTALL_DIR}
)

# Export されたターゲットファイルがインストール先に存在するようにするために、
# 上で install(EXPORT ...) したファイルと同じディレクトリに Config ファイルが置かれる。
# これにより find_package(BreadLibraryTarget CONFIG) が期待通りに動作します。

# 任意: Example が存在すれば add_subdirectory() してビルド可能にする
if (BREAD_BUILD_EXAMPLE)
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Example/CMakeLists.txt")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Example" EXCLUDE_FROM_ALL)
  else()
    message(WARNING "BREAD_BUILD_EXAMPLE=ON ですが Example/CMakeLists.txt が見つかりません。")
  endif()
endif()

# 最後にユーザ向けのメッセージ（configure 時に出力されます）
message(STATUS "BreadLibraryTarget: configured (header-only, install include -> ${BREAD_INSTALL_INCLUDEDIR})")
message(STATUS "BreadLibraryTarget: install config will be available at ${CONFIG_INSTALL_DIR}")
message(STATUS "BreadLibraryTarget: exported target name is 'BreadLibraryTarget' with aliases Bread::BreadLibraryTarget, BreadLibrary, Bread::BreadLibrary")

# EOF
