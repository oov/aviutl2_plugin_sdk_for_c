include/*.h にあるソースコードは aviutl2_plugin_sdk にあるサブモジュール内のソースコードのうち、ヘッダー類をC言語から使いやすいように変換したものです。
新しいコミットがきているかサブモジュールを pull して調べ、あればヘッダー類を最新版へ追従させてください。

なお変換後のファイル名は完全に同一ではなく、変換後のファイル名には接頭辞 `aviutl2_` が付与されます。
例えば aviutl2_plugin_sdk/plugin2.h は変換後は include/aviutl2_plugin2.h のようになります。

作業する際は必ず以下の点を守って行ってください。

- 新しいヘッダーファイルが追加されていた場合は変換済みファイルを新しく作成してください
- ヘッダーファイルが削除されていた場合は変換済みファイルを削除してください
- 追加されたエントリーはなるべく同じ位置に追加、削除されたエントリーも適切に削除してください
- 元のソースコードにあるコメントはなるべく忠実に英語に翻訳して記述してください
- 構造体のメモリーレイアウトや関数の引数の互換性が崩れないように細心の注意を払ってください
- C言語から使えない機能が使われている場合はC言語で使える形に修正してください
- どんな型を使うのか、どんなスタイルで記述するのかは既存の変換済みファイルを参考にしてください
- 元のソースコードはShift_JISで書かれています。iconv を利用して正しいコメントを把握した上で翻訳してください
- 元のソースコードにないコメントは基本的には加えず、C言語用に変更した部分について説明が必要な場合のみ追加してください

作業を円滑に行うため、以下の手順で作業するのがいいでしょう。

1. `cd aviutl2_plugin_sdk` でサブモジュール内に移動する
2. `COMMITID=$(git rev-parse HEAD)` で現在のコミットIDを控えてから `git pull origin main` する
3. `git --no-pager diff $COMMITID | tee ../.working/diff-utf8.txt | iconv -c -f cp932 -t utf-8 > ../.working/diff-shift_jis.txt` で差分を取得
4. 更に `git diff --name-status $COMMITID` で変わっているヘッダーファイル名を列挙して、TODOリストにまとめる
5. `cd ..` でプロジェクトルートに戻り、ファイル毎に `.working/diff-shift_jis.txt` や `.working/diff-utf8.txt` を見ながらヘッダーファイルの変換作業を行い、TODOリストを消化していく
6. 全てのヘッダーファイルを変換し終わったら `git --no-pager diff > .working/diff-c.txt` で変更点を書き出し、`.working/diff-shift_jis.txt` や `.working/diff-utf8.txt` と突き合わせて漏れがないか確認する
7. `git add . && git commit -m "sync to $(cat aviutl2_plugin_sdk/.tools/sdk_last_update.txt)"` でコミットする
8. `git push origin main` でリモートにプッシュする
