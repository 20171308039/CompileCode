#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstring>
#include<string>
#include<cstdlib>

using namespace std;

string reserved_word[20];//�洢������ 
char buffer;//ÿ�δ��ļ��ж�ȡ���ַ� 
int num=0;//ÿ�������е�ǰ�ַ���λ��
int line=1; //����
int row=1; //����������ÿ�еĵڼ���
bool flag; //�ж��ļ��Ƿ��ȡ���
int type;//���ʵ�����
string word="";//�洢��ǰ��ȡ�ĵ��ʵ����� 
int pos;// fseek��ʱ���������ŵ�

//���ñ�����
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

//�ж��ַ��Ƿ�Ϊ��ĸ 
bool letterJudge(char x)
{
    if(x>='a' && x<='z' || x>='A' && x<='Z' ){
        return true;
    }
    else{
		return false;
	}
}
 
//�ж��ַ��Ƿ�Ϊ���� 
bool numberJudge(char x)
{
    if(x>='0' && x<='9'){
        return true;
    }
    else{
    	return false;
	} 
}

//�ж��ַ��Ƿ�Ϊ���
bool delimiterJudge(char x)
{
    if(x=='('||x==')'||x==','||x==';'){
        return true;
    }
    else{
		return false;
	}
}

//�ж��ַ��Ƿ�Ϊ�Ӽ���
bool operator1Judge(char x)
{
    if(x=='+'||x=='-'||x=='*'){
        return true;
    }
    else{
		return false;
	}
}

//�ж��ַ��Ƿ�Ϊ��ֵ�����ڻ�С��
bool operator2Judge(char x)
{
    if(x=='='|| x=='>'||x=='<'){
        return true;
    }
    else{
    	return false;
	}
}

//���ļ��ж�ȡ���� 
int GetToken(FILE *fp)
{
    buffer=fgetc(fp);    //���ļ��ж�ȡһ���ַ� 
    if(feof(fp)){       //�ļ���ȡ��� 
        flag=0;
		return 0;
    }
    //cout<<buffer;
    else if(buffer==' ')  //��ȡΪ���ַ�����ʼ��ȡ��һ���ַ� 
    {
        row++;
        return 0;
    }
    else if(buffer=='\n')   //���ж�ȡ��� 
    {
        line++;
        row=1;
        return 0;
    }
 
 
    //�������ĸ��'_' �ж����������ͣ��ؼ��ֻ��ʶ���� 
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
        
        //��һ������ת��ΪСд�������ж��Ƿ�Ϊ�ؼ��� 
        string tempword=word;
        for(int i=0;i<tempword.length();i++)
        {
            if(tempword[i]>='A' && tempword[i]<='Z')
            {
                tempword[i]+=32;   //ת��ΪСд��ĸ 
            }
        }
        for(int i=1;i<=9;i++){     //�ؼ��ֵĳ��� 
            if(tempword==reserved_word[i]){   //���Ϊ�ؼ��� 
                pos=fseek(fp,-1,SEEK_CUR);
                return 3;
            }
        }
        pos=fseek(fp,-1,SEEK_CUR);
        return 1;
    }
 
    //����ǼӼ��ˣ���������4
    else if(operator1Judge(buffer))
    {
        word+=buffer;
		row++;
        return 4;
    }
 
    //��������֣���������2 
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
 
    //����ǽ������������6 
    else if(delimiterJudge(buffer))
    {
        word+=buffer;
		row++;
        return 6;
    }
 
    //���� <=��  >=��  <>��  == =�� <��>
    else if(operator2Judge(buffer))
    {
    	word+=buffer;
        row++;
        //���� <>��<= 
        if(buffer=='<'){
            buffer=fgetc(fp);
            if(buffer=='>' || buffer=='=')
            {
                word+=buffer;
                row++;
                return 5;
            }
        }
        //���� >=�� ==
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
 
    //�����/ ��Ϊ���Ż�ע��
    else if(buffer=='/')
    {
    	word+=buffer;
        row++;
        buffer=fgetc(fp);
        //����
        if(buffer!='*' && buffer !='/')
        {
            pos=fseek(fp,-1,SEEK_CUR);
            return 4;
        }
        // ��һ��ʣ�µ�ȫ��ע����
        if(buffer=='/')
        {
            word.clear();
            while((buffer=fgetc(fp)) && buffer!='\n' &&!feof(fp)) //��ѭ������ȡ����ע���ַ� 
            {
            }
            if(feof(fp)){
                flag=0;
				return 0;
            }
            else{
               pos=fseek(fp,-1,SEEK_CUR);
            }
            //��ʼ��ȡ��һ�� 
            line++;
			row=1;
            return 0;
        }
        
        if(buffer=='*')
        {
            bool endflag=1;  //ע�ͽ����ı�־ 
            while(endflag)
            {
                word.clear();
                buffer=fgetc(fp);
                row++;
                if(buffer=='\n'){   //��ʼ��ȡ��һ�� 
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
        while((buffer=fgetc(fp)) && buffer!='}') //��ѭ������ȡ����ע���ַ� 
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
        //��ʼ��ȡ��һ�� 
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
    cout<<"��ʼ���� "<<"code.txt"<<endl;   
    system("pause");
 
    FILE *fp;
    flag=1;
    //if(!(fp=fopen("test.txt","r")))
    if(!(fp=fopen("test.txt","r")))
    {
        cout<<"δ�ҵ��ļ�"<<endl;
        flag=0;
    }
    
 	lineflag=line;
 
    while(flag==1)  //�ļ�δ����һֱѭ�� 
    {
    	if(lineflag!=line){
    		cout<<endl;
    		lineflag=line;
		}
        //�жϷ��ص����� 
        type=GetToken(fp);//���ú�����ȡ���� 
 
        if(type==1)   //��ʶ�� 
        {
        	cout<<line<<": ID, name= "<<word<<endl;
            //cout<<"type:1 identifier      "<<"line "<<line<<" row "<<row-word.length()<<"  "<<word<<endl;
            //if(word.length()>20)
            //cout<<"ERROR Identifier length cannot exceed 20 characters"<<endl;
            word.clear();
        }
        else if(type==2)   //���� 
        {
        	cout<<line<<": NUM, value= "<<word<<endl; 
            //cout<<"type:2 positive number "<<"line "<<line<<" row "<<row-word.length()<<"  "<<word<<endl;
            //if(word[0]=='0')
            //cout<<"ERROR: The first digit cannot be 0!"<<endl;
            word.clear();
        }
        else if(type==3)   //������ 
        {
        	cout<<line<<": reserved word: "<<word<<endl;
            //cout<<"type:3 reserved word   "<<"line "<<line<<" row "<<row-word.length()<<"  "<<word<<endl;
            word.clear();
        }
        else if(type==4) //������ţ�=��<�� 
        {
        	cout<<line<<": "<<word<<endl; 
            //cout<<"type:4 unary_operator  "<<"line "<<line<<" row "<<row-1<<"  "<<word<<endl;
            word.clear();
        }
        else if(type==5)  // ��������� <= �� 
        {
            cout<<"type:6 double_operator "<<"line "<<line<<" row "<<row-2<<"  "<<word<<endl;
            word.clear();
        }
        else if(type==6) //��� 
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
        //�Ƿ��ַ�
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
