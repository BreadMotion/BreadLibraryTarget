# BreadLibraryTarget

BreadLibraryTarget は、クロスプラットフォーム開発のための基本型定義とプラットフォーム抽象化を提供する、C++ ヘッダーオンリーライブラリです。
主に `s32`, `u32` などの固定長整数型の定義や、プラットフォーム固有のヘッダインクルードを管理します。

## 特徴

- **ヘッダーオンリー**: ビルド不要で、インクルードパスを通すだけで利用可能。
- **基本型定義**: `s8`, `u8`, `s32`, `u32`, `f32` などの短縮型名をグローバルおよび `Bread` 名前空間で提供。
- **プラットフォーム抽象化**: `Target/Target.h` を通じて環境ごとの差異を吸収（現在は Windows 環境を主対象）。

## 導入方法

本ライブラリはヘッダーオンリーであるため、プロジェクトのインクルードパスに本リポジトリのルートディレクトリを追加するだけで利用できます。

### CMake での利用例 (FetchContent)

CMake の `FetchContent` を使用して自動的にダウンロード・設定する方法を推奨します。

```cmake
include(FetchContent)

FetchContent_Declare(
  BreadLibraryTarget
  GIT_REPOSITORY https://github.com/BreadMotion/BreadLibraryTarget.git
  GIT_TAG        main
)

FetchContent_MakeAvailable(BreadLibraryTarget)

# FetchContent で取得したソースディレクトリをインクルードパスに追加
if(DEFINED breadlibrarytarget_SOURCE_DIR)
  include_directories(${breadlibrarytarget_SOURCE_DIR})
endif()
```

### サブモジュールとして利用する場合

```bash
git submodule add https://github.com/BreadMotion/BreadLibraryTarget.git External/BreadLibraryTarget
```

CMakeLists.txt:
```cmake
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/External/BreadLibraryTarget)
```

## 使い方

インクルードパスが通っていれば、以下のようにインクルードして使用します。

```cpp
#include <Types.h>
#include <Target/Target.h>

void Example() {
    // グローバル型名の使用
    s32 value = 100;
    u64 bigValue = 1234567890ULL;

    // Bread 名前空間の使用
    Bread::f32 floatValue = 1.0f;
}
```

## ディレクトリ構成

- `Types.h`: 基本型定義 (s32, u32 等)
- `Target/`: プラットフォーム依存の定義ファイル群

## 使用されているリポジトリ一覧

- [FNDLibrary](https://github.com/BreadMotion/FNDLibrary)

## ライセンス

本ライブラリは [MIT License](LICENSE) の下で公開されています。