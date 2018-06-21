> 这篇只是给学校公众号写的一篇教程。为了跟编辑交稿方便就发在这儿了。

# 预备知识
- C++在C语言的基础上添加了对面向对象和泛型编程的支持。
- 如果学过Ｃ语言的话对学习C++很有帮助，但是你得知道，从Ｃ过度到C++要学的内容比C语言要学得内容还要多。
# 开始学习C++
## 第一个C++程序
学过C语言的朋友应该知道，在Ｃ语言中标准输入输出是printf()和scanf()函数。但在C++中，引入了输入输出流，cout和cin。用法如下
### cout
```
#include <iostream>
using namespace std;
int main(){
	cout << "Hello World" << endl;
	return 0;
}
```
某些时刻cout 比printf方便，比如要输出某个变量的时候，直接`cout << [变量名]`就好了。
同样，cout也支持多种不同类型的变量或常量一起输出
比如这样
```
#include <iostream>
using namespace std;
int main(){
	int a = 100;
	double b = 5.3;
	char s[] = "hahaha";
	cout << "Hello World" << " " << a << " " << b << " "<< s << endl;
	return 0;
}
```
### cin
用法跟cout类似，不过它用于对变量的输入
```
#include <iostream>
using namespace std;
int main()
{
	int a;
	char s[100];
	cout << "input:" << endl;
	cin >> a;
	cin >> s;
	cout << "output:" << endl;
	cout << a << endl;
	cout << s << endl;
	return 0;
}
```
![运行结果](https://upload-images.jianshu.io/upload_images/7123916-5e35bfed27a7d382.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

**注：每个输出语句最后面的endl表示end of line，效果等同于"\n"**

- 在这部分，`using  namespace std`的含义读者现在不必理解，就当成像include头文件一样必须做的就好，在后面学习名称空间的时候自然会理解为什么这么做。
- cin和cout是iostream里istream和ostream的对象。就目前阶段把它当成printf和scanf用就好。
- 千万要分清，cout对应的是<<运算符，而cin对应的是>>运算符。在这里，这两个运算符是被重载的，与位运算无关。
## 变量
在C++里，变量还是跟C语言很类似的。
`变量类型 变量名`或者`变量类型 变量名 = 常值`
常见的变量有:
整型变量：int, short, long int, long long int
浮点变量：double, float, long double
字符:char
## 函数
### 函数的声明
`函数类型 函数名(参数类型，参数名（多个参数用逗号隔开）);`
函数的声明和函数的定义可以一起写在main函数的上方，也可以将声明写在main函数上方，定义写在main函数下方。前者比较方便，但是后者更美观。
#### 简单例子：
```
#include <iostream>
using namespace std;
int square(int x){
	return x * x;
}
int main(){
	int a;
	cin >> a;
	cout << square(a) << endl;
}
```
![image.png](https://upload-images.jianshu.io/upload_images/7123916-5a20af86b38766ff.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 作业
1. 输入三个整型变量a, b, c然后输出它们的最大值。
2. 编写一个函数，输入参数为n，返回小于n的所有质数之和。

数据大小无限制，int的范围就好。