# [졍빈이] 2022- 1 캡스톤디자인 1
---
# Introduction
PX4의 로그 데이터 무결성 검증을 위한 파일을 만드는 라이브러리입니다.
## Contributer 
<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
  <tr>
    <td align="center"><a href="https://github.com/sju0924"><img src="https://avatars.githubusercontent.com/u/39671049?v=4" width="100px;" alt=""/><br /><sub><b>손지언</b></sub></a><br /><a href=" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/kimbongsik"><img src="https://avatars.githubusercontent.com/u/63995044?v=4" width="100px;" alt=""/><br /><sub><b>유영빈</b></sub></a><br /><a href="" title="Code">💻</a></td>
  </tr>
</table>

<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->

<!-- ALL-CONTRIBUTORS-LIST:END -->
# 적용 대상
* /fs/microsd/log
  * /yyyy-mm-dd/{hh_mm_ss}.ulg
  * /sess\<i>/log\\{3}.ulg
<img width="566" alt="image" src="https://user-images.githubusercontent.com/39671049/174072111-16f8536e-239a-449e-9137-e2b99eda1d75.png">
<img width="530" alt="image" src="https://user-images.githubusercontent.com/39671049/174072196-512956cd-aa28-4e7d-bc4c-e8ff99f23d40.png">
  
![image](https://user-images.githubusercontent.com/39671049/174072258-66a6d82c-b7a0-4c1e-af95-50308d24881c.png)  
![image](https://user-images.githubusercontent.com/39671049/174072226-8c4d76b6-072c-4251-b573-e2db9274a787.png)

# 사용 방법
## ulog 파일에 적용

### client
* ./convert \[ulog 파일 경로] 실행
 
### backend
* .ulg -> .csv 파일로 변환
* csv 파일 읽기
* 로그를 해시하여 머클 트리 노드에 추가
* 머클 트리 생성
* 머클 루트 생성, 블록 생성
* 블록 연결, 체인 구성

### product
* 체인 정보를 담은 binary file
![image](https://user-images.githubusercontent.com/39671049/174093179-2a9be5f2-c609-4556-9ce9-626a4b84419d.png)
