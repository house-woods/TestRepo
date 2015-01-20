#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <time.h>

using namespace std;

int readFile(string file,char** data,char** token)
{
    ifstream in(file.c_str(), ios::binary);
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
    int temp = in.get();
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
    *token = new char[token_size + 1];
    *data = new char[data_size + 1];
    (*token)[token_size] = 0;
    (*data)[data_size] = 0;
    in.read(*data, data_size);
    in.read(*token, token_size);
    in.close();
    return 0;
}
int printMap(map<string,bool>& licMap)
{
    for(map<string,bool>::iterator iter = licMap.begin(); iter != licMap.end(); iter++)
    {
        cout<<"Feature:"<<iter->first<<"("<<iter->second<<")"<<endl;
    }
    return 0;
}
int main(int argc, char* argv[])
{
    map<string,bool> _licenseMap;
    char* data = NULL;
    char* token = NULL;
    readFile(argv[1], &data, &token);
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
    
    unsigned long d = 0;
    for(int j = 7; j >= 0; --j)   
    {
        d += data[idx + j] & 0xff;
        if(j != 0)
            d <<= 8;
    }
    idx += 8;
    time_t t = (time_t)d;
    cout<<"Time:"<<t<<endl;
    time_t t1 = time(NULL);
    cout<<"Current time:"<<t1<<endl;

    int feature_array_n = data[idx + 3] & 0xff;
    feature_array_n <<= 8;
    feature_array_n += data[idx + 2] & 0xff;
    feature_array_n <<= 8;
    feature_array_n += data[idx + 1] & 0xff;
    feature_array_n <<= 8;
    feature_array_n += data[idx + 0] & 0xff;
    idx += 4;
    int rounded_bit_slots = (feature_array_n % 8 == 0) ? feature_array_n / 8 : feature_array_n / 8 + 1;
    cout<<"The feature number is:"<<feature_array_n<<endl;
    
    int fLen = 0;
    char* strtmp = NULL;
    vector<string> features;
    for(int kk = 0; kk < feature_array_n; ++kk)
    {
        fLen = data[idx + 3] & 0xff;
        fLen <<= 8;
        fLen += data[idx + 2] & 0xff;
        fLen <<= 8;
        fLen += data[idx + 1] & 0xff;
        fLen <<= 8;
        fLen += data[idx + 0] & 0xff;
        idx += 4;
        
        strtmp = new char[fLen + 1];
        strtmp[fLen] = '\0';
        for(int ll = 0; ll < fLen; ll++)
        {
            strtmp[ll] = data[idx++];
        }
        string name = strtmp;
        delete strtmp;
        map<string,bool>::iterator iter = _licenseMap.find(name);
        if(iter == _licenseMap.end())
            _licenseMap.insert(map<string,bool>::value_type(name, false));
        features.push_back(name);
    }
    for(int mm = 0; mm < rounded_bit_slots; mm++)
    {
        unsigned char next = data[idx++];
        for(int j = 0; j < 8; j++)
        {
            int index = 8 * mm + j;
            if(index < feature_array_n)
            {
                map<string,bool>::iterator i = _licenseMap.find(features[index]);
                if(i != _licenseMap.end())
                {
                    if(i->second == false)
                        i->second = (next & (0x01 << j)) > 0;
                }
            }
        }
    }
    printMap(_licenseMap);
    cout<<"------------clear resource.-----------------"<<endl;
    delete token;
    delete data;
    delete strTmp;
    return 0;

}



