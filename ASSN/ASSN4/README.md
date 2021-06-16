Assignment #4: 3D Shading
===
CSED451 Computer Graphics (Spring 2021) Assignment #4

`콤퓨타화상처리`
**최은수** (컴퓨터공학과, 20180050, ches7283),
**권민재** (컴퓨터공학과, 20190084, mzg00)


# 개요
이번 과제는 Assignment 3-2에서 제작한 GLSL 기반의 프로그램에 광원과 텍스쳐를 입히는 쉐이딩 작업을 하는 것을 목표로 한다. 이전 Assignment 3-2 과제를 확장한 형태의 과제이었기 때문에, **본 보고서에는 Assignment 3-2에서 변경 및 확장된 부분에 대해서만 자세히 다루고 있으며 기존의 과제와 동일한 부분은 작성하지 않았다.**

### Lighting
Directional light와 point light를 구현하되, 전자는 월드의 중심을 기준으로 주기적으로 운동하며, 후자는 항성계의 위성과 상대적으로 고정된 위치를 가지도록 구현해야한다.
### Shading
Gouraud shading과 Phong shading을 구현하되, 다수의 광원에 의한 diffuse와 specular reflection을 구현해야한다.

### Texture mapping
기본적으로는 diffuse 텍스쳐 매핑을 구현하되, phong shading 모드일 때에는 normal mapping을 구현하여야 한다.

# 프로그래밍 환경
### 개발 환경
- Visual Studio 2019
### 라이브러리 버전
- OpenGL 4.1
- FreeGLUT 3.2.1
- GLEW 2.2.0
- GLM 0.9.9.9
- GLSL 3.3


# 프로그램 설계 및 구현
## Shape
광원과 텍스쳐를 구현하기 전에, 우선 각 vertex의 uv 좌표와 normal vector를 구할 수 있어야 한다. 그렇기 때문에 obj 파일을 불러올 경우에는 이를 파싱해서 shape에 벡터 형태로 저장할 수 있게 하였다. 구 같은 경우, 각 vertex에서  $(\sin\phi\cos\theta, \sin\phi\sin\theta, \cos\phi)$을 normal vector로 저장하고, $(0.5 + \frac{\text{arctan2}(\sin\phi\cos\theta, \cos\phi)}{2\pi}, 0.5 - \frac{\arcsin(\sin\phi\sin\theta)}{\pi})$을 uv 좌표로 설정하여 쉐이딩에 필요한 정보를 구할 수 있었다. 이 정보들은 Attribute Array의 인덱스를 설정하여 그래픽카드에 VBO/VAO의 형태로 정보를 전달할 수 있도록 하였다.

## Lighting
광원을 관리하는 가장 기본적인 클래스인 Light를 클래스 DirectionalLight와 PointLight가 상속하는 형태로 구현하였다.

### Directional Light
class DirectionalLight는 ambient, diffuse, specular 빛의 세기와 빛의 방향 값을 저장한다. 각각의 값은 use 메소드를 통해 그래픽카드에 uniform의 형태로 전달된다. 그래픽카드에서는 전달받은 값을 가지고 빛의 방향과 법선 벡터의 내적값을 이용하여 시뮬레이션 된 빛의 세기에 따라 더 밝게 나올 수 있도록 구현하였다.

### Point Light
class Point Light는 ambient, diffuse, specular 빛의 세기와 광원의 위치, 상수항 계수, 1차항 계수, 2차항 계수를 저장한다. 각각의 값은 use 메소드를 통해 그래픽카드에 uniform의 형태로 전달된다. 그래픽카드에서는 빛의 위치와 fragment의 위치를 이용하여 빛의 방향을 구하고, 해당 방향과 법선 벡터의 내적 값을 이용하여 시뮬레이션 된 빛의 세기에 따라 밝기가 상이할 수 있도록 구현하였다. 다만, 전달받은 계수들 값을 이용하여 거리에 따른 감쇠(attenuation)가 일어날 수 있도록 구현하였다.

## Texture mapping
### Class Texture
Shape 의 constructor에서 Diffuse map과 Normal map image path를 받아 각각 instance를 생성하도록 구현. 그리고 VAO와 마찬가지로 VertexNode에 tid 를 저장한다.

__Method__
- Constructor(path) : path의 map file을 읽어서 TexImage2D 생성.


__Attribute__
- Tid : constructor에서 binding된 Texture id


