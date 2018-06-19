#ifndef MD
#define MD
#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <vector>
using namespace std;
enum{
	maxlength =  10000,
	nul = 0,
	paragraph = 1,
	href = 2,
	ul = 3,
	ol = 4,
	li = 5,
	em = 6,
	strong = 7,
	hr = 8,
	br = 9,
	image = 10,
	quote = 11,
	h1 = 12,
	h2 = 13,
	h3 = 14,
	h4 = 15,
	h5 = 16,
	h6 = 17,
	blockcode = 18,
	code = 19
};
const std::string frontTag[] = {
	"","<p>","","<ul>","<ol>","<li>","<em>","<strong>",
    "<hr color=#CCCCCC size=1 />","<br />",
    "","<blockquote>",
    "<h1 ","<h2 ","<h3 ","<h4 ","<h5 ","<h6 ",
    "<pre><code>","<code>"
};
const std::string backTag[] = {
    "","</p>","","</ul>","</ol>","</li>","</em>","</strong>",
    "","","","</blockquote>",
    "</h1>","</h2>","</h3>","</h4>","</h5>","</h6>",
    "</code></pre>","</code>"
};

typedef struct Cnode{
	std::vector <Cnode *> ch;
	std::string heading;
	std::string tag;
	Cnode (const std::string &hd):heading(hd){}
} Cnode;

typedef struct node{
	int type;
	std::vector <node *> ch;
	std::string elem[3];
	node(int _type):type(_type){}
} node;
class Markdown{
private:
	std::string content, TOC;
	node *root, *now;
	Cnode *Croot;
	int cntTag = 0;
	char s[maxlength];
inline pair<int, char *> start(char *src){
	if ((int)strlen(src) == 0){
		return make_pair(0, nullptr);
	}
	int cntspace = 0, cnttab = 0;
	for (int i = 0; src[i] != '\0'; i++){
		if (src[i] == ' ') cntspace++;
		else if (src[i] == '\t') cnttab++;
		else return make_pair(cnttab + cntspace / 4, src + i);
	}
	return make_pair(0, nullptr);
}
inline pair<int, char *> JudgeType(char *src){
	char *ptr = src;
	while (*ptr == '#') ptr++;

	if (ptr - src > 0 && *ptr == ' '){
		return make_pair(ptr - src + h1 - 1, ptr + 1);
	}

	ptr = src;

	if (strncmp(ptr, "```", 3) == 0){
		return make_pair(blockcode, ptr + 3);
	}

	if (strncmp(ptr, "- ", 2) == 0){
		return make_pair(ul, ptr + 1);
	}

	if (*ptr == '>' && (ptr[1] == ' ')){
		return make_pair(quote, ptr + 1);
	}

	char *ptr1 = ptr;
	while (*ptr1 && (isdigit(*ptr1))) ptr1++;
	if (ptr1 != ptr && *ptr1 == '.' && ptr1[1] == ' '){
		return make_pair(ol, ptr1 + 1);
	}
	return make_pair(paragraph, ptr);
}
public:
	Markdown(){}
	std::string getContents(){
		return content;
	}
	std::string getTableOfContents(){
		return TOC;
	}
	~Markdown(){}
};
#endif