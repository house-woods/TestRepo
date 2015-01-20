#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    ifstream in(argv[1], ios::binary);
    int begin,end,token_size,size;
    if(!in.good())
    {
        cout<<"Err:open file failed."<<endl;
    } 
    begin = in.tellg();
    in.seekg(0, ios::end);
    end = in.tellg();
    in.seekg(0, ios::beg);
    size = end - begin -2;
    if(size < 0)
    {
        cout<<"Err:no data in the file."<<endl;
        in.close();
        return -1;
    }
    cout<<"The file body size:"<<size<<endl;
    token_size = in.get();
    cout<<"1st byte is:"<<token_size<<endl;
    int temp = in.get();
    cout<<"2nd byte is:"<<temp<<endl;
    token_size += (temp*256);
    if(token_size > size)
    {
        cout<<"Err:token is too large."<<endl;
        in.close();
        return -1;
    }
    cout<<"The token size:"<<token_size<<endl;
    int data_size = size - token_size;
    cout<<"The data size:"<<data_size<<endl;
    char* token = new char[token_size + 1];
    char* data = new char[data_size + 1];
    token[token_size] = 0;
    data[data_size] = 0;
    in.read(data, data_size);
    in.read(token, token_size);
    in.close();
    cout<<"Token:"<<endl;
    for(int i = 0;i < token_size;i++)
    {
        cout<<token[i];
    }
    cout<<endl;
    cout<<"Data:"<<endl;
    for(int j = 0;j < data_size;j++)
    {
        cout<<data[j];
    }
    cout<<endl;
    cout<<"--------------------------------------------------------"<<endl;
    int idx = 0;
    int len = data[idx + 3] & 0xff;
    len <<= 8;
    len += data[idx + 2] & 0xff;
    len <<= 8;
    len += data[idx + 1] & 0xff;
    len <<= 8;
    len += data[idx + 0] & 0xff;
    idx += 4;
    cout<<"version len:"<<len<<endl;
    char* strTmp = new char[len + 1];
    strTmp[len] = '\0';
    for(int ii = 0; ii < len; ii++)
    {
        strTmp[ii] = data[idx++];
    }
    string version = strTmp;
    cout<<"version is:"<<version<<endl;
    

    cout<<"------------clear resource.-----------------"<<endl;
    delete token;
    delete data;
    delete strTmp;
    return 0;

}



