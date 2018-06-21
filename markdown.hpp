#ifndef Markdown
#define Markdown

#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <vector>
#include <cstdio>
#define maxLength 10000
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
/*
保存目录的结构
成员变量：一个子目录的数组，标题内容，标签内容
构造函数：初始化head的内容
*/
typedef struct Cnode{
	std::vector <Cnode *> ch;
	std::string heading;
	std::string tag;
	Cnode (const std::string &hd):heading(hd){}
} Cnode;
/*
保存正文内容的结构
正文需要知道它的结点文本类型，然后还有它的子内容。
构造函数只需要初始化文本类型即可
*/
typedef struct node{
	int type;
	std::vector <node *> ch;
	std::string elem[3];			//elem[0]保存显示的内容
									//elem[1]保存了链接，elem[2]保存了标题
	node(int _type):type(_type){}
} node;
class MarkdownTransform{
private:
	std::string content, TOC;
	node *root, *now;
	Cnode *Croot;
	int cntTag = 0;
	char s[maxlength];
	//先把前面的空格和tab给处理了
	//返回的pair的第一个元素是类型对应的数值，第二个元素是偏移后的地址
 	// std::pair 实质上是一个结构体, 可以将两个数据组合成一个数据
	// 计算一行中开始的空格和 Tab 数
	inline pair<int, char *> start(char *src){
		//如果长度为0，则返回0和空指针，其中0代表enum里的nul。
		if ((int)strlen(src) == 0){
			return make_pair(0, nullptr);
		}
		//记录空格的数量和tab的数量
		//做个标记
		int cntspace = 0, cnttab = 0;
		for (int i = 0; src[i] != '\0'; i++){
			if (src[i] == ' ') cntspace++;
			else if (src[i] == '\t') cnttab++;
			else return make_pair(cnttab + cntspace / 4, src + i);
		}
		return make_pair(0, nullptr);
	}
	//判断正文的类型
	//返回的pair的第一个元素是类型对应的数值，第二个元素是关键字偏移后的地址
	inline pair<int, char *> JudgeType(char *src){
		char *ptr = src;
		//如果有#说明是标题
		while (*ptr == '#') ptr++;

		if (ptr - src > 0 && *ptr == ' '){
			//ptr - src返回值是#的个数x，x + h1 - 1就是对应的标题
			return make_pair(ptr - src + h1 - 1, ptr + 1);
		}
		//如果上面的if没有返回，则将ptr重置然后继续判断
		ptr = src;
		//如果是前三个字符是```则表示是代码，返回blockcode,偏移量为3
		if (strncmp(ptr, "```", 3) == 0){
			return make_pair(blockcode, ptr + 3);
		}
		//如果出现“-”且下一个字符是空格，则说明是列表
		if (strncmp(ptr, "- ", 2) == 0){
			return make_pair(ul, ptr + 1);
		}
		//如果出现“>”且下一个字符是空格，则说明是引用，返回quote和一个偏移量
		if (*ptr == '>' && (ptr[1] == ' ')){
			return make_pair(quote, ptr + 1);
		}
		//如果ptr1是数字，而且ptr1的下一个字符是"."，则说明是有序列表
		char *ptr1 = ptr;
		while (*ptr1 && (isdigit(*ptr1))) ptr1++;
		if (ptr1 != ptr && *ptr1 == '.' && ptr1[1] == ' '){
			return make_pair(ol, ptr1 + 1);
		}
		return make_pair(paragraph, ptr);
	}
	//是否是标题
	inline bool isHeading(node *v){
		return (v->type >= h1 && v->type <= h6);
	}
	//是否是图片
	inline bool isImage(node *v){
		return (v->type == image);
	}
	//是否为超链接
	inline bool isHref(node * v){
		return (v->type == href);
	}
	//给定树的深度寻找结点
	inline node * findnode(int depth){
		node *ptr = root;
		while (!ptr->ch.empty() && depth != 0){
			ptr = ptr->ch.back();
			if (ptr->type == li)
				depth--;
		}
		return ptr;
	}
	//处理标题
	//新增结点的插入
	//传入一个节点，要插入的深度，以及标题内容，还有tag标签
	void Cins(Cnode *v, int x, const string &hd, int tag){
		//n为当前结点的子节点数组的长度
		int n = (int)v->ch.size();
		//如果x为1，则插入
		if (x == 1){
			//把一个新节点插入到当前结点的子节点数组列表里，并且新结点的内容是标题内容
			v->ch.push_back(new Cnode(hd));
			v->ch.back()->tag = "tag" + to_string(tag);
			return ;
		}
		//如果长度为0，并且尾部元素为空，就插入一个空的结点
		if (!n || v->ch.back()->heading.empty())
	   		v->ch.push_back(new Cnode(""));
	   	//然后继续下一步递归
	    Cins(v->ch.back(), x - 1, hd, tag);
	}
	//向指定结点中插入要处理的字符串
	//v:节点
	//src:要处理的字符串
	void insert(node *v, const string &src){
		int n = (int)src.size();
		bool 	incode = false, 	//是否是代码
				inem = false,   	//是否是斜体
				instrong = false, 	//是否是加粗
				inautolink = false; //是否是链接
		v->ch.push_back(new node(nul));

		for (int i = 0; i < n; i++){
			char ch = src[i];
			//如果是注释，则continue
			if (ch == '\\'){
				ch = src[++i];
				//显示的内容为src[++i]
				v->ch.back()->elem[0] += string(1, ch);
				continue;
			}
			//处理行内代码
			if (ch == '`' && inautolink){
				//正文结构的构造函数的参数就是文本类型
				//如果不是代码，则文本类型为nul
				if (incode){
					v->ch.push_back(new node(nul));
				}
				else{
				//如果是代码，则文本类型为code
					v->ch.push_back(new node(code));
					incode = !incode;
				}
				continue;
			}
			//如果出现两个连续的**则为加粗文本
			if (ch == '*' && (i < n - 1 && (src[i + 1] == '*')) && !incode && !inautolink){
				++i;//越过第二个*
				//如果被标记过不是加粗，则放入一个空的节点
				if (instrong){
					v->ch.push_back(new node(nul));
				}
				//否则，放入strong
				else{
					v->ch.push_back(new node(strong));
				}
				continue;
			}
			//处理斜体
	        if (ch == '_' && !incode && !instrong && !inautolink) {
	            inem ? v->ch.push_back(new node(nul)) : v->ch.push_back(new node(em));
	            inem = !inem;
	            continue;
	        }
	        //处理图片
	        //如果第一个字符是“！”而且第二个字符是“[”，并且src[i] 不是最后一个元素
	        if (ch == '!' && (i < n - 1 && src[i + 1] == '[')
	        	&& !incode && !instrong && !inem && !inautolink){
	        	v->ch.push_back(new node(image));
	        	//elem[0]保存的是显示的内容
	        	//将字符逐个放入elem[0]
	        	for (i += 2; i < n - 1 && src[i] != ']'; i++){
	        		v->ch.back()->elem[0] += string(1, src[i]);
	        	}
	        	i++;
	        	//elem[1]保存的是链接
	        	//将字符逐个放入elem[1]
	        	for (i++; i < n - 1 && src[i] != ' ' && src[i] != ')'; i++){
	        		v->ch.back()->elem[1] += string(1, src[i]);
	        	}
	        	//elem[2] 保存的是标题
	        	//将字符逐个放入elem[1]
	        	if (src[i] != ')'){
	        		for (i++; i < n - 1 && src[i] != ')'; i++)
                    if (src[i] != '"')
                        v->ch.back()->elem[2] += string(1, src[i]);
	        	}
	        	//最后放入一个空节点
	        	v->ch.push_back(new node(nul));
	        	continue;
	        }
	        //处理超链接
	        //判断是否是超链接的条件
	        if (ch == '[' && !incode && !instrong && !inem && !inautolink) {
	        	//放入href，其中href在enum中代表超链接
	            v->ch.push_back(new node(href));
	            //elem[0]存放显示内容，所以将[]之间的字符全部放入elem[0]
	            for (i++; i < n - 1 && src[i] != ']'; i++)
	                v->ch.back()->elem[0] += string(1, src[i]);
	            i++;
	            //elem[1]存放链接，所以将[]之间的字符全部放入elem[1]
	            for (i++; i < n - 1 && src[i] != ' ' && src[i] != ')'; i++)
	                v->ch.back()->elem[1] += string(1, src[i]);
	            //elem[2]存放标题，所以将[]之间的字符全部放入elem[1]
	            if (src[i] != ')')
	                for (i++; i < n - 1 && src[i] != ')'; i++)
	                    if (src[i] != '"')
	                        v->ch.back()->elem[2] += string(1, src[i]);
	            v->ch.push_back(new node(nul));
	            continue;
	        }
         	v->ch.back()->elem[0] += string(1, ch);
        	if (inautolink) v->ch.back()->elem[1] += string(1, ch);	
		}
	    if (src.size() >= 2)
	        if (src.at(src.size() - 1) == ' ' && src.at(src.size() - 2) == ' ')
	            v->ch.push_back(new node(br));
	}
	inline bool isCutline(char *src){
		int cnt = 0;
		char *ptr = src;
		while (*ptr){
			//如果不是空格，tab, -, 或 *,那么则不需要换行
	        if (*ptr != ' ' && *ptr != '\t' && *ptr != '-')
	            return false;
	       	if (*ptr == '-')
	       		cnt++;
       		ptr++;
		}
		//如果出现了大于等于3个“-”，则需要换行
		return (cnt >= 3);
	}
	//处理段落文本
	inline void mkpara(node *v) {
		//如果当前节点的子节点数组长度为1，并且数组里的元素类型为段落
	    if (v->ch.size() == 1u && v->ch.back()->type == paragraph)
	        return;
	    //如果本身就是段落
	    if (v->type == paragraph)
	        return;
	    //如果没有类型，就让它变成段落类型
	    if (v->type == nul) {
	        v->type = paragraph;
	        return;
	    }
	    //设一个新的节点，然后把它放到当前节点的子节点数组里
	    node *x = new node(paragraph);
	    x->ch = v->ch;
	    v->ch.clear();
	    v->ch.push_back(x);
	}
	//从某个节点开始dfs(深度优先搜索)
	void dfs(node *v) {
		//如果搜到普通段落了，就该返回了
	    if (v->type == paragraph && v->elem[0].empty() && v->ch.empty())
	        return ;
	    //先在内容里加入对应的前置html文本
	    content += frontTag[v->type];
	    //设置flag，判断是否是标题，超链接或者图片
	    bool flag = true;

	    // 处理标题, 支持用目录进行跳转
	    if (isHeading(v)) {
	    	//加入对应的html文本
	        content += "id=\"" + v->elem[0] + "\">";
	        flag = false;
	    }

	    // 处理超链接
	    if (isHref(v)) {
	    	//加入对应的html文本
	        content += "<a href=\"" + v->elem[1] + "\" title=\"" + v->elem[2] + "\">" + v->elem[0] + "</a>";
	        flag = false;
	    }

	    // 处理图片
	    if (isImage(v)) {
	    	//加入对应的html文本
	        content += "<img alt=\"" + v->elem[0] + "\" src=\"" + v->elem[1] + "\" title=\"" + v->elem[2] + "\" />";
	        flag = false;
	    }

	    // 如果上面三者都不是, 则直接添加内容
	    if (flag) {
	        content += v->elem[0];
	        flag = false;
	    }

	    // 递归遍历所有
	    for (int i = 0; i < (int)v->ch.size(); i++)
	        dfs(v->ch[i]);

	    // 拼接为结束标签
	    content += backTag[v->type];
	}
	//遍历目录
	void Cdfs(Cnode *v, string index) {
		//在目录中加入内容
	    TOC += "<li>\n";
	    TOC += "<a href=\"#" + v->tag + "\">" + index + " " + v->heading + "</a>\n";
	    //加入子目录的内容
	    int n = (int)v->ch.size();
	    if (n) {
	        TOC += "<ul>\n";
	        for (int i = 0; i < n; i++) {
	        	//遍历所有
	            Cdfs(v->ch[i], index + to_string(i + 1) + ".");
	        }
	        TOC += "</ul>\n";
	    }
	    TOC += "</li>\n";
	}
public:
	//构造函数
	MarkdownTransform(const std::string &filename) {
	    // 首先为文档的树结构进行初始化，并将当前指针 not 指向根节点
	    Croot = new Cnode("");
	    root = new node(nul);
	    now = root;

	    // 从文件流中读取文件
	    std::ifstream fin(filename);

	    // 默认不是新段落、默认不在代码块内
	    bool newpara = false;
	    bool inblock = false;

	    // 直到读取到 eof 为止
	    while (!fin.eof()) {
	        // 从文件中获取一行
	        fin.getline(s, maxLength);

	        // 处理不在代码块且需要换行的情况
	        if (!inblock && isCutline(s)) {
	            now = root;
	            now->ch.push_back(new node(hr));
	            newpara = false;
	            continue;
	        }

	        // std::pair 实质上是一个结构体, 可以将两个数据组合成一个数据
	        // 计算一行中开始的空格和 Tab 数
	        std::pair<int, char *> ps = start(s);

	        // 如果没有位于代码块中, 且没有统计到空格和 Tab, 则直接读取下一行
	        if (!inblock && ps.second == nullptr) {
	            now = root;
	            newpara = true;
	            continue;
	        }

	        // 分析该行文本的类型
	        std::pair<int, char *> tj = JudgeType(ps.second);

	        // 如果是代码块类型
	        if (tj.first == blockcode) {
	            // 如果位于代码块中, 则 push 一个空类型的节点
	            inblock ? now->ch.push_back(new node(nul)) : now->ch.push_back(new node(blockcode));
	            inblock = !inblock;
	            continue;
	        }

	        // 如果在代码块中, 直接将内容拼接到当前节点中
	        if (inblock) {
	            now->ch.back()->elem[0] += string(s) + '\n';
	            continue;
	        }

	        // 如果是普通段落
	        if (tj.first == paragraph) {
	            if (now == root) {
	                now = findnode(ps.first);
	                now->ch.push_back(new node(paragraph));
	                now = now->ch.back();
	            }
	            bool flag = false;
	            if (newpara && !now->ch.empty()) {
	                node* ptr = nullptr;
	                for (auto i: now->ch) {
	                    if (i->type == nul)
	                        ptr = i;
	                }
	                if (ptr != nullptr)
	                    mkpara(ptr);
	                flag = true;
	            }
	            if (flag) {
	                now->ch.push_back(new node(paragraph));
	                now = now->ch.back();
	            }
	            now->ch.push_back(new node(nul));
	            insert(now->ch.back(), string(tj.second));
	            newpara = false;
	            continue;
	        }

	        now = findnode(ps.first);

	        // 如果是标题行, 则向其标签中插入属性 tag
	        if (tj.first >= h1 && tj.first <= h6) {
	            now->ch.push_back(new node(tj.first));
	            now->ch.back()->elem[0] = "tag" + to_string(++cntTag);
	            insert(now->ch.back(), string(tj.second));
	            Cins(Croot, tj.first - h1 + 1, string(tj.second), cntTag);
	        }

	        // 如果是无序列表
	        if (tj.first == ul) {
	            if (now->ch.empty() || now->ch.back()->type != ul) {
	                now->ch.push_back(new node(ul));
	            }
	            now = now->ch.back();
	            bool flag = false;
	            if (newpara && !now->ch.empty()) {
	                node* ptr = nullptr;
	                for (auto i: now->ch) {
	                    if (i->type == li) ptr = i;
	                }
	                if (ptr != nullptr) mkpara(ptr);
	                flag = true;
	            }
	            now->ch.push_back(new node(li));
	            now = now->ch.back();
	            if (flag) {
	                now->ch.push_back(new node(paragraph));
	                now = now->ch.back();
	            }
	            insert(now, string(tj.second));
	        }

	        // 如果是有序列表
	        if (tj.first == ol) {
	            if (now->ch.empty() || now->ch.back()->type != ol) {
	                now->ch.push_back(new node(ol));
	            }
	            now = now->ch.back();
	            bool flag = false;
	            if (newpara && !now->ch.empty()) {
	                node* ptr = nullptr;
	                for (auto i: now->ch) {
	                    if (i->type == li) ptr = i;
	                }
	                if (ptr != nullptr) mkpara(ptr);
	                flag = true;
	            }
	            now->ch.push_back(new node(li));
	            now = now->ch.back();
	            if (flag) {
	                now->ch.push_back(new node(paragraph));
	                now = now->ch.back();
	            }
	            insert(now, string(tj.second));
	        }

	        // 如果是引用
	        if (tj.first == quote) {
	            if (now->ch.empty() || now->ch.back()->type != quote) {
	                now->ch.push_back(new node(quote));
	            }
	            now = now->ch.back();
	            if (newpara || now->ch.empty()) now->ch.push_back(new node(paragraph));
	            insert(now->ch.back(), string(tj.second));
	        }

	        newpara = false;

	    }

	    // 文件读取分析完毕
	    fin.close();

	    // 深入优先遍历整个语法树
	    dfs(root);

	    // 构造目录
	    TOC += "<ul>";
	    for (int i = 0; i < (int)Croot->ch.size(); i++)
	        Cdfs(Croot->ch[i], to_string(i + 1) + ".");
	    TOC += "</ul>";
	}
	template <typename T>
	void destroy(T *v) {
	    for (int i = 0; i < (int)v->ch.size(); i++) {
	        destroy(v->ch[i]);
	    }
	    delete v;
	}
	std::string getContents(){
		return content;
	}
	std::string getTableOfContents(){
		return TOC;
	}
	~MarkdownTransform(){
		destroy<node>(root);
		destroy<Cnode>(Croot);
	}
};
#endif