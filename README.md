# CodeTips

CodeTips は、1980年代末から1990年代前半の DOS / Windows 向けに配布されていた、メモリ管理系ライブラリとサンプル集のアーカイブです。収録されている文書には「CodeTips とは、ソフトウェア IC を具現化する製品の総称」とあり、各パッケージは小さく速いメモリ操作を目的にした部品として提供されていました。

このディレクトリには、当時の配布物をほぼそのままの形で保存しています。販売やサポートは文書中でも終了済みと明記されており、ここでは歴史資料・ソース参照用として扱うのが前提です。

## 収録パッケージ

### `dosac106`

`DosAlloc` 1.0.6。MS-DOS 向けのメモリアロケータです。

- 主なファイル
  - `DOSALLOC.H`: ヘッダ
  - `DA_MAX.C`, `DA_MIX.C`, `DA_RAND.C`, `DA_BENCH.C`: サンプル・ベンチマーク
  - `MAKE.MS`, `MAKE.BC`: Microsoft C / Borland C 向けビルド設定
  - `USERGUID.TXT`, `REFERENC.TXT`, `HISTORY.TXT`: 利用ガイド、API 参照、更新履歴
- ドキュメント上の主な関数
  - `_dInit`, `_dAlloc`, `_dRealloc`, `_dFree`, `_dCheck`, `_dSize`, `_dTerm`

### `nddos100`

`NewDely DOS` 1.00。DOS 向けの `new` / `delete` スーパー クラスです。C++ でのメモリ管理を補助するパッケージとして配布されていました。

- 主なファイル
  - `NEWDELY.H`: ヘッダ
  - `ND_MAX.CPP`, `ND_MIX.CPP`, `ND_BENCH.CPP`: サンプル・ベンチマーク
  - `MAKE.MS`, `MAKE.BC`: ビルド設定
  - `USERGUID.TXT`, `REFERENC.TXT`, `HISTORY.TXT`: ドキュメント
- ドキュメント上の主な機能
  - `new`, `delete`, `size`, `NEWDELY_INITIAL`, `NEWDELY_CHECK`

### `vmdos133`

`VMM_Dos` 1.3.3。DOS 向け仮想メモリマネージャライブラリです。XMS / EMS / DISK などの補助メモリを活用する前提で設計されています。

- 主なファイル
  - `VM_DOS.H`: ヘッダ
  - `VM_MAX.C`, `VM_MIX.C`, `VM_RAND.C`, `VM_BENCH.C`: サンプル・ベンチマーク
  - `MAKE.MS`, `MAKE.BC`: ビルド設定
  - `USERGUID.TXT`, `REFERENC.TXT`, `HISTORY.TXT`, `CATALOG.TXT`: ドキュメント
- ドキュメント上の主な関数
  - `vm_init`, `vm_alloc`, `vm_allocFix`, `vm_load`, `vm_lock`, `vm_unlock`, `vm_free`, `vm_freeQuick`, `vm_freeAll`, `vm_compaction`, `vm_term`

### `winac104`

`WinAlloc` 1.0.4。Windows 3.0 / 3.1 向けメモリアロケータです。保護モード環境での利用を前提にしています。

- 主なファイル
  - `WALLOC.H`, `WINALLOC.H`: ヘッダ
  - `WALLOC.C`, `WPUTS.C`: サンプルソース
  - `WBENCH1.C` - `WBENCH3.C`, `WDEBUG1.C` - `WDEBUG3.C`: ベンチマークとデバッグ用サンプル
  - `MAKE.MS`, `MAKE.BC`, `WALLOC.DEF`, `WALLOC.RC`: ビルド関連ファイル
  - `USERGUID.TXT`, `REFERENC.TXT`, `HISTORY.TXT`, `Q&A.TXT`: ドキュメント
- ドキュメント上の主な関数
  - `_wInit`, `_wAlloc`, `_wRealloc`, `_wFree`, `_wFreeAll`, `_wCheck`, `_wSize`, `_wTerm`

## ディレクトリ構成

```text
CodeTips/
├── dosac106/   # DOS memory allocator
├── nddos100/   # DOS new/delete super class
├── vmdos133/   # DOS virtual memory manager
└── winac104/   # Windows memory allocator
```

各サブディレクトリには、概ね次のファイル群が含まれます。

- ソースコード: `*.C`, `*.CPP`, `*.H`
- コンパイラ別メイクファイル: `MAKE.MS`, `MAKE.BC`
- 配布ライブラリ: `*.LIB`
- 実行サンプル: `*.EXE`
- 文書: `USERGUID.TXT`, `REFERENC.TXT`, `HISTORY.TXT`, `BENCH.TXT` など

## ビルドについて

収録物は当時の Microsoft C / Quick-C / Borland C / Turbo C / Windows SDK を前提にしています。`MAKE.MS` と `MAKE.BC` はそのまま残っていますが、現代の macOS / Linux / Windows 環境でそのまま再ビルドできるとは限りません。

試す場合は、以下のようなレトロ開発環境が必要になります。

- MS-DOS 互換環境
- Windows 3.1 互換環境 (`winac104` の場合)
- 当時の Microsoft または Borland 系コンパイラ

実用目的よりも、以下の用途に向いています。

- 1990年代前後のメモリ管理手法の調査
- 当時の配布形式やドキュメント様式の保存
- レトロPC / DOS / Windows 3.x 開発資料としての参照

## 読み始めるなら

まずは各ディレクトリの `USERGUID.TXT` を読むと全体像をつかみやすいです。詳細な API は `REFERENC.TXT`、変更履歴は `HISTORY.TXT`、ベンチマークの考え方は `BENCH.TXT` にまとまっています。

## 注意

- 文書中の購入案内、送付先、BBS 情報は当時のものです。
- 一部ファイルは Shift_JIS ベースの古いテキストで、環境によって文字化けすることがあります。
- `.EXE` や `.LIB` は歴史的アーカイブとして保存しており、現行環境での動作は想定していません。
