#include <fstream>
#include "../h/configLoader.h"

configLoader::configLoader(string FileName, int _size)
{
    size = _size;
    vec.resize(size);

    fstream fin;
    fin.open(FileName, ios::binary | ios::in);

    if (!fin) 
    {
        for (int y = 0; y < size; y++)
        {
            vec[y] = 0;
        }
        return;
    }
    active = true;

    for (int y = 0; y < size; y++)
    {
        fin >> vec[y];
    }
    fin.close();
}

int configLoader::getConfig(int _idx)
{
    if (_idx < size)
    {
        return vec[_idx];
    }
    return 0;
}

vector<int> configLoader::GetCanSetVec(string filepath, int max)
{
    configLoader cl_switch(filepath, max);
    vector<int> rv;
    rv.clear();

    for (int i = 0; i < max; i++)
    {
        if (cl_switch.getConfig(i))
        {
            rv.push_back(i);
        }
    }
    return rv;
}