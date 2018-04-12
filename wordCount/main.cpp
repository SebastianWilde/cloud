#include <iostream>
#include <functional>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <queue>
#include <mutex>
#include <thread>
using namespace std;

double timeval_diff(struct timeval *a, struct timeval *b)
{
  return
    (double)(a->tv_sec + (double)a->tv_usec/1000000) -
    (double)(b->tv_sec + (double)b->tv_usec/1000000);
}

mutex io_mutex;
struct wordCount 
{
    vector<size_t> keys;
    vector<string> words;
    vector<double> frequency;
    void Print()
    {
        if (keys.empty()==false)
        {
            cout<< "keys:[";
            for (int i=0;i<(int)keys.size();i++)
            {
                cout<<keys[i]<<" ";
            }
            cout<<"]\n";
        }
    }

    void Clean()
    {
        keys.clear();
        words.clear();
        frequency.clear();
    }
    bool Find(size_t hash, int &pos)
    {   
	    pos = find(keys.begin(), keys.end(), hash) - keys.begin();
	    if( pos > keys.size()-1 )
	        return false;
        return true;
    }
    bool Insert(string word)
    {
        hash<string> hash_fn;
        size_t hash = hash_fn(word);
        if (keys.empty())
        {
            keys.push_back(hash);
            words.push_back(word);
            frequency.push_back(1);
            return true;
        }
        int pos = 0;
        //Print();
        if (Find(hash,pos))
            frequency[pos] += 1;
        else
        {
            keys.push_back(hash);
            words.push_back(word);
            frequency.push_back(1);            
        }
        
        //Print();
        return true;
    }

    void operator = (wordCount wc)
    {
        keys = wc.keys;
        words = wc.words;
        frequency = wc.frequency; 
    }
    //wordCount
    void sum (wordCount &wc1)
    {
        wordCount result;
        for (int i=0;i<(int)wc1.keys.size();i++)
        {
            int pos = 0;
            if (Find(wc1.keys[i],pos))
            {
                frequency[pos] += wc1.frequency[i];
            }
            else
            {
                result.keys.push_back(wc1.keys[i]);
                result.words.push_back(wc1.words[i]);
                result.frequency.push_back(wc1.frequency[i]);
            }
        }
        if (!result.keys.empty())
        {
            result.keys.insert(result.keys.end(),keys.begin(),keys.end());
            result.words.insert(result.words.end(),words.begin(),words.end());
            result.frequency.insert(result.frequency.end(),frequency.begin(),frequency.end());

        }
        return;

    }
    

    void Write()
    {
        ofstream file;
        file.open("resultados.txt");
        for (int i=0;i<(int)words.size();i++)
        {   
            
            string input = to_string(keys[i]) +" || " +words[i] + " --> " + to_string(frequency[i]) + "\n";
            file << input;
        }
        file.close();
    }
};

wordCount resultados [4];
queue<string>  ram [4];
bool flag [4] = {true,true,true,true};

void threadWordCount(int pk)
{
    while (flag[pk])
    {
        io_mutex.lock();	
        while(!ram[pk].empty())
        {
            string palabra = ram[pk].front();
            ram[pk].pop();
            resultados[pk].Insert(palabra);
        }
        io_mutex.unlock();	
    }

}

void threadReduce(int pk, int i)
{
    wordCount aux;
    resultados[pk].sum(resultados[pk+i]);
    resultados[pk+i].Clean();
}

int main()
{
    
    struct timeval t_ini, t_fin;
    double secs;
    string line;
    string nameFile;
    cout<<"Ingrese el nombre del archivo: ";
    cin>>nameFile;
    wordCount wc;
    int nThreads = thread::hardware_concurrency();
    int aux = 0;
    int fin;
    cout<<"Ingrese el numero de repeticiones: ";
    cin>>fin;
    vector<thread> ths(nThreads);
    wordCount result1;
    gettimeofday(&t_ini, NULL);
    while (aux!=fin)
    {
        int i = 0;
        for (i = 0;i<nThreads;i++)
            ths[i] = thread(threadWordCount,i);
        ifstream file(nameFile);
        if(file.is_open())
        {
            int index = 0;
            while(getline(file,line))
            {
                //cout<<"Palabra: "<<line<<endl;
                io_mutex.lock();	
                ram[index%4].push(line);
                io_mutex.unlock();	
                //wc.Insert(line);
            }
            flag[0] = false;
            flag[1] = false;
            flag[2] = false;
            flag[3] = false;
            file.close();
            //wc.Write();
        }
        else
            cout <<"No se puede abrir el archivo";
        

        for (i=0;i<nThreads;i++)
            ths[i].join();
        cout<<"Entra a reducir"<<endl;
        //Fase 2
        for (i = 0;i<2;i++)
            ths[i] = thread(threadReduce,i,2);
        for (i=0;i<2;i++)
            ths[i].join();

        threadReduce(0,1);
        aux+=1;
        flag[0] = true;
        flag[1] = true;
        flag[2] = true;
        flag[3] = true;
        cout<<"Segunda ronda"<<endl;
    }
    gettimeofday(&t_fin, NULL);
    resultados[0].Write();
    
    secs = timeval_diff(&t_fin, &t_ini);
    printf("%.16g milliseconds\n", secs * 1000.0);
    return 0;
}