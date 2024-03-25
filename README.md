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

<br>

- ## 예상 활용 가능 분야
    - ### 카카오톡 채팅을 이용한 리모콘
    - ### 카카오뱅크 알림 문자를 이용한 자동 결제처리 시스템
    - ### 채팅봇

---
<br><br>

# 2. 원리

- ### 카카오톡은 메세지를 발신, 수신하였을 때 내부의 SQLite3 데이터베이스에 발신자, 메세지 텍스트 데이터등을 기록하는데, DB에 기록하는 함수를 가로채어 메시지를 얻어올 수 있다.


    - ### 1. 두번째 인자로 사용할 쿼리문 문자열의 주소를 받는 함수
        ![image](https://github.com/kks00/kakaotalk_msg_hook/assets/68108664/6ea57e48-8211-408a-8169-854f8eea7942)

    - ### 2. 채팅 내용이 추가될 때 INSERT INTO chatLogs((logId, authorId, type, clientMsgId, sendAt, message, attachement, ... ) 쿼리 발생 
        ![Honeycam 2024-03-25 19-41-03](https://github.com/kks00/kakaotalk_msg_hook/assets/68108664/fe40ed59-651e-4670-b43b-120ed675631b)

    - ### 3. message 자리에 들어갈 문자열을 후킹하여 가져올 수 있음
        ![Honeycam 2024-03-25 19-47-32](https://github.com/kks00/kakaotalk_msg_hook/assets/68108664/9556bcee-e0c4-4f35-86ea-1e3a1cbbbda1)


---
<br><br>