### 1) Diffuse mapping
#### pass to shader
```cpp
struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};
````
ambient도 diffuse와 같은 Texture mapping을 사용할 것 이므로 diffuse, specular, shininess만 입력 받는다. Material struct를 통해 값을 전달하며 GLSL에서 제공하는 **sampler2D** type을 사용한다.
- diffuse texture 는 0번, normal map texture는 1번으로 binding한다.

#### using at Lighting calc
기존에는 lighting에서 vec3 값을 곱했다. 하지만 이번에는 GLSL의 texture 함수와 위에서 언급한 sampler2D 값, 그리고 u-v coordinate 좌표 값을 사용하여 texture map에서 해당 fragment에 대응되는 색깔을 뽑아오도록 한다.



### 2) Normal mapping
#### extract Normal vector
- 기존에는 Shape에서 만든 normal vector를 사용하였다면, normal mapping에서는 Normal map image의 튀어나오고 들어간 부분들의 색깔로 부터 normal vector를 뽑아내서 lighting에 사용한다.
- GLSL의 rgb 함수를 이용해 normal vector를 구하면 [0, 1] 범위의 값이므로 [-1, 1] range를 가지는 vector로 변환하기 위하여 작은 계산이 필요하다.

```cpp
norm = texture(normalTexture, TexCoords).rgb; 
norm = normalize(norm * 2.0 - 1.0);
```

#### Tangent Space
그러나, 이대로 구현을 하게 되면 문제점이 한가지가 생긴다. Normal map에서는 +z방향이 튀어나오는 방향이기 때문에 World-coordinate에 다양한 위치와 각도로 존재하는 fragment 들에 직접 적용할 수가 없다. 그래서 Normal map space를 World-coordinate에 맵핑해서 Normal Vector를 구해야 한다. 그래서 접선을 구해서 Tangent Space를 계산하고 TBN(Tangent, Bi-tangent, Normal) 행렬을 활용하여 Normal vector를 최종적으로 계산해야 한다.

1. vertex 리스트를 돌면서, uv 좌표와 vertex좌표를 활용하여 Tagent vector를 구한다.
   ![](https://i.imgur.com/BOVWByW.png)
- 예를 들어 Sphere의 특정 vertex에 대한 tangent space는 위의 그림속 노란 plane이며 T, B, N은 서로 직교한다.

![](https://i.imgur.com/fpXG5aV.png)
- deltaU, deltaV는 u-v 좌표값을 이용하여 계산할 수 있고, E 값은 실제 vertex (x, y, z) 값을 이용하여 계산할 수 있다. vertex list에서 순서대로 v(t), v(t+1), v(t+2) 를 이용하여 계산하였다.

2. Bi-tangent vector는 Normal vector, Tangent vector와 직교하므로 N, T를 이용하여 B도 구할 수 있다.
3. 3. 각 vector는 모두 World-coordinate에 맵핑되어야 하므로 ModelView matrix를 곱해준다.


```
vec3 T = normalize(vec3(ModelView * vec4(vTangent - dot(vTangent, vNormal)*vNormal, 0.0)));
vec3 B = normalize(vec3(ModelView * vec4(cross(N, T), 0.0)));
```    

4. TBN행렬로 만들어 Fragment Shader에 넘겨준 후 기존의 normal vector에 곱해 최종적으로 normal vector를 완성한다.




## Shading 종류
- lighting 계산을 Fragment/Vertex shader 중 어느 곳에서 하는지에 따라 다르게 구현할 수 있다.

### make 2 different shaders
Phong/Gouraud shading 용 Vertex/Fragment Shader를 각각 1개씩 프로그램 초기에 생성한다.

### Keyboard handling
```Global variable : Shader * shader;```

__s__ 키가 눌린 경우 __Global__ 변수 shader의 값을 변경한다.
VertexNode 의 display에서는 shader 변수를 이용하여 display 한다.

## Background
__Grid 활용__ : 기존에 구현한 Grid class 에서 instance를 만든 후
1) x-axis 기준으로 90도 rotate
2) 원하는 위치로 translate
   해서 뒤쪽 벽을 구성하도록 하였다.





# 프로그램 실행 방법
### 우주선 이동
- 키보드의 **상하좌우** 키를 누르면 그에 따라 우주선이 상하좌우로 이동한다.
- 우주선은 선체의 일부분도 window 밖으로 나갈 수 없으며 적이 있는 위치까지 제한없이 이동 가능하다.

### 총알 발사
- 키보드의 **space bar** 를 누르면 삼각형의 위쪽 꼭짓점을 통해 총알을 발사할 수 있다.
- 발사 가능한 총알 개수에는 제한이 없으며 이동과 동시에 총알 발사도 가능하다.

### 모드 변경
- `C` : 키보드의 `c` 키를 누르면 올패스 모드로 전환된다.
- `F` : 키보드의 `f` 키를 누르면 올패일 모드로 전환된다.
- `R` : 키보드의 `r` 키를 누르면 rendering mode가 변경된다.
- `V` : 키보드의 `v` 키를 누르면 viewing mode가 변경된다.
- `N` : 키보드의 `n` 키를 누르면 Normal mapping mode가 변경된다.
- `S` : 키보드의 `s` 키를 누르면 Shading mode가 변경된다.



# 예제
### Lighting
![](https://i.imgur.com/VzVbWL8.png)

- 월드의 중심을 기준으로 directional light를 주기적으로 회전시키도록 하였다. 이것은 각 오브젝트에 좌우로 움직이는 광택을 통해 확인할 수 있다.
- 각 위성의 중심에 point light가 위치하도록 하였다. 이는 각 위성에 가까운 지점의 행성과 항성의 광택을 통해 알 수 있다.


### Texture mapping
|Diffuse Mapping|Normal Mapping|
|:--:|:--:|
|![](https://i.imgur.com/PeuVBiM.png) |![](https://i.imgur.com/qvnDGgO.png) |
**n** 키를 눌러 Normal mapping mode로 변경할 수 있다.
- diffuse mapping : texture map에서 uv 좌표를 이용한 diffuse/ambient 맵핑으로 texture가 매끈한 상태로 가정하여 lighting 하는 것.
- normal mapping : normap map을 활용하여 texture의 굴곡, 튀어나오고 들어간 부분들에도 lighting 효과를 준 것.



### Shading
|Phong Shading|Gouraud Shading|
|:--:|:--:|
|![](https://i.imgur.com/PeuVBiM.png)|![](https://i.imgur.com/BhigvUz.png)|

**s** 키를 눌러 Shading mode를 변경할 수 있다. Normal mapping은 phong Shading에서만 작동한다.

- Phong Shading : fragment shader에서 texture와 lighting을 계산
- Gouraud Shading : vertex shader에서 texture와 lighting을 계산




# 토론
## Sphere 최상단점 normal mapping
normal mapping을 처음에 구현했을 때 구의 최상단점 부분이 빛을 받아도 어둡게 나오는 현상을 관찰하였다.
그 원인은 tangent Space에 있었는데, 우리의 구현에서 구를 traingle_strip을 이용하여 구현하다보니 최상단점이 중복되게 되었다.
그러다 보니 tangent Vector를 구할 때 영벡터가 구해지게 되고 TBN행렬이 제대로 만들어지지 않아 생기는 문제점이었다.
우리는 Sphere에서 특수하게 최상단점의 tangent space를 x-y plane으로 지정해주었고 문제를 해결할 수 있었다.



# 결론
1. Phong Shading과 Gouraud shading 을 구현하면서 vertex shader와 fragment shader의 차이와 interpolation을 정확히 이해할 수 있었다.
2. Point Light와 Directional Light를 구현하면서 이 둘은 각 fragment에서 빛의 방향을 구하는 방식과 감쇠에 있어서 차이점이 있다는 것을 알 수 있었다.
3. uv-coordinate를 사용하여 Texture mapping을 구현하는 방법을 잘 익힐 수 있었다.
4. Normal mapping 과 tangent sapce의 활용과 개념을 정확히 익힐 수 있었다.




# 개선 방향
### Shadow map
본 Assignment에서는 광원과 광원으로 생기는 lighting 효과를 구현하였는데 이는 단순히 빛과의 거리와 각도를 통해 얼마나 더 밝고 어두운가 만을 표현하고 있다.
하지만, real-world에서는 빛이 닿지 않는 곳에는 그림자가 생기는데 Shadow map 도 표현한다면 좀 더 real-world에 가까운 그래픽을 구현할 수 있을 것 같다.



# 참고문헌
- OpenGL Docs (https://www.khronos.org/opengl/)
- https://heinleinsgame.tistory.com
- https://github.com/nothings/stb/blob/master/stb_image.h
- https://gyutts.tistory.com/46
- https://gyutts.tistory.com/174