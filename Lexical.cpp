#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstring>
#include<string>
#include<cstdlib>

using namespace std;

string reserved_word[20];//存储保留字 
char buffer;//每次从文件中读取的字符 
int num=0;//每个单词中当前字符的位置
int line=1; //行数
int row=1; //列数，就是每行的第几个
bool flag; //判断文件是否读取完成
int type;//单词的类型
string word="";//存储当前读取的单词的内容 
int pos;// fseek的时候用来接着的

//设置保留字
void set_reserve()
{
    reserved_word[1]="if";
    reserved_word[2]="then";
    reserved_word[3]="else";
    reserved_word[4]="end";
    reserved_word[5]="repeat";
    reserved_word[6]="until";
    reserved_word[7]="read";
    reserved_word[8]="write";
    reserved_word[9]="write";
    //reserved_word[10]="and";
    //reserved_word[11]="or";
} 

//判断字符是否为字母 
bool letterJudge(char x)
{
    if(x>='a' && x<='z' || x>='A' && x<='Z' ){
        return true;
    }
    else{
		return false;
	}
}
 
//判断字符是否为数字 
bool numberJudge(char x)
{
    if(x>='0' && x<='9'){
        return true;
    }
    else{
    	return false;
	} 
}

//判断字符是否为界符
bool delimiterJudge(char x)
{
    if(x=='('||x==')'||x==','||x==';'){
        return true;
    }
    else{
		return false;
	}
}

//判断字符是否为加减乘
bool operator1Judge(char x)
{
    if(x=='+'||x=='-'||x=='*'){
        return true;
    }
    else{
		return false;
	}
}

//判断字符是否为赋值，大于或小于
bool operator2Judge(char x)
{
    if(x=='='|| x=='>'||x=='<'){
        return true;
    }
    else{
    	return false;
	}
}

//从文件中读取程序 
int GetToken(FILE *fp)
{
    buffer=fgetc(fp);    //从文件中读取一个字符 
    if(feof(fp)){       //文件读取完毕 
        flag=0;
		return 0;
    }
    //cout<<buffer;
    else if(buffer==' ')  //读取为空字符，开始读取下一个字符 
    {
        row++;
        return 0;
    }
    else if(buffer=='\n')   //本行读取完毕 
    {
        line++;
        row=1;
        return 0;
    }
 
 
    //如果是字母或'_' 判断是哪种类型（关键字或标识符） 
    else if(letterJudge(buffer) || buffer=='_')
    {
        word+=buffer;
		row++;
        while((buffer=fgetc(fp)) && (letterJudge(buffer) || numberJudge(buffer) || buffer=='_'))
        {
            word+=buffer;
			row++;
        }
        if(feof(fp)){
            flag=0;
			return 1;
        }
        
        //将一个单词转化为小写，用来判断是否为关键字 
        string tempword=word;
        for(int i=0;i<tempword.length();i++)
        {
            if(tempword[i]>='A' && tempword[i]<='Z')
            {
                tempword[i]+=32;   //转化为小写字母 
            }
        }
        for(int i=1;i<=9;i++){     //关键字的长度 
            if(tempword==reserved_word[i]){   //如果为关键字 
                pos=fseek(fp,-1,SEEK_CUR);
                return 3;
            }
        }
        pos=fseek(fp,-1,SEEK_CUR);
        return 1;
    }
 
    //如果是加减乘，返回类型4
    else if(operator1Judge(buffer))
    {
        word+=buffer;
		row++;
        return 4;
    }
 
    //如果是数字，返回类型2 
    else if(numberJudge(buffer))
    {
        word+=buffer;
		row++;
        while((buffer=fgetc(fp)) && (numberJudge(buffer) || buffer=='.'))
        {
            word+=buffer;
			row++;
        }
        if(feof(fp)){
            flag=0;
			return 2;
        }
        pos=fseek(fp,-1,SEEK_CUR);
        return 2;
    }
 
    //如果是界符，返回类型6 
    else if(delimiterJudge(buffer))
    {
        word+=buffer;
		row++;
        return 6;
    }
 
    //检验 <=、  >=、  <>、  == =、 <、>
    else if(operator2Judge(buffer))
    {
    	word+=buffer;
        row++;
        //检验 <>或<= 
        if(buffer=='<'){
            buffer=fgetc(fp);
            if(buffer=='>' || buffer=='=')
            {
                word+=buffer;
                row++;
                return 5;
            }
        }
        //检验 >=或 ==
        else{
            buffer=fgetc(fp);
            if(buffer=='=')
            {
                word+=buffer;
                row++;
                return 5;
            }
        }
        if(feof(fp)){
            flag=0;
        }
        pos=fseek(fp,-1,SEEK_CUR);
        return 4;
    }
 
    //如果是/ ，为除号或注释
    else if(buffer=='/')
    {
    	word+=buffer;
        row++;
        buffer=fgetc(fp);
        //除号
        if(buffer!='*' && buffer !='/')
        {
            pos=fseek(fp,-1,SEEK_CUR);
            return 4;
        }
        // 这一行剩下的全被注释了
        if(buffer=='/')
        {
            word.clear();
            while((buffer=fgetc(fp)) && buffer!='\n' &&!feof(fp)) //死循环并读取所有注释字符 
            {
            }
            if(feof(fp)){
                flag=0;
				return 0;
            }
            else{
               pos=fseek(fp,-1,SEEK_CUR);
            }
            //开始读取下一行 
            line++;
			row=1;
            return 0;
        }
        
        if(buffer=='*')
        {
            bool endflag=1;  //注释结束的标志 
            while(endflag)
            {
                word.clear();
                buffer=fgetc(fp);
                row++;
                if(buffer=='\n'){   //开始读取下一行 
					line++;
					row=1;
				}
                if(buffer!='*')
					continue;
                else{
                    buffer=fgetc(fp);
                    row++;
					if(buffer=='\n'){
						line++;
						row=1;
					}
                    if(buffer=='/'){
                        endflag=0;
                    }
                    else continue;
                }
                if(feof(fp)){
					flag=0;
					return 0;
				}
            }
 
        }
 
    }
    
    else if(buffer=='{')
    {
    	word+=buffer;
        row++;
        while((buffer=fgetc(fp)) && buffer!='}') //死循环并读取所有注释字符 
        {
        	word+=buffer;
        	row++;
        }
        word+=buffer;
        row++;
        if(feof(fp)){
            flag=0;
			return 0;
        }
        pos=fseek(fp,-1,SEEK_CUR);
        //开始读取下一行 
        //line++;
		//row=1;
    	return 7;
    }
 
    else {
        word+=buffer;
        row++;
        return -1;
    }
}

