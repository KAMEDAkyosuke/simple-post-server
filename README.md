simple-post-server
==================

これはなに？
------------

以下のコマンドでファイルを送りつけられることのみをサポートした、シンプルな HTTP サーバです

    $ curl -F file=@your_file_path http://your_host_address:port_number

使いかた
--------

    #include "simple-post-server.h"
    
    /* ファイル取得時のコールバック関数の定義 */
    static void simple_post_server_callback(post_content_t* post_content)
    {
        printf("filename\n%s\n", post_content->filename);
        printf("content\n%s\n", post_content->body);
    }

    int main(int argc, char** argv)
    {
        /* サーバの起動 */
        start_server(8080, simple_post_server_callback);
        return 0;
    }
