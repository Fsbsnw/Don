# Don
 언리얼 게임 클라이언트 포트폴리오 - 'Don'

 <img src="https://github.com/user-attachments/assets/154ec290-540d-44b5-aed1-1f4b4e26504e">
 <br>

 ※ 외부에서 받은 애니메이션 리소스가 기본 SK_Mannequin_Skeleton 용으로 제작되었는데, 다른 캐릭터 메시로 리타겟팅하는게 어려워서 그대로 사용했습니다..


## 1인 프로젝트 - 최도의

## 포트폴리오 요약
* GAS 시스템 기반 Ability 구현
* 인벤토리(MVC패턴)
* 퀘스트
* 아이템
* 장비 강화
* 상점
* 적 AI(Behavior Tree)

## 게임 모드 소개
![Image](https://github.com/user-attachments/assets/0dc82893-94c5-41f8-bbc4-038b61fd2ee1)

## 캐릭터 클래스 구조
![Image](https://github.com/user-attachments/assets/b2ac41e2-0385-4ba3-82ed-4d22c7710ec4)

## 플레이어 캐릭터

### <li>Attribute Set</li>
 <br>
 <img src="https://github.com/user-attachments/assets/1253504f-281a-406c-a0ed-75744f33b4cd" alt="Attribute Set 설명" width="500"/>
 <br>
 <br>
<details>
 <summary>Attribute Set 속성 설명</summary>
 
 <br>
 <li>Vigor -> 체력, 공격력 추가</li>
 <li>Agility -> 치명타 확률, 추가 치명타 데미지, 회피 확률</li>
 <li>Supernatural -> 스킬 쿨타임 감소, 적 추가 스폰 확률, 아이템 획득 비율, 경험치 획득 비율, 돈 획득 비율</li>
 
 ### 예시
 <br>
 <img src="https://github.com/user-attachments/assets/71c7ada2-2da0-4670-86b5-840d0b83ca95">
 
</details>

---

### <li>인벤토리</li>
 <br>
 <img src="https://github.com/user-attachments/assets/b726ab54-e899-413e-a8a4-827a0a1a429a" alt="인벤토리 설명"/>
 <br>
 <br><br><br>
 <img src="https://github.com/user-attachments/assets/bd287183-838c-4773-9c51-ea4b22f3e109">
 <br><br><br>

 <li>Model</li>
 <br>
 <img src="https://github.com/user-attachments/assets/db80e350-abc2-482f-b26f-09d0e4eb3667">
 <img src="https://github.com/user-attachments/assets/1d9ddf32-39a7-4399-8c5e-fa8f8b315b66">
 <img src="https://github.com/user-attachments/assets/748763eb-bbe9-459a-b1b1-1287bb69a59f">
 <br><br>
 <li>Controller</li>
 <br>
 <img src="https://github.com/user-attachments/assets/67566903-be56-4649-a794-abbb4df617b9">
 <br><br>
 <li>View</li>
 <br>
 <img src="https://github.com/user-attachments/assets/7db17656-4466-4864-9763-a9d221d9b8bc">
 
 ---

 ### <li>장비 강화</li>
 <img src="https://github.com/user-attachments/assets/1dba1ded-3221-49e8-a2a3-0aedba6c8479">
 <img src="https://github.com/user-attachments/assets/6a296361-4df0-41b2-a1be-fd603d99136a">
 <br>
 
 강화 : **Upgrade Crystal** 소지 시 강화 가능

 <img src="https://github.com/user-attachments/assets/078e691a-77e3-4e78-8f65-cf58cbcfc83e">

## NPC 클래스
 ### <li>상점 NPC</li>

 <img src="https://github.com/user-attachments/assets/4b5aecae-46e3-4d73-9828-eb35e1c141eb">
 
 상인 아이템 구매
 
 <img src="https://github.com/user-attachments/assets/0d8fdd18-a6d9-40a1-a38b-99f8f1568db9">
 
 플레이어 아이템 판매
 
 <img src="https://github.com/user-attachments/assets/34694690-d7ed-40ab-9d44-7a076546228f">
 <br>

 ---

 ### <li>퀘스트</li>

 <img src="https://github.com/user-attachments/assets/636849ed-f893-42cf-964f-162eb205a59a">

 퀘스트 완료 보상
 
 <img src="https://github.com/user-attachments/assets/68ce44d7-7d70-429f-a75d-1f5dd4b258f1">

## 적 클래스
 ### 일반(따라가기, 공격)
 <img src="https://github.com/user-attachments/assets/193b8254-4b01-41f5-9d36-25b6f2413198">
 <img src="https://github.com/user-attachments/assets/fd238d1e-22d4-4898-ae35-34660fdae529">
 
 ### 보스(따라가기, 원거리 공격, 근거리 공격)
 <img src="https://github.com/user-attachments/assets/822a05aa-6a38-4913-b1a3-80599c6eb19b">
 <br>

  #### 보스 AI
  <img src="https://github.com/user-attachments/assets/a915becc-cc7d-4db6-8e4a-f4d56a5510bd">
  

  <br><Gameplay Tag로 능력을 활성화>
  
  <img src="https://github.com/user-attachments/assets/e19511ea-bacb-400c-a70a-a06938277b64">

 ## 트러블 슈팅
  <details>
   <summary>Physics 적용 시, Collision의 위치 고정 문제</summary>
   <br>
   넉백 공격을 통해 Skeltal Mesh의 Physics를 활성화시키면 자연스러운 Ragdoll 모션 연출이 가능했지만,
   
   Capsule Component는 해당 시점에 고정되어 어색한 타격 판정이 발생.
   
   -> Unreal 구조상 Simulate Physics가 활성화되면 해당 **Mesh가 위치 제어의 주체**가 되며 **Capsule Component는 물리 연산에서 제외**되기 때문.

   <br>
   <img src="https://github.com/user-attachments/assets/0b5d09e6-f828-4ba2-afe5-4ea6e1486611">
   <br>
   <br>
   
   => **Capsule Component**가 **Mesh**를 따라가야 하기 때문에 Tick보단 적은 비용의 Timer를 활용하여 Capsule Component의 위치를 변경함.

   <br>
   <img src="https://github.com/user-attachments/assets/9072ca3c-f612-4061-ae66-4424405e05e6">
   <br>
   
  </details>

  <details>
   <summary>아이템 구조 설계의 어려움</summary>
   <br>
   처음엔 아이템 구조체 안에 선언된 타입별 Gameplay Tag에 따라 장착/사용/기타 등으로 동작을 분기하여 처리했으나,
   
   아이템 종류가 다양해지고 속성이 복잡해지면서 기존 방식에 구조적 한계가 발생.


   **<기존 Item 구조체>**
   <br>
   <img src="https://github.com/user-attachments/assets/fb3ec98e-0375-4055-abb4-f8c6e3e32357">
   <br>

   하나의 구조체 안에 공통 속성과 개별 속성을 통합된 상태로 선언했기 때문에, 구분이 어렵고 확장될수록 더 비대한 구조를 가지게 됨.
   
   그로 인해 **가독성 저하, 불필요한 메모리 낭비** 등 유지보수의 어려움이 발생.

   예시) 아이템의 희귀도, 희귀도에 따른 추가 속성, 장전이 필요한 무기 등이 추가될수록 더욱 복잡해짐.

   **=> 구조체의 확장성과 효율성을 어떻게 개선할 것인가?**

   1. FItem 구조체에는 **공통 속성**만을 넣고, 개별 속성들을 각각의 **데이터 애셋**에서 참조하여 사용 (각 아이템 타입에 맞는 데이터 애셋을 추가로 참조하기 때문에 번거로울 수 있음)
   2. 아이템 사용을 위한 **ItemBase** 부모 클래스를 생성하여 다형성 확보

   ---

   **<아이템 구조 분리>**

   <img src="https://github.com/user-attachments/assets/b10e95fc-64c7-45bd-9cf0-6a4cb399e97f">

   <img src="https://github.com/user-attachments/assets/2198da2b-f63b-45ae-a37c-7f4bd6795fb7">

   <img src="https://github.com/user-attachments/assets/e482ba2a-8320-4154-b735-5dc8e550d925">

   ---

   **<아이템 사용 로직>**

   <img src="https://github.com/user-attachments/assets/926ff9e9-cf5c-41cf-8f3f-56a909335fa5">

   <img src="https://github.com/user-attachments/assets/9e7a87e8-5fa9-420d-bfe4-11cea100529c">

   <img src="https://github.com/user-attachments/assets/e3f85f43-ed80-4dcb-8351-d82a9c026fd1">

   <img src="https://github.com/user-attachments/assets/a72b6b3f-73fa-427e-a15a-8d72d5bab266">
   
  </details>

 ---

  [Don 포트폴리오 영상](https://youtu.be/gn_eyMzty2I)
