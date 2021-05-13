#include <string>
#include "helper.h"

glm::vec3 getVector3f(ifstream& f) {
    string a, b, c;
    f >> a >> b >> c;
    glm::vec3 result(stof(a), stof(b), stof(c));
    return result;
}

glm::vec2 getVector2f(ifstream& f) {
    string a, b;
    f >> a >> b;
    glm::vec2 result(stof(a), stof(b));
    return result;
}

vector<float> vectorCompat(const vector<vector<float>>& vertices) {
    vector<float> compat;

    for(auto & ver : vertices) {
        for(float v : ver) {
            compat.push_back(v);
        }
    }

    return compat;
}

vector<float> vectorCompat(std::vector<std::vector<float>> &vertex, std::vector<std::vector<float>> &normal,
        std::vector<std::vector<float>> &uv) {
    vector<float> compat;

    for(int i = 0; i < vertex.size(); i++) {
        compat.insert(compat.end(), {
            vertex[i][0], vertex[i][1], vertex[i][2],
            normal[i][0], normal[i][1], normal[i][2],
            uv[i][0], uv[i][1], 0 // uv has no third entry
        });
    }

    return compat;
}