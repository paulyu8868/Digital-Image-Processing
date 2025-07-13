# Digital Image Processing

C++/OpenCV를 활용한 디지털 영상처리 구현 프로젝트

## 📌 Overview

다양한 디지털 영상처리 기법들을 C++과 OpenCV를 사용하여 직접 구현한 프로젝트입니다. 기본적인 필터링부터 고급 영상처리 기법까지 폭넓게 다루고 있습니다.

## 🛠️ Tech Stack

- **Language**: C++
- **Library**: OpenCV
- **Topics**: Image Filtering, Edge Detection, Feature Detection, Image Transformation, Segmentation, Color Processing, HDR Imaging

## ✨ Features

### 📸 Image Filtering

#### **Gaussian Filter** 
> 이미지를 부드럽게 만들어주는 블러링 필터

- 9x9 크기의 가우시안 필터를 직접 구현
- 가우시안 분포를 따르는 마스크를 이용한 Convolution 연산
- 3x3 필터보다 더 강한 블러링 효과 확인

<details>
<summary>💡 Implementation Details</summary>

정규화를 위해 마스크값의 합으로 나누어주는 과정을 추가했으며, 히스토그램 분석을 통해 불연속적이던 밝기값이 연속적으로 변화하는 것을 확인했습니다.
</details>

#### **Median Filter**
> Salt & Pepper 노이즈 제거에 특화된 필터

- 3x3, 5x5 크기의 미디언 필터 구현
- 주변 픽셀값들을 정렬하여 중간값으로 대체
- **Double Median Filter**로 강력한 노이즈 제거 달성

<details>
<summary>💡 Key Findings</summary>

가우시안 필터와 달리 충격성 노이즈 제거에 탁월했으며, 두 번 적용하는 방식으로 잔여 노이즈까지 완벽히 제거할 수 있었습니다.
</details>

#### **Bilateral Filter**
> 엣지를 보존하면서 노이즈를 제거하는 스마트한 필터

- 공간 도메인과 색상 도메인을 동시에 고려
- 가우시안 필터의 경계선 무너짐 문제 해결
- gr(range filter)과 gs(spatial filter)의 가중치 조합

### 🔍 Edge Detection

#### **Sobel Filter**
> 특정 방향의 엣지를 검출하는 필터

- 45도, 135도 대각선 엣지 검출 구현
- 그래디언트 기반 엣지 검출
- 두 마스크의 절댓값 합으로 최종 엣지 결정

#### **Canny Edge Detection**
> 가장 널리 사용되는 엣지 검출 알고리즘

- Double threshold와 Hysteresis를 통한 정확한 엣지 검출
- Low threshold: 0~150, High threshold: 50~300 범위 실험
- 임계값 조정으로 원하는 수준의 엣지만 추출 가능

### 🎯 Feature Detection

#### **Smart Coin Counter**
> Blob Detection을 활용한 동전 개수 자동 계산

```cpp
// 핵심 파라미터 조정
params.minCircularity = 0.3;     // 원형 판단 기준 완화
params.minArea = 100;             // 작은 노이즈 제거
params.maxArea = 8000;            // 500원 동전까지 검출
```

초기 5개 → 최종 14개 검출 성공! 💰

#### **Shape Classifier**
> Harris Corner + Blob Detection 조합

- 도형의 꼭짓점 자동 검출
- 삼각형(3), 사각형(4), 오각형(5) 등 정확한 분류
- 중복 검출 문제를 threshold 조정으로 해결

#### **SIFT Features**
> 크기와 회전에 강인한 특징점 검출

- Scale & Rotation invariant ✅
- Brightness change sensitive ⚠️
- Perspective transform vulnerable ⚠️

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

#### **Auto Card Rectification**
> 기울어진 카드를 자동으로 정면 변환

1. Harris Corner로 카드 모서리 검출
2. Threshold로 배경 제거
3. Perspective Transform으로 정면 변환

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

#### **GrabCut**
> 스마트한 전경/배경 분리

✅ **잘 되는 경우**: 색상 대비가 뚜렷한 이미지  
❌ **한계**: 전경/배경 색상이 유사한 경우

### 🌈 Color Processing

#### **RGB to HSV Converter**
> 색상 공간 변환 및 특정 색상 검출

- RGB → HSV 변환 공식 직접 구현
- inRange를 통한 색상 마스킹
- 과일 색상 자동 분류 시스템 구현

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

### 🏆 Best Practices Learned

1. **노이즈 제거**: Salt & Pepper → Median Filter가 최적
2. **엣지 보존 블러링**: Bilateral Filter 사용
3. **동전 검출**: Blob Detection 파라미터 세밀 조정 필수
4. **HDR**: 최소 4장 이상의 다른 노출 영상 필요