int main()
{
	int lineflag;
    set_reserve();
    cout<<"开始分析 "<<"code.txt"<<endl;   
    system("pause");
 
    FILE *fp;
    flag=1;
    //if(!(fp=fopen("test.txt","r")))
    if(!(fp=fopen("test.txt","r")))
    {
        cout<<"未找到文件"<<endl;
        flag=0;
    }
    
 	lineflag=line;
 
    while(flag==1)  //文件未读完一直循环 
    {
    	if(lineflag!=line){
    		cout<<endl;
    		lineflag=line;
		}
        //判断返回的类型 
        type=GetToken(fp);//调用函数提取单词 
 
        if(type==1)   //标识符 
        {
        	cout<<line<<": ID, name= "<<word<<endl;
            //cout<<"type:1 identifier      "<<"line "<<line<<" row "<<row-word.length()<<"  "<<word<<endl;
            //if(word.length()>20)
            //cout<<"ERROR Identifier length cannot exceed 20 characters"<<endl;
            word.clear();
        }
        else if(type==2)   //数字 
        {
        	cout<<line<<": NUM, value= "<<word<<endl; 
            //cout<<"type:2 positive number "<<"line "<<line<<" row "<<row-word.length()<<"  "<<word<<endl;
            //if(word[0]=='0')
            //cout<<"ERROR: The first digit cannot be 0!"<<endl;
            word.clear();
        }
        else if(type==3)   //保留字 
        {
        	cout<<line<<": reserved word: "<<word<<endl;
            //cout<<"type:3 reserved word   "<<"line "<<line<<" row "<<row-word.length()<<"  "<<word<<endl;
            word.clear();
        }
        else if(type==4) //特殊符号，=，<等 
        {
        	cout<<line<<": "<<word<<endl; 
            //cout<<"type:4 unary_operator  "<<"line "<<line<<" row "<<row-1<<"  "<<word<<endl;
            word.clear();
        }
        else if(type==5)  // 两个运算符 <= 等 
        {
            cout<<"type:6 double_operator "<<"line "<<line<<" row "<<row-2<<"  "<<word<<endl;
            word.clear();
        }
        else if(type==6) //界符 
        {
        	cout<<line<<": "<<word<<endl;
            //cout<<"type:6 Separator       "<<"line "<<line<<" row "<<row-1<<"  "<<word<<endl;
            word.clear();
        }
        else if(type==7)
        {
        	cout<<line<<": "<<word<<endl;
        	word.clear();
		}
        //非法字符
        else if(type==-1)
        {
        	//cout<<line<<": "<<word<<endl;
           //cout<<"Illegal character      "<<"line "<<line<<" row "<<row-1<<"  "<<word<<endl;
           word.clear();
        }
    }
 
        int close=fclose(fp);
    	system("pause");
    return 0;
}
