#include <string>
#include "CompilationEngine.hpp"

// libxmlの参考リンク
// https://elsammit-beginnerblg.hatenablog.com/entry/2021/08/01/115314#libxml%E3%82%92%E7%94%A8%E3%81%84%E3%82%8B%E4%B8%8A%E3%81%A7%E3%81%AE%E6%B3%A8%E6%84%8F%E7%82%B9

int main() {
    string ifilename = "../10/ArrayTest/Main.jack";
    //string ifilename = "../10/test.jack";
    string ofilename = "./output.xml";
    //string ofilename = "../10/ArrayTest/MyMain.xml";
    CompilationEngine engine(ifilename, ofilename);
    engine.compile();
    return 0;
}