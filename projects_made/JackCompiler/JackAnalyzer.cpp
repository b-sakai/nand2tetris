#include <libxml/tree.h>
#include <libxml/parser.h>
#include <string>
#include "JackTokenizer.hpp"

// libxmlの参考リンク
// https://elsammit-beginnerblg.hatenablog.com/entry/2021/08/01/115314#libxml%E3%82%92%E7%94%A8%E3%81%84%E3%82%8B%E4%B8%8A%E3%81%A7%E3%81%AE%E6%B3%A8%E6%84%8F%E7%82%B9

xmlNodePtr add_node(xmlNodePtr node, 
    char* node_name, char* text, char* attr, char* attr_val){
    
    xmlNodePtr new_node = NULL;
    xmlNodePtr new_text = NULL;

    new_node = xmlNewNode(NULL, (const xmlChar*)node_name);

    if(text != NULL){
        new_text = xmlNewText((const xmlChar*)text);
        xmlAddChild(new_node, new_text);
    }

    if(attr != NULL && attr != ""){
        xmlNewProp(new_node, (const xmlChar*)attr, (const xmlChar*)attr_val);
    }

    xmlAddChild(node, new_node);
    
    return new_node;
}

int main() {
    xmlDocPtr doc;
    xmlNodePtr root_node = NULL;
    xmlNodePtr new_node = NULL;
    int ret = 0;
    doc = xmlNewDoc((const xmlChar*)"1.0");
    root_node = xmlNewNode(NULL, (const xmlChar*)"tokens");
    xmlDocSetRootElement(doc, root_node);    

    string ifilename = "../10/ArrayTest/Main.jack";
    //string ifilename = "../10/test.jack";    
    string ofilename = "../10/ArrayTest/MyMain.xml";
    JackTokenizer tokenizer(ifilename);
    while (true) {
        tokenizer.advance();
        if (tokenizer.hasMoreTokens == false) {
            break;
        }
        switch (tokenizer.tokenType) {
            case KEYWORD:
                {
                string keyword = tokenizer.keyword;
                add_node(root_node, "keyword", (char*)(keyword.c_str()), "", "");
                }
                break;
            case SYMBOL:
                {
                string symbol = tokenizer.symbol;            
                add_node(root_node, "symbol", (char*)(symbol.c_str()), "", "");
                }
                break;
            case IDENTIFIER:
                {
                string identifier = tokenizer.identifier;
                add_node(root_node, "identifier", (char*)(identifier.c_str()), "", "");
                }
                break;
            case INT_CONST:
                {
                string intVal = to_string(tokenizer.intVal);
                add_node(root_node, "integerConstant", (char*)(intVal.c_str()), "", "");
                }
                break;
            case STRING_CONST:
                {
                string stringVal = tokenizer.stringVal;
                add_node(root_node, "stringConstant", (char*)(stringVal.c_str()), "", "");
                }
                break;
            default:
                break;
        }
    }
   ret = xmlSaveFormatFileEnc(ofilename.c_str(), doc, "UTF-8",1);
    if(ret < 0){
        printf("error \n");
        return -1;
    }
    xmlFreeDoc(doc);
    xmlCleanupParser();    
    return 0;
}

int xmlOutputExample() {
    xmlDocPtr doc;
    xmlNodePtr root_node = NULL;
    xmlNodePtr new_node = NULL;
    std::string outputFlile = "./output.xml";
    int ret = 0;

    char nameList[3][10] = {"Tom", "Alice", "Bob"};
    char englishList[3][10] = {"30", "95", "20"};
    char mathList[3][10] = {"90", "85", "35"};
    char scienceList[3][10] = {"80", "75", "30"};

    for(int i=0;i<3;i++){
        new_node = add_node(root_node, "user", NULL, "", "");
        add_node(new_node, "name", nameList[i], "", "");
        add_node(new_node, "English", englishList[i], "", "");
        add_node(new_node, "Math", mathList[i], "", "");
        add_node(new_node, "Science", scienceList[i], "", "");
    }
    ret = xmlSaveFormatFileEnc(outputFlile.c_str(), doc, "UTF-8",1);
    if(ret < 0){
        printf("error \n");
        return -1;
    }
    xmlFreeDoc(doc);
    xmlCleanupParser(); 
}