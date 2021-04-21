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

void Model::display() {
    this->display(_mode);
}

void Model::display(GLenum mode) {
    glBegin(mode);
    for(auto & v : _vertex) {
        glVertex3f(v.x, v.y, v.z);
    }
    glEnd();
}
