# Don
 언리얼 게임 클라이언트 포트폴리오 - 'Don'

 <img width="409" height="429" alt="Image" src="https://github.com/user-attachments/assets/7191d90b-b254-4b12-9a9a-7bd03c5b7981" />
 <br>



## 1인 프로젝트 - 최도의

## 게임 장르 : 3D 뱀파이어 서바이벌류

## 기획 배경

 뱀파이어 서바이벌이 주는 **수많은 적을 쓸어버리는 핵 앤 슬래시의 쾌감**과 **매번 새롭게 강해지는 로그라이크의 재미**를 3D 환경에서 구현하고 싶었습니다.
 
 이 핵심 기능을 구현하기 위해서는 화면에 수많은 적을 동시에 처리하는 기술력이 필수적이었습니다. 특히, 무거운 기능들이 바탕이 되는 언리얼 엔진 환경에서 이러한 다수의 오브젝트를 어떻게 효율적으로 관리하고 최적화할 수 있는지에 대한 기술적 호기심이 생겼습니다.
 
 따라서 이 프로젝트는 **3D 뱀파이어 서바이벌류 게임 제작**이라는 기획적 목표와, **다수의 AI 오브젝트를 다루는 언리얼 엔진의 성능 최적화**라는 기술적 목표를 동시에 달성하는 것을 과제로 삼고 시작되었습니다.
 
## 포트폴리오 요약
* 최적화 시도들
* GAS 시스템 기반 Ability 구현
* 인벤토리(MVC패턴)
* 퀘스트
* 아이템
* 장비 강화
* 상점
* 적 AI(Behavior Tree)

## [Don 포트폴리오 영상](https://youtu.be/gn_eyMzty2I)

## 최적화 시도
**<문제 상황>** <br>
다수의 적이 스폰된 상황에서, 적이 **50마리만 스폰되어도 프레임이 73 FPS에서 39 FPS로 급락**하는 심각한 성능 저하가 발생

<br>

**<문제 원인 파악>** <br>
Stat Game을 통한 프로파일링 <br>
⇒ **Char Movement Total, EndScopedMovementUpdate Time, UpdateOverlaps Time, PerformOverlapQuery Time** 이 네 가지 항목에서 많은 비용이 발생

<br>

**<시도한 해결 방안>** <br>
1. Enemy Object 전용 콜리전 사용 및 캡슐 컴포넌트의 물리 충돌 Ignore
2. RVO 사용

<img width="1347" height="473" alt="Image" src="https://github.com/user-attachments/assets/16070b8e-bf9c-4a42-abbf-7563e54b9513" />

⇒ 여전히 **Char Movement Total**의 비용이 높음, 39 FPS 근처

<br>

3. Overlap 관련 비용 원인 탐색 → **Mesh overlap events**이 불필요하게 켜져 있었고, 이를 해제한 상태

<img width="1372" height="476" alt="Image" src="https://github.com/user-attachments/assets/3999fe3d-4720-46cd-994b-782f4043a17a" />

⇒ **UpdateOverlaps Time, PerformOverlapQuery Time** 감소 <br>
⇒ 하지만 여전히 **Char Movement Total**의 비용이 높음, 유의미한 FPS 변화 없음 <br>
⇒ **Character Movement Component** 자체를 사용하면 안 되는 상황

<br>

4. **Character Movement Component → Floating Pawn Movement**로 변경 <br>
   ⇒ 다수의 AI에게 불필요한 네트워크, 물리 상호작용 기능이 없는 경량 컴포넌트로 교체하여 이동 비용 절감

<img width="1367" height="482" alt="Image" src="https://github.com/user-attachments/assets/7b0e8a22-4ac8-46c8-bc2b-ad754a601481" />

⇒ 비용 자체가 확 줄었고, 60 FPS 근처 유지 가능 <br>
⇒ 하지만 캡슐 컴포넌트 콜리전의 Block 상태로 인해 물리 계산 비용이 좀 더 발생하는 듯.

<br>

5. 물리 충돌을 Ignore함에 따라 발생하는 AI 뭉침 현상을 완화하기 위해 각 AI에게 플레이어 주변의 랜덤 좌표를 할당하는 <br>
   커스텀 **BTTaskNode_MoveToLocationAndRepath** 노드 사용

<img width="1345" height="525" alt="Image" src="https://github.com/user-attachments/assets/b8aa0770-f962-42ab-b487-5206a74a4023" />

<br><br>

<BTTaskNode_MoveToLocationAndRepath - TickTask>

<img width="722" height="398" alt="Image" src="https://github.com/user-attachments/assets/59248caa-4225-4829-bf49-e444c45d929f" /> <br><br>

**<최종 결과>** <br>
프레임 드랍의 주요 원인 4가지의 비용을 현저히 저하시킴으로써 <br> 
**39** FPS -> **60** FPS <br>
**25** ms -> **17** ms <br>
만큼의 개선 가능
<br><br>

 ## 트러블 슈팅
  <details>
   <summary>Physics 적용 시, Collision의 위치 고정 문제</summary>
   <br>
   넉백 공격을 통해 Skeltal Mesh의 Physics를 활성화시키면 자연스러운 Ragdoll 모션 연출이 가능했지만,
   
   **Capsule Component는 해당 시점에 고정되어 어색한 타격 판정**이 발생.
   
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
   
   아이템 **종류가 다양**해지고 **속성이 복잡**해지면서 기존 방식에 **구조적 한계**가 발생.


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
