# Digital Image Processing

C++/OpenCV를 활용한 디지털 영상처리 구현 프로젝트

## 📌 개요

다양한 디지털 영상처리 기법들을 C++과 OpenCV를 사용하여 직접 구현한 프로젝트입니다. 기본적인 필터링부터 고급 영상처리 기법까지 폭넓게 다루고 있습니다.

## 🛠️ 기술스택

- **Language**: C++
- **Library**: OpenCV
- **Topics**: Image Filtering, Edge Detection, Feature Detection, Image Transformation, Segmentation, Color Processing, HDR Imaging

## ✨ 내용

### 📸 이미지 필터링링

#### **Gaussian Filter** 
> 이미지를 부드럽게 만들어주는 블러링 필터

- 9x9 크기의 가우시안 필터를 직접 구현
- 가우시안 분포를 따르는 마스크를 이용한 Convolution 연산
- 3x3 필터보다 더 강한 블러링 효과 확인

<img width="347" height="417" alt="image" src="https://github.com/user-attachments/assets/25936740-52c9-46bc-8721-cbb6e333c570" />

<details>
<summary>💡 Implementation Details</summary>

정규화를 위해 마스크값의 합으로 나누어주는 과정을 추가했으며, 히스토그램 분석을 통해 불연속적이던 밝기값이 연속적으로 변화하는 것을 확인했습니다.
</details>

#### **Median Filter**
<img width="237" height="145" alt="image" src="https://github.com/user-attachments/assets/d1516385-90b9-4d6d-bb96-a3dcdd257b4c" />


> Salt & Pepper 노이즈 제거에 특화된 필터

- 3x3, 5x5 크기의 미디언 필터 구현
- 주변 픽셀값들을 정렬하여 중간값으로 대체
- **Double Median Filter**로 강력한 노이즈 제거 달성
<img width="358" height="457" alt="image" src="https://github.com/user-attachments/assets/5f22bfd2-4394-4444-96fd-aee2429fcbab" />


<details>
<summary>💡 Key Findings</summary>

가우시안 필터와 달리 충격성 노이즈 제거에 탁월했으며, 두 번 적용하는 방식으로 잔여 노이즈까지 완벽히 제거할 수 있었습니다.
</details>

#### **Bilateral Filter**
<img width="427" height="176" alt="image" src="https://github.com/user-attachments/assets/83a0e541-8846-4d32-b710-0c6a790f1e46" />

> 엣지를 보존하면서 노이즈를 제거하는 스마트한 필터

- 공간 도메인과 색상 도메인을 동시에 고려
- 가우시안 필터의 경계선 무너짐 문제 해결
- gr(range filter)과 gs(spatial filter)의 가중치 조합

<img width="446" height="168" alt="image" src="https://github.com/user-attachments/assets/11f14d08-8b76-4909-9eb8-486abff17d66" />

### 🔍 Edge Detection

#### **Sobel Filter**
> 특정 방향의 엣지를 검출하는 필터

- 45도, 135도 대각선 엣지 검출 구현
- 그래디언트 기반 엣지 검출
- 두 마스크의 절댓값 합으로 최종 엣지 결정
<img width="453" height="175" alt="image" src="https://github.com/user-attachments/assets/e718c9bf-c411-4cf8-b014-18411f0995b4" />


#### **Canny Edge Detection**
> 가장 널리 사용되는 엣지 검출 알고리즘

- Double threshold와 Hysteresis를 통한 정확한 엣지 검출
- Low threshold: 0 ~ 150, High threshold: 50 ~ 300 범위 실험
- 임계값 조정으로 원하는 수준의 엣지만 추출 가능

<img width="458" height="131" alt="image" src="https://github.com/user-attachments/assets/0aa73af4-0995-43e8-b967-82fdb5f4f5ac" />


### 🎯 Feature Detection

#### **Smart Coin Counter**
> Blob Detection을 활용한 동전 개수 자동 계산

```cpp
// 핵심 파라미터 조정
params.minCircularity = 0.3;     // 원형 판단 기준 완화
params.minArea = 100;             // 작은 노이즈 제거
params.maxArea = 8000;            // 500원 동전까지 검출
```
<img width="386" height="190" alt="image" src="https://github.com/user-attachments/assets/0c9e94a2-fdca-46a0-b44c-de95bc3a3255" />
<br><img width="389" height="181" alt="image" src="https://github.com/user-attachments/assets/c7295019-7e4b-4e88-b7aa-4e68ec13d3b0" />

초기 5개 → 최종 14개 검출 성공

#### **Shape Classifier**
> Harris Corner + Blob Detection 조합

- 도형의 꼭짓점 자동 검출
- 삼각형(3), 사각형(4), 오각형(5) 등 정확한 분류
- 중복 검출 문제를 threshold 조정으로 해결

<img width="367" height="239" alt="image" src="https://github.com/user-attachments/assets/785cc75f-9e9c-47d6-9913-69cef5116d72" />
<br><img width="382" height="224" alt="image" src="https://github.com/user-attachments/assets/9a75622f-bf4a-43a3-9135-3a3b0d76d6ff" />


#### **SIFT Features**
> 크기와 회전에 강인한 특징점 검출

