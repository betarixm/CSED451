#include "Model.h"

Model::Model(char *path) {
    ifstream f(path);

    if(!f.is_open()){ return; }

    while(!f.eof()) {
        string s;
        f >> s;
        if(f.eof()){ break; }

        if(s == "v") {
            _vertexBuf.push_back(getVector3f(f));
        } else if (s == "vt") {
            _uvBuf.push_back(getVector2f(f));
        } else if (s == "vn") {
            _normalBuf.push_back(getVector3f(f));
        } else if (s == "f") {
            vector<unsigned int>* target[3] = {&_vertexIdx, &_uvIdx, &_normalIdx};
            for(int i = 0; i < 3; i++) {
                string strVec;
                f >> strVec;

                istringstream ss(strVec);
                string buf;

                int j = 0;
                while(getline(ss, buf, '/')) {
                    target[j++]->push_back(stoi(buf));
                }
            }
        }
    }

    for(unsigned int idx : _vertexIdx) {
        glm::vec3 vertex = _vertexBuf[idx - 1];
        for(int j = 0; j < 3; j++) {
            _max[j] = (_max[j] < vertex[j]) ? vertex[j] : _max[j];
            _min[j] = (_min[j] > vertex[j]) ? vertex[j] : _min[j];
        }
        _vertex.push_back(vertex);
    }

    for(unsigned int idx : _uvIdx) {
        glm::vec2 vertex = _uvBuf[idx - 1];
        _uv.push_back(vertex);
    }

    for(unsigned int idx : _normalIdx) {
        glm::vec3 vertex = _normalBuf[idx - 1];
        _normal.push_back(vertex);
    }
}

vector<float> Model::compat() {
    if(_compat.empty()) {
        for(int i = 0; i < _vertex.size(); i += 1) {
            _compat.insert(_compat.end(), {
                _vertex[i][0], _vertex[i][1], _vertex[i][2],
                _normal[i][0], _normal[i][1], _normal[i][2],
                _uv[i][0], _uv[i][1], 0 // uv has no third entry
            });
        }
    }

    return _compat;
}

glm::vec3 Model::max() const {
    return _max;
}

glm::vec3 Model::min() const {
    return _min;
}

vector<vector<float>> Model::getVertex()
{
    vector<vector<float>> result;

    for(int i = 0; i < _vertex.size(); i++)
    {
        result.push_back({_vertex[i].x, _vertex[i].y, _vertex[i].z});
    }

    return result;
}

vector<vector<float>> Model::getUV()
{
    vector<vector<float>> result;

    for(int i = 0; i < _uv.size(); i++)
    {
        result.push_back({_uv[i].x, _uv[i].y, 0.0});
    }

    return result;
}