#include<iostream>
#include<string>
#include<queue>
#include<unordered_map>
using namespace std;

struct node{
    char ch;
    int freq;
    node *left,*right;
};

node *getnode(char ch,int freq,node *left,node *right){
    node *p =new node();
    p->ch=ch;
    p->freq=freq;
    p->left=left;
    p->right=right;

    return p;
}
struct comp{
    bool operator()(node *l,node *r){
        return l->freq>r->freq;
    }
};

void encode(node *root,string str,unordered_map<char,string>&huffmanCode){
    if(root==nullptr){
        return;
    }
    if(!root->left && !root->right){
        huffmanCode[root->ch]=str;
    }
    encode(root->left,str +"0",huffmanCode);
    encode(root->right,str+"1",huffmanCode);
}

void decode(node *root,int &top_index,string str){
    if(root==nullptr){
        return;
    }
    if(!root->left && !root->right){
        cout<<root->ch;
        return;
    }
    top_index++;
    if(str[top_index]=='0'){
        decode(root->left,top_index,str);
    }
    else{
        decode(root->right,top_index,str);
    }

}

void buildhuffmantree(string text){
    
    unordered_map<char,int>freq;
    for(char ch:text){
        freq[ch]++;
    }
    
    priority_queue<node*,vector<node*>,comp>pq;
   
    for(auto pair:freq){
        pq.push(getnode(pair.first,pair.second,nullptr,nullptr));
    }
    
    while(pq.size()!=1){
        
        node *left=pq.top(); pq.pop();
        node *right =pq.top(); pq.pop();

        
        int sum=left->freq+right->freq;
        pq.push(getnode('\0',sum,left,right));
    }
    
    node *root=pq.top();
   
    unordered_map<char,string>huffmanCode;
    encode(root,"",huffmanCode);

    cout<<"Huffman Codes are :\n"<<'\n';
    for(auto pair:huffmanCode){
        cout<<pair.first<<" "<<pair.second<<'\n';
    }
    cout<<"\n Original string was:\n"<<text<<'\n';
  
    string str="";
    for(char ch:text){
        str+=huffmanCode[ch];
    }
    cout<<"\n encoded string is:\n"<<str<<'\n';
    
    int top_index=-1;
    cout<<"\n decoded string is:\n";
    while(top_index<(int)str.size()-2){
        decode(root,top_index,str);
    }




}

int main(){
    string text="My name is Astika Sinha";
    buildhuffmantree(text);
}