- Scale & Rotation invariant ✅
- Brightness change sensitive ⚠️
- Perspective transform vulnerable ⚠️
<img width="436" height="146" alt="image" src="https://github.com/user-attachments/assets/f0595971-1a05-4c13-af63-9e6dc118e014" />
<br><img width="451" height="161" alt="image" src="https://github.com/user-attachments/assets/e54bf1d2-91b1-4d5b-91e3-27a19ed79c81" />


### 🔄 Image Transformation

#### **Custom Rotation Matrix**
> OpenCV 함수 없이 회전 변환 구현

```cpp
// 회전 변환 행렬 직접 구현
α = scale × cos(angle)
β = scale × sin(angle)
```

- Scaling 기능 포함
- 중심점 기준 회전
- OpenCV 결과와 100% 일치

<img width="446" height="599" alt="image" src="https://github.com/user-attachments/assets/22479557-6916-4a01-9793-c510f48b3a7e" />


#### **Auto Card Rectification**
> 기울어진 카드를 자동으로 정면 변환

1. Harris Corner로 카드 모서리 검출
2. Threshold로 배경 제거
3. Perspective Transform으로 정면 변환

<img width="350" height="250" alt="image" src="https://github.com/user-attachments/assets/6fe9e260-ce7b-4de7-8474-021f208d291e" />

<br><img width="386" height="206" alt="image" src="https://github.com/user-attachments/assets/3deed767-d24f-46b2-8676-531de637d455" />


### 🎨 Segmentation & Clustering

#### **K-means Clustering**
> 색상 기반 영상 분할

| K값 | 결과 |
|-----|------|
| k=2 | 흑백 이미지 수준 |
| k=5 | 기본적인 색상 구분 |
| k=10+ | 구름과 석양 구분 가능 |

#### **Mean Shift**
> 밀도 기반 클러스터링

- **Spatial bandwidth**: Region of Interest 크기 결정
- **Color bandwidth**: 색상 유사도 범위 결정
- Bandwidth 증가 → 더 부드러운 결과

<img width="409" height="488" alt="image" src="https://github.com/user-attachments/assets/7941ddee-204d-431b-856b-65c62049d3ba" />


#### **GrabCut**
> 스마트한 전경/배경 분리

**잘 되는 경우**: 색상 대비가 뚜렷한 이미지  
**한계**: 전경/배경 색상이 유사한 경우

<img width="435" height="208" alt="image" src="https://github.com/user-attachments/assets/e59318d9-e407-4383-a831-21040b12ba6e" />
<br><img width="473" height="172" alt="image" src="https://github.com/user-attachments/assets/c6a7523d-5a20-45d8-8b3c-3c1230e9d75e" />
<br><img width="467" height="202" alt="image" src="https://github.com/user-attachments/assets/c4975ca3-8330-47de-935a-75e72b7aef91" />


### 🌈 Color Processing

#### **RGB to HSV Converter**
<img width="334" height="152" alt="image" src="https://github.com/user-attachments/assets/28c16747-932d-4afa-bc44-75b05d221b2f" />

> 색상 공간 변환 및 특정 색상 검출

- RGB → HSV 변환 공식 직접 구현
- inRange를 통한 색상 마스킹
- 과일 색상 자동 분류 시스템 구현

<img width="388" height="201" alt="image" src="https://github.com/user-attachments/assets/10829622-3374-4ce5-b8f3-e41e799bf578" />
<br><img width="449" height="153" alt="image" src="https://github.com/user-attachments/assets/21fd0c02-ab29-4764-8c24-558efe08a1de" />
<br><img width="455" height="170" alt="image" src="https://github.com/user-attachments/assets/1bb8b3ff-6a5d-44f6-914a-8cb327e14ba4" />


### 🌟 Advanced Techniques

#### **Gaussian Pyramid**
> 다중 해상도 영상 생성

- 4단계 피라미드 구축
- 각 단계별 1/2 다운샘플링 + 가우시안 필터링
- 컬러 영상 지원

#### **HDR Imaging**
> 다중 노출 영상 합성으로 다이나믹 레인지 확장

**실험 조건**:
- 셔터스피드: 1/30s, 0.25s, 2.5s, 15s
- Tone Mapping: Drago, Reinhard, Mantiuk
- 어두운 영역의 디테일 향상 확인

<img width="404" height="518" alt="image" src="https://github.com/user-attachments/assets/6906f2b8-56a3-4127-84ba-813ed30fb2df" />
<img width="433" height="672" alt="image" src="https://github.com/user-attachments/assets/be330cbe-e5c7-4ce5-95fb-cec90ea2537a" />



<details>
<summary>📱 모바일 실험 노트</summary>

Lightroom 앱으로 직접 촬영 시도했으나, 삼각대 없이는 완벽한 정렬이 어려웠고 최대 셔터스피드가 1초로 제한되어 있어 기대했던 HDR 효과를 완전히 재현하기는 어려웠습니다.
</details>

## 🚀 Getting Started

```bash
# Clone repository
git clone https://github.com/paulyu8868/Digital-Image-Processing.git

# Build (OpenCV required)
g++ -o output filename.cpp `pkg-config --cflags --libs opencv`

# Run
./output
```

## 📈 Performance & Insights

1. **노이즈 제거**: Salt & Pepper → Median Filter가 최적
2. **엣지 보존 블러링**: Bilateral Filter 사용
3. **동전 검출**: Blob Detection 파라미터 세밀 조정 필수
4. **HDR**: 최소 4장 이상의 다른 노출 영상 필요

