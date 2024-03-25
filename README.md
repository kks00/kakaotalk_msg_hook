# 카카오톡 송/수신 메세지 후킹

# 1. Overview

- ## 관계도
    ![image](https://github.com/kks00/kakaotalk_msg_hook/assets/68108664/b70d0739-f212-4fc5-a59c-8d82565d9be2)

<br>

- ## 개발 언어
    - ### 모듈: C++
    - ### 클라이언트: Delphi(Pascal)

<br>

- ## 역할
    - ### kakaotalk_module
        - ### 송/수신 발생시 메시지를 클라이언트에 ShareMemory를 통해 보내줌

    - ### kakaotalk_client
        - ### Module에서 전송해준 메시지를 Form의 ListView에 렌더링함
        ![image](https://github.com/kks00/kakaotalk_msg_hook/assets/68108664/78a7adcb-edba-48db-aac9-e9416f22a77b)

---
<br><br>

# 2